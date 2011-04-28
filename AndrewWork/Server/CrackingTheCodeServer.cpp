// CrackingTheCodeServer.cpp
// Andrew Ribeiro 
// http://www.AndrewRibeiro.com
// April 13, 2011
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
#include <map>
#include <fstream>
#include <list>

#include "..\GlobalHeaders\SimpleSocketsLibrary\SimpleServerSocket.h"
#include "..\GlobalHeaders\SimpleSocketsLibrary\SimpleSocket.h"
#include "..\GlobalHeaders\SimpleSocketsLibrary\SocketCommunicator.h"


#include "CrackProtocolServer.h"
#include "CodeCrackingState.h"
#include "CrackingTheCodeServer.h"


using namespace AndrewRibeiro; 
using namespace std; 

void PushStringOntoBuffer(char *str);

DWORD WINAPI handleClient( LPVOID arg )
{
	SOCKET clientConnection                 = ( SOCKET ) ( ( void ** ) arg )[0];
	CodeCrackingState * globalState         = ( CodeCrackingState * ) ( (void ** ) arg )[1];
	unsigned long int * permutationInt      = ( unsigned long int * ) ( (void ** ) arg )[2];

	CrackProtocolServer clientInterface( clientConnection, globalState, *permutationInt );

	clientInterface.processRequests();


	return 1;
}

DWORD WINAPI listenForClientsClients( LPVOID arg )
{
	CrackingTheCodeServer * server = ( CrackingTheCodeServer * ) arg;
	bool clientConnectionError     = false;
	string promptString;
	list< HANDLE > clientThreads;

	// Connect to client, and store socket/connection in 'clientConnection'
	do
	{
		// # Connecting the the server 'serverIP', on port 'serverPort'
		SOCKET clientConnection  = server->serverSocket.listenForConnection();

		if( clientConnection == NULL )
		{
			// Unable to connect.
			clientConnectionError = true;
		}
		else
		{
			// Connected to the server. 
			clientConnectionError = false; 

			void * threadArgsArray[3] = { (void * )clientConnection , server->serverState, &server->permutationInt};
	
			clientThreads.push_back( CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size  
            handleClient,       // thread function name
            threadArgsArray,          // argument to thread function 
            0,                      // use default creation flags 
            NULL) );   // returns the thread identifier 


			promptString = "Client connected.";
			PushStringOntoBuffer( (char *)promptString.c_str() );
		}

	}while( !clientConnectionError && server->isRunning );

	// Terminate all client threads.
	for( list<HANDLE>::iterator i = clientThreads.begin(); i != clientThreads.end(); i++ )
	{
		TerminateThread( *i , 0 );
	}

	return 1;
}

map<string,string> CrackingTheCodeServer::getConfigurationVariables()
{
	map<string,string> configVars;

	ifstream configFile("serverConfig.txt",ios::in);

	string line;
	int indexOfFirstSpace;

	while( !configFile.eof() )
	{
		getline( configFile,line,'\n');

		indexOfFirstSpace = line.find_first_of(' ');

		if( indexOfFirstSpace != string::npos )
		{
			configVars.insert( pair< string, string >( line.substr(0,indexOfFirstSpace ), line.substr(indexOfFirstSpace+1, line.size() - indexOfFirstSpace ) ) ); 
		}
	}

	return configVars;
}

CrackingTheCodeServer::CrackingTheCodeServer()
{
	// Load configuration variables from serverConfig.txt
	map<string,string>  configVars = getConfigurationVariables();
	port                           = configVars["Server_Port"];
	permutationInt                 = atoi( configVars["Permutation_Interval"].c_str() );
	string alphaStr                = configVars["Alphabet_Pairs"]; 
	
	// Initialize and calculate initial values.
	isRunning           = false; 
	
	
	// Calculate the alphabet size given the ASCII ranges. 
	alphabetSize        = 0;
	unsigned int subVal = 0;

	for( int i = 0; i < alphaStr.size(); i += 2)
	{	
		if( subVal == 0 )
		{
			subVal = alphaStr[i];
		}
		else
		{
			alphabetSize += (alphaStr[i] + 1) - subVal;
			subVal = 0;
		}
	}
}

void CrackingTheCodeServer::start()
{
	// If the server is not started.
	if( !isRunning)
	{
		isRunning = true;

		serverState      = new CodeCrackingState( alphabetSize );

		string promptString;

		if (serverSocket.bindToPort( port ) )
		{
			promptString = "Waiting for incoming connections on port "+port+".";
			PushStringOntoBuffer( (char *)promptString.c_str() );

			CreateThread( 
				NULL,                   // default security attributes
				0,                      // use default stack size  
				listenForClientsClients,       // thread function name
				this,          // argument to thread function 
				0,                      // use default creation flags 
				NULL);   // returns the thread identifier 
		}
		else
		{
			promptString = "Could not bind to port "+port+".";
			PushStringOntoBuffer( (char *)promptString.c_str() );
		}
	}
}

void CrackingTheCodeServer::stop()
{
	// If the server is not stoped.
	if( isRunning )
	{
		isRunning = false;

		delete serverState;

		this->serverSocket.unbind();
	}
}



