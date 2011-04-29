// SimpleClientSocket.cpp
// Andrew Ribeiro 
// March 4, 2011

#include "CrackingTheCodeClient.h"
#include "CrackProtocolClient.h"
#include "SimpleClientSocket.h"
#include "SimpleSocket.h"
#include "SocketCommunicator.h"
#include "threadArgumentStructures.h"

namespace AndrewRibeiro
{
	SOCKET SimpleClientSocket::connectToServer( string serverIP, string serverPort )
	{
		string initError = init();

		if( initError.size() == 0 )
		{	
			SockAddr.sin_family       = AF_INET;
			SockAddr.sin_addr.s_addr  = inet_addr( serverIP.c_str() );
			SockAddr.sin_port         = htons( atoi(serverPort.c_str()) );

			// Connecting to server 
			int status = connect (hSocket,(LPSOCKADDR) &SockAddr,sizeof(SockAddr));

			if (status == INVALID_SOCKET)
			{
				return NULL;
			}

			return hSocket;
		}
		else
		{
			return NULL;
		}
	}
}