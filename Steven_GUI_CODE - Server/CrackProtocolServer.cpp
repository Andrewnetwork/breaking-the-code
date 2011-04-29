// CrackProtocolServer.h
// Andrew Ribeiro 
// http://www.AndrewRibeiro.com
// April 10, 2011
/*******************************************************
** This code was created by Andrew Ribeiro whom        *
** retains the rights to all the material presented in *
** this document, unless stated otherwise. The usage   *
** of this code is granded with no liability to the    *
** creator of this material.                           *
********************************************************/

#include <Windows.h>
#include <string> 
#include <vector>
#include <sstream>
#include <fstream>


#include "SocketCommunicator.h"
#include "CrackProtocolConstants.h"

#include "CodeCrackingState.h"
#include "CrackProtocolServer.h"


using namespace AndrewRibeiro; 
using namespace std; 

void PushStringOntoBuffer(char *str);

CrackProtocolServer::CrackProtocolServer( SOCKET activeConnection , CodeCrackingState * globalState, long unsigned int permutationInt)
{
	this->gobalState = globalState;
	this->connection = activeConnection;
	this->permutationInt = permutationInt;
	this->connectionCom = new SocketCommunicator( activeConnection );
	this->rangeAssignedInProgress = false;
	this->gobalState->addClient();
}

bool CrackProtocolServer::processRequests()
{
	string clientMessage;
	string keyword;
	
	clientMessage = this->connectionCom->read();

	while( clientMessage.size() != 0 )
	{
		keyword = clientMessage[0];
		keyword += clientMessage[1];

		if( keyword.compare( KEY_START_PROCESS ) == 0 )
		{
			startProcess();
		}
		else if( keyword.compare( KEY_GET_PROCESS_STATUS ) == 0 )
		{
			sendProcessStatus();
		}
		else if( keyword.compare( KEY_RANGE_COMPLETE ) == 0 )
		{
			recvRangeComplete();
		}
		else if( keyword.compare( KEY_REQUEST_INFO ) == 0 )
		{
			requestInformation();
		}
		else if( keyword.compare( KEY_GET_RANGE ) == 0 )
		{
			sendRange();
		}
		else if( keyword.compare( KEY_GET_VERIFICATION_ARCHIVE ) == 0 )
		{
			sendVerificationArchive();
		}
		else
		{
			// Unsuported Operation.
			sendInvalidOperationMessage( "ERROR: "+keyword+"is an invalid/unsupported operation.");
		}

		clientMessage = this->connectionCom->read();
	}
	// End of processing.

	PushStringOntoBuffer("A client disconnected.");

	
	// If the client was in the middle of a permutation range, add it to the incomplete 
	// range list. 
	if( this->rangeAssignedInProgress )
	{
		this->gobalState->addIncompletedRange( this->rangeAssigned );
	}
	
	this->gobalState->removeClient();
	closesocket( this->connection );
	delete this->connectionCom;
	

	return true;
}

string CrackProtocolServer::getFileContents( string filePath )
{
	stringstream strStream;

	ifstream inputFile( filePath.c_str() , ios::binary | ios::in);

	inputFile.seekg (0, ios::end);
	int fileLength = inputFile.tellg();
	inputFile.seekg (0, ios::beg);

	char * fileBuffer  = new char[ fileLength ];

	inputFile.read(fileBuffer, fileLength );
	strStream.write(fileBuffer, fileLength );
		
	return strStream.str();
}

bool CrackProtocolServer::makeDirectory( string name, string bin )
{
	fstream outFile( name.c_str() , ios::app | ios::binary);

	outFile.write( bin.c_str() , bin.size() );

	outFile.close();

	return !outFile.fail();
}

// Protocol Functions 

bool CrackProtocolServer::recvRangeComplete() 
{
	string result = this->connectionCom->read();
	string bufferString;
	
	if( result.size() == 0 || ( result.size() == 1 && result[0] == BLANK_CHAR ) )
	{
		bufferString = "A client completed a non-result yealding permutation range.";
	}
	else
	{
		// TODO: update record in db. Jaynam.

		bufferString = "A client cracked the code.\n  Result: "+result;
		this->gobalState->clearVerificationArchive();
	}

	PushStringOntoBuffer((char *) bufferString.c_str() );

	rangeAssignedInProgress = false;

	return true;
}

bool CrackProtocolServer::sendProcessStatus()
{
	string out = "0";

	if( this->gobalState->processStatus() )
	{
		out = "1";
	}

	return this->connectionCom->write( out );
}

void CrackProtocolServer::requestInformation()
{
	// Jaynams work
}

void CrackProtocolServer::startProcess()
{
	// If not cracking a code. 
	if( !this->gobalState->processStatus() )
	{
		string name, email, binArchive; 

		name = this->connectionCom->read();
		email = this->connectionCom->read();

		// TODO: Insert items into record. Jaynam.

		binArchive = this->connectionCom->read();

		addProcessRecordToDB( name, email );

		this->gobalState->setVerificationArchive( binArchive );
		this->gobalState->newCounter();
	}
}

bool CrackProtocolServer::addProcessRecordToDB( string name, string email )
{
	// TODO: work with jaynam to finish this.
	return true;
}

void CrackProtocolServer::sendRange()
{
	if( this->gobalState->processStatus() )
	{
		pair< vector<unsigned char>,vector<unsigned char>> range = this->gobalState->getRange(this->permutationInt);

		string lowerBound="", upperBound="";

		for( int i = 0; i < range.first.size(); i++)
		{
			lowerBound += range.first[i];
		}

		for( int i = 0; i < range.second.size(); i++)
		{
			upperBound += range.second[i];
		}

		rangeAssignedInProgress = true;
		rangeAssigned = range;

		this->connectionCom->write( lowerBound );
		this->connectionCom->write( upperBound );
	}
	else
	{
		// No proecess is in execution.
		this->connectionCom->write( "0" );
		this->connectionCom->write( "0" );
	}
}

void CrackProtocolServer::sendVerificationArchive()
{
	// While there is no code craking process taking place.
	while( !this->gobalState->processStatus() );

	this->connectionCom->write( this->gobalState->getVerificationArchive() );
}

void CrackProtocolServer::sendInvalidOperationMessage(string errorMessage)
{

}
