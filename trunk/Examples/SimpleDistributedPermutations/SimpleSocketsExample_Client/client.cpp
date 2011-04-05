// client.cpp
// Andrew Ribeiro 
// Andrew@AndrewRibeiro.com
// March 5, 2011

#include <iostream>
#include <vector>

// Headers from SimpleSocketsLibrary
#include "SimpleClientSocket.h"
#include "SocketCommunicator.h"

// The namespace for the SimpleSocketsLibrary
using namespace AndrewRibeiro;


#include "Permutation.h"

using namespace std;




pair<vector<unsigned char>,vector<unsigned char> > getRangeFromString( string range );
void printCounterVector( vector<unsigned char> vect , unsigned char base);
unsigned int alphabetCount( vector<char> * alphabet );

void main()
{
	SimpleClientSocket    clientSock;
	SocketCommunicator *  serverCom         = NULL; 
	SOCKET                serverConnection  = NULL; 

	cout<<"---------------------------------- Client ------------------------------"<<endl;
	cout<<"Enter an integer, i, and the server will tell you the i'th prime number."<<endl;
	cout<<"------------------------------------------------------------------------"<<endl<<endl;

	string  serverIP; 
	string  serverPort; 
	bool    serverConnectionError = false;

	vector<char> alpha;
	alpha.push_back( 'A' );
	alpha.push_back( 'Z' );
	alpha.push_back( 'a' );
	alpha.push_back( 'z' );
	alpha.push_back( '0' );
	alpha.push_back( '9' );

	const unsigned int alphaSize = alphabetCount(&alpha);

	// Connect to server, and store socket/connection in 'serverConnection'
	do
	{
		cout<<"---------- Server Connection -----------"<<endl;
		cout<<"Enter server ip ( 0 for local host ): ";
		getline( cin, serverIP, '\n');

		if( serverIP == "0" )
		{
			serverIP = "127.0.0.1";
		}

		cout<<"Enter server port: ";
		getline( cin, serverPort, '\n');

		// # Connecting the the server 'serverIP', on port 'serverPort'
		serverConnection = clientSock.connectToServer(serverIP,serverPort);

		if( serverConnection == NULL )
		{
			// Unable to connect.
			serverConnectionError = true;
		}
		else
		{
			// Connected to the server. 
			serverConnectionError = false; 
		}

	}while( serverConnectionError );

	cout<<"Connected to server '"<<serverIP<<"' on port '"<<serverPort<<"'."<<endl;
	
	// # Create a socket communicator, so that we may communicate with the server using 'serverConnection'
	serverCom = new SocketCommunicator( serverConnection );


	// # Start conversation loop.

	bool    serverCommunicationError = false;

	do
	{
		string input, serverResult;

		serverCom->write( "GET" );

		serverResult = serverCom->read();

		if( serverResult.length() > 0 )
		{
			pair<vector<unsigned char>,vector<unsigned char> > range = getRangeFromString( serverResult );
			
			Permutation perm(&alpha, alphaSize );
			perm.setRange( &range );
			
			while( !perm.haveCompletedRange() )
			{
			
				// Print the permutation. 
				cout<<perm<<endl;
				
				// Increment permutation. 
				++perm;
			
			}

			cout<<endl;
			
		}
		else
		{
			cout<<"ERROR: Communication ended abruptly with server."<<endl;
			serverCommunicationError = true;
		}

	}while( !serverCommunicationError ); 


	system("pause");
}

