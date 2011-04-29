// CrackProtocolClient.cpp
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

#include "CrackingTheCodeClient.h"
#include "CrackProtocolClient.h"
#include "SimpleClientSocket.h"
#include "SimpleSocket.h"
#include "SocketCommunicator.h"
#include "threadArgumentStructures.h"

using namespace AndrewRibeiro; 
using namespace std; 

void PushStringOntoBuffer(char *str);

// ## START Constructor(s) ##
CrackProtocolClient::CrackProtocolClient( SOCKET activeConnection)
{
	this->connection                = activeConnection; 
	this->connectionCom             = new SocketCommunicator( activeConnection );
}
// ## END Constructor(s) ##

// ## START Protocol Functions ##
bool CrackProtocolClient::startProcess( string name, string email, string verificationArchPath )
{
	// If a code is not currently being cracked.
	if( ! this->getProcessStatus() )
	{
		this->connectionCom->write(KEY_START_PROCESS);

		this->connectionCom->write(name);

		this->connectionCom->write(email);

		this->connectionCom->write( getFileContents( verificationArchPath ) );

		return true; 
	}
	else
	{
		return false;
	}
}

bool CrackProtocolClient::getProcessStatus()
{
	this->connectionCom->write(KEY_GET_PROCESS_STATUS );

	return atoi( this->connectionCom->read().c_str() );;
}

void CrackProtocolClient::rangeComplete( string crackedCode )
{
	this->connectionCom->write( KEY_RANGE_COMPLETE );

	if( crackedCode.size() == 0 )
	{
		string garbage ="";
		garbage += BLANK_CHAR;
		this->connectionCom->write( garbage );
	}
	else
	{
		this->connectionCom->write( crackedCode );

		string outputBuff =  "You cracked the code!\n Result: "+crackedCode;

		PushStringOntoBuffer((char * )outputBuff.c_str());
	}
}

pair<string, string > CrackProtocolClient::getRange()
{
	string messageBuffer;
	char charToNum[3];

	messageBuffer+="Getting permutation range from server.\n";

	pair<string, string > rangeOut;

	this->connectionCom->write( KEY_GET_RANGE );

	string serverMessage; 

	serverMessage = this->connectionCom->read();

	rangeOut.first = serverMessage;

	messageBuffer += "  Range Start: | ";

	for( int i = 0; i < serverMessage.size(); i++)
	{
		itoa((int)serverMessage[i],charToNum,10 );

		messageBuffer += charToNum;
		messageBuffer += " | ";
	}

	messageBuffer += "\n";

	serverMessage = this->connectionCom->read();
	
	rangeOut.second = serverMessage;

	messageBuffer += "  Range End: | ";

	for( int i = 0; i < serverMessage.size(); i++)
	{
		itoa((int)serverMessage[i],charToNum,10 );

		messageBuffer += charToNum;
		messageBuffer += " | ";
	}

	messageBuffer += "\n";

	if( rangeOut.first.compare( rangeOut.second ) != 0 )
	{
		PushStringOntoBuffer( (char *) messageBuffer.c_str() );
	}

	return rangeOut;
}

string CrackProtocolClient::getVerificationArchive()
{
	this->connectionCom->write( KEY_GET_VERIFICATION_ARCHIVE );

	makeDirectory( "VerificationFolder.zip" , this->connectionCom->read() );

	unzipFile( "VerificationFolder.zip" );

	return "VerificationFolder";
}
// ## END Protocol Functions ##

// ## START Utlility Functions ##
string CrackProtocolClient::getFileContents( string filePath )
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

string CrackProtocolClient::requestInformation()
{
	// Jaynam's Job...
	// The output string should be ready to print directly to the screen.
	// Use \n to indicate new lines. 
	// The > character will be automatically added to new lines. 

	this->connectionCom->write( KEY_REQUEST_INFO );


	return this->connectionCom->read();
}

bool CrackProtocolClient::unzipFile( string name )
{
	STARTUPINFO         startUpInfo;
	PROCESS_INFORMATION procInfo;
	BOOL                success;
	char                appName[256];
	DWORD               processExitCode; 
	LPDWORD			  ptrProccessExitCode = &processExitCode; 

 
	strcpy (appName, "7zip\\7z.exe\0" );

	// 2. Retrieve the STARTUPINFOR structure for current process
	GetStartupInfo(&startUpInfo);

	string args = "7z x -y ";
	args += name;

	// 3. Create the child process
	success = CreateProcess(
					appName, // or NULL    // app. name
					(LPSTR)args.c_str(),    // c:\ ... // command line
					NULL,   // security
					NULL,   // thread security
					FALSE,  // do not inherit handles of parent process
					DETACHED_PROCESS, // various constants
					NULL,      // points to array of string containing environment variables
					NULL,      // Starting drive and directory for the process
					&startUpInfo, // appearance ifno
					&procInfo     // return info for process
							);


	WaitForSingleObject( procInfo.hProcess, INFINITE );

	return success;
}

bool CrackProtocolClient::makeDirectory( string name, string bin )
{
	fstream outFile( name.c_str() , ios::app | ios::binary);

	outFile.write( bin.c_str() , bin.size() );

	outFile.close();

	return !outFile.fail();
}
// ## END Utlility Functions ##