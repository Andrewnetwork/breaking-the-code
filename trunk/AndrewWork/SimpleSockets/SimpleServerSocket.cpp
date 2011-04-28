// SimpleServerSocket.cpp
// Andrew Ribeiro 
// March 4, 2011

#include "Stdafx.h"

namespace AndrewRibeiro
{
	bool  SimpleServerSocket::bindToPort( string port )
	{
		string initError = init();

		if( initError.size() == 0 )
		{
			SOCKADDR_IN  serverSockAddr;

			memset(&serverSockAddr, 0, sizeof(serverSockAddr));
			serverSockAddr.sin_family       = AF_INET;
			serverSockAddr.sin_addr.s_addr  = htonl(INADDR_ANY);
			serverSockAddr.sin_port         = htons( atoi(port.c_str()) );

			// Binding socket to port. 
			int status = bind ( hSocket, (LPSOCKADDR) &serverSockAddr, sizeof(serverSockAddr));

			
			if (status == INVALID_SOCKET)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}

	SOCKET SimpleServerSocket::listenForConnection()
	{
		// Listen for incoming connections 
		int status = listen ( hSocket, 1 );

		if (status == SOCKET_ERROR) 
		{ 
			return NULL;
		}

		// Accept connection received 

		SOCKET       clientSocket;
		SOCKADDR_IN  clientSockAddr;
		int addressLength = sizeof(SOCKADDR_IN);

		clientSocket = accept ( hSocket, (LPSOCKADDR) &clientSockAddr, &addressLength );

		if (clientSocket == INVALID_SOCKET) 
		{ 
			return NULL; 
		}
		else
		{
			return clientSocket; 
		}
			
	}

	void SimpleServerSocket::unbind()
	{
		closesocket( hSocket );
	}
}
