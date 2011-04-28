// CrackingTheCodeClient.cpp
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

#include <map>
#include <string>
#include <fstream>
#include <queue>

#include "..\GlobalHeaders\SimpleSocketsLibrary\SimpleClientSocket.h"
#include "..\GlobalHeaders\SimpleSocketsLibrary\SimpleSocket.h"
#include "..\GlobalHeaders\SimpleSocketsLibrary\SocketCommunicator.h"

#include "threadArgumentStructures.h"
#include "CrackProtocolClient.h"
#include "CrackingTheCodeClient.h"

using namespace AndrewRibeiro; 
using namespace std; 

void PushStringOntoBuffer(char *str);

// ## START Utility Functions ##
SOCKET connectToServer( string serverIP, string serverPort )
{
	SimpleClientSocket clientSock;

	return clientSock.connectToServer(serverIP,serverPort);
}

map<string,string> getConfigurationVariables()
{
	map<string,string> configVars;

	ifstream configFile("clientConfig.txt",ios::in);

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

bool CrackingTheCodeClient::startVerificationServer()
{
	PushStringOntoBuffer("Starting verification service");

	SOCKET verificationConn  = connectToServer("127.0.0.1", this->verificationPort );

	// If the previous verification service is still running, 
	// wait untill it closes before attempting to open a new one.
	while( verificationConn != NULL );

	while( verificationConn == NULL )
	{
		
		STARTUPINFO         startUpInfo;
		PROCESS_INFORMATION procInfo;
		BOOL                success;
		char                appName[256];
		DWORD               processExitCode; 
		LPDWORD			    ptrProccessExitCode = &processExitCode; 

 
		strcpy (appName, "VerificationFolder\\VerificationProgram.exe\0" );

		// 2. Retrieve the STARTUPINFOR structure for current process
		GetStartupInfo(&startUpInfo);

		char alphaSizeBuff[ 5 ];

		itoa(this->alphabetSize,alphaSizeBuff,10);

		// Port
		string args = "VerificationProgram.exe "+verificationPort+" "+alphaSizeBuff+" "+this->alphabet;

		// 3. Create the child process
		success = CreateProcess(
						appName, // or NULL    // app. name
						(LPSTR)args.c_str(),    // c:\ ... // command line
						NULL,   // security
						NULL,   // thread security
						true,  // do not inherit handles of parent process
						DETACHED_PROCESS , // various constants CREATE_NO_WINDOW
						NULL,      // points to array of string containing environment variables
						NULL,      // Starting drive and directory for the process
						&startUpInfo, // appearance ifno
						&procInfo     // return info for process
								);

		Sleep( 2000 );

		verificationConn  = connectToServer("127.0.0.1", this->verificationPort );
	}

	closesocket( verificationConn );

	PushStringOntoBuffer("Connected verification service");

	return true;
}
// ## END Utility Functions ##

// ## START Constructor(s) ##
CrackingTheCodeClient::CrackingTheCodeClient()
{
	map<string,string>  configVars = getConfigurationVariables();

	serverIP                  = configVars["Server_IP"]; 
	serverPort                = configVars["Server_Port"]; 
	verificationPort          = configVars["Verification_Port"];
	alphabet                  = configVars["Alphabet_Pairs"];
	numberOfThreads           = configVars["Worker_Threads"];

	alphabetSize = 0;

	unsigned int subVal = 0;

	for( int i = 0; i < alphabet.size(); i += 2)
	{
		if( subVal == 0 )
		{
			subVal = alphabet[i];
		}
		else
		{
			alphabetSize += (alphabet[i] + 1) - subVal;
			subVal = 0;
		}
	}
}
// ## END Constructor(s) ##

// ## START Threading Functions/Methods ##
DWORD WINAPI crackCodeThread( LPVOID arg )
{
	CodeCrackingThreadArgs *   args              =  ( CodeCrackingThreadArgs *  ) arg ;
	SimpleClientSocket *       serverSock        = NULL;
	SOCKET serverConnection          = NULL;
	SOCKET verificationServerConn    = NULL;
	SocketCommunicator * verificationCom = NULL;
	string verificationResult;
	
	
	verificationServerConn = connectToServer("127.0.0.1",args->verificationPort);
	serverConnection = connectToServer(args->serverIP,args->serverPort);

	// Successfully connected to servers

	if( verificationServerConn != NULL && serverConnection != NULL )
	{
		CrackProtocolClient serverInterface( serverConnection);
		verificationCom = new SocketCommunicator( verificationServerConn );
		
		bool codeCracked = false;
	
		// Get ranges and generate permutations, while checking them with the verification server. 
		while( !codeCracked && !*args->exitFlag)
		{
			pair<string, string > range = serverInterface.getRange();
			
			// If the ranges are equal, that is a signal to exit. 
			if( range.first.compare( range.second ) != 0 )
			{
				verificationCom->write( range.first );
				verificationCom->write( range.second );

			
				verificationResult = verificationCom->read();
				
				if(verificationResult == "")
				{
					// Connection closed.
					*args->exitFlag = true;
				}
				else if( verificationResult == " ")
				{
					serverInterface.rangeComplete ( RANGE_YEILDED_NO_RESULTS );
				}
				else
				{
					// Range yeilded results. 
					serverInterface.rangeComplete ( verificationResult );
				}
			}
			else
			{
				*args->exitFlag = true;
			}
		}
	}


	//delete args->exitFlag;

	closesocket( serverConnection );
	closesocket( verificationServerConn );

	if( serverSock != NULL )
	{
		delete serverSock; 
	}

	if( verificationCom != NULL )
	{
		delete verificationCom;
	}

	delete args;
	
	return 0;
}

DWORD WINAPI processStopCheck( LPVOID arg )
{
	CrackingTheCodeClient * client = ( CrackingTheCodeClient *) arg ;

	SOCKET serverConnection = NULL;
	
	// While the thread cannot connect to the server, try to connect. 
	do
	{
		serverConnection = connectToServer(client->serverIP, client->serverPort);

	}while( serverConnection == NULL );

	CrackProtocolClient serverInterface( serverConnection );

	// Wait until process has stoped. 
	while( serverInterface.getProcessStatus() );

	// Kill the previous code cracking threads. 
	client->stopCrackingCodeThreads();

	closesocket( serverConnection );

	// Crack the code once more.
	client->crackCode();

	return 0;
}

HANDLE CrackingTheCodeClient::startNewCodeCrackingThread(bool * exitFlag)
{
	CodeCrackingThreadArgs * threadArgs = new CodeCrackingThreadArgs( serverIP, serverPort, exitFlag, verificationPort, alphabet, alphabetSize);

	HANDLE hThread = CreateThread( 
						NULL,                   // default security attributes
						0,                      // use default stack size  
						crackCodeThread,       // thread function name
						threadArgs,          // argument to thread function 
						0,                      // use default creation flags 
						NULL);   // returns the thread identifier 

	return hThread;
}
// ## END Threading methods ##

// ## START Use Case Methods ##
void CrackingTheCodeClient::crackCode()
{
	// If the exit flags are empty, that means the code is not being cracked.
	// Do not start cracking the code again until it is stopped.
	if( this->codeCrackingExitFlags.empty() )
	{
		string promptString;
		SOCKET serverCon = connectToServer(serverIP,serverPort);

		if( serverCon != NULL )
		{
			CrackProtocolClient serverInterface( serverCon );

			serverInterface.getVerificationArchive();

			startVerificationServer();
	
			// Find out how many CPU's the client has and utilize them all.
			SYSTEM_INFO sysinfo;
			GetSystemInfo( &sysinfo );

			// Doubling the number of threads launched because there will be a large wait time in each thread.
			unsigned int nCPU;
	

			if( this->numberOfThreads.substr(0,5).compare("CPU *") == 0)
			{
				nCPU = sysinfo.dwNumberOfProcessors * atoi( this->numberOfThreads.substr(6,this->numberOfThreads.size() ).c_str() );
			}
			else if( this->numberOfThreads.compare("CPU") == 0 )
			{
				nCPU = sysinfo.dwNumberOfProcessors;
			}
			else
			{
				 nCPU = atoi( this->numberOfThreads.c_str() );
			}
			

			bool * pFlag;

			// For each CPU assign a thread.
			for( int i = 0; i < nCPU; i++)
			{
				pFlag = new bool;
				*pFlag = false;

				codeCrackingExitFlags.push_back( pFlag );
				startNewCodeCrackingThread( pFlag );
			}

		

			// Wait until the process has started on the server
			while( !serverInterface.getProcessStatus() );

			// Create a thread to check if the process has stoped. 
			CreateThread(   NULL,                   // default security attributes
							0,                      // use default stack size  
							processStopCheck,       // thread function name
							this,          // argument to thread function 
							0,                      // use default creation flags 
							NULL);   // returns the thread identifier 

			closesocket( serverCon );

			promptString = "Code is being cracked.";
			PushStringOntoBuffer( (char *)promptString.c_str() );
		}
		else
		{
			promptString = "Fatal Error: Could not connect to server.";
			PushStringOntoBuffer( (char *)promptString.c_str() );
		}
	}
	else
	{
		PushStringOntoBuffer( "Code cracking is already in progress.");
	}
}

void CrackingTheCodeClient::startProcess(string processName, string email, string verificationArchivePath)
{
	string promptString;
	SOCKET serverCon = connectToServer( serverIP, serverPort );
	
	if( serverCon != NULL )
	{
		CrackProtocolClient serverInterface( serverCon );
		
		promptString = "Starting code cracking process.";
		PushStringOntoBuffer( (char *)promptString.c_str() );

		if( serverInterface.getProcessStatus() )
		{
			promptString = "Error: A process is already executing. You cannot start a new one until it finishes.";
			PushStringOntoBuffer( (char *)promptString.c_str() );
		}
		else
		{
			serverInterface.startProcess( processName, email, verificationArchivePath );
		}
	}
	else
	{
		promptString = "Fatal Error: Could not connect to server.";
		PushStringOntoBuffer( (char *)promptString.c_str() );
	}
}

void CrackingTheCodeClient::displayInfo()
{
	string promptString;
	SOCKET serverCon = connectToServer( serverIP, serverPort );

	if( serverCon != NULL )
	{
		CrackProtocolClient serverInterface( serverCon );
		
		promptString = "Retrieving process information.";
		PushStringOntoBuffer( (char *)promptString.c_str() );

		PushStringOntoBuffer( (char *)serverInterface.requestInformation().c_str() );
	}
	else
	{
		promptString = "Fatal Error: Could not connect to server.";
		PushStringOntoBuffer( (char *)promptString.c_str() );
	}
}

void CrackingTheCodeClient::stopCrackingCodeThreads()
{
	PushStringOntoBuffer( "Code cracking stoping For current code." );

	// Send kill flag to verification server. 
	SOCKET verificationServerConn = connectToServer("127.0.0.1",verificationPort);
	SocketCommunicator * verificationCom = new SocketCommunicator( verificationServerConn );

	char killChar[2] = {127,'\0'};

	verificationCom->write( killChar );

	delete verificationCom;

	closesocket( verificationServerConn );

	for(int i = 0; i < codeCrackingExitFlags.size(); i++)
	{
		*codeCrackingExitFlags[i] = true;
	}

	codeCrackingExitFlags.clear();
}
// ## END Use Case Methods ##