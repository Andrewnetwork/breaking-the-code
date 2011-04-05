// server.cpp
// Andrew Ribeiro 
// Andrew@AndrewRibeiro.com
// March 5, 2011

#include <iostream>
#include <sstream>
#include <string>
#include <Windows.h>

// Headers from SimpleSocketsLibrary
#include "SimpleServerSocket.h"
#include "SocketCommunicator.h"

#include "HeftyCounter.h"


// The namespace for the SimpleSocketsLibrary
using namespace AndrewRibeiro;

using namespace std;

// Function Prototypes
unsigned int alphabetCount( vector<char> * alphabet );
void printCounterVector( vector<unsigned char> vect , unsigned char base);


HeftyCounter * GLOBAL_COUNTER;

// Constants 
const int RANGE_SIZE = 1000000;


DWORD WINAPI handleClient(  LPVOID arg )
{
	cout<<"NEW THREAD"<<endl;
	SOCKET clientConnection = ( SOCKET ) arg;
	SocketCommunicator *  clientCom         = NULL; 

	// # Create a socket communicator, so that we may communicate with the server using 'serverConnection'
	clientCom = new SocketCommunicator( clientConnection );

	// # Start conversation loop.

	bool    clientCommunicationError = false;

	do
	{
		string clientResult, clientOut;
	
		clientResult = clientCom->read();

		if( clientResult.length() > 0 )
		{
			if( clientResult.compare("GET") == 0)
			{
				// Client wishes to get a permutation range.
				pair<vector<unsigned char>, vector<unsigned char>> range = GLOBAL_COUNTER->getRange( RANGE_SIZE );

				string lowerBound="", upperBound="";

				// ** Need to add one to avoid 0. Must decrement on client.
				for( int i = 0; i < range.first.size(); i++)
				{
					lowerBound += 1+(int)range.first[i];
				}

				for( int i = 0; i < range.second.size(); i++)
				{
					upperBound += 1+(int)range.second[i];
				}

			
				clientOut = lowerBound + "~" +upperBound;


				printCounterVector( range.first , 62);
				printCounterVector( range.second , 62);
				
				
			}
			else
			{
				clientOut = "";
			}

			cout<<"Size: "<<strlen(clientOut.c_str())<<endl;

			clientCom->write( clientOut );
		}
		else
		{
			cout<<"ERROR: Communication ended abruptly with client."<<endl;
			clientCommunicationError  = true;
		}

	}while( !clientCommunicationError ); 

	return 1;
}

void main()
{
	vector<char> alpha;
	alpha.push_back( 'A' );
	alpha.push_back( 'Z' );
	alpha.push_back( 'a' );
	alpha.push_back( 'z' );
	alpha.push_back( '0' );
	alpha.push_back( '9' );

	const unsigned int alphaSize = alphabetCount(&alpha);

	GLOBAL_COUNTER = new HeftyCounter(1, alphaSize);

	SimpleServerSocket    serverSock;
	

	cout<<"---------------------------------- Server ------------------------------"<<endl;
	cout<<"This server responds to a client asking for the i'th prime number       "<<endl;
	cout<<"------------------------------------------------------------------------"<<endl<<endl;


	bool    clientConnectionError = false;
	string  serverPort;



	cout<<"---------- Server Setup -----------"<<endl;
	cout<<"Enter server port: ";
	getline( cin, serverPort, '\n');

	
	cout<<"Waiting for incoming connections"<<endl;

	serverSock.bindToPort( serverPort );

	// Connect to client, and store socket/connection in 'clientConnection'
	do
	{
		// # Connecting the the server 'serverIP', on port 'serverPort'
		SOCKET                clientConnection  = serverSock.listenForConnection();


		if( clientConnection == NULL )
		{
			// Unable to connect.
			clientConnectionError = true;
			cout<<"Could not connect to client on port '"<<serverPort<<"'."<<endl;
		}
		else
		{
			// Connected to the server. 
			clientConnectionError = false; 

			CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size  
            handleClient,       // thread function name
            (void * )clientConnection,          // argument to thread function 
            0,                      // use default creation flags 
            NULL);   // returns the thread identifier 


			cout<<"Server connected to client on port '"<<serverPort<<"'."<<endl; 

		}


	}while( !clientConnectionError );

}
