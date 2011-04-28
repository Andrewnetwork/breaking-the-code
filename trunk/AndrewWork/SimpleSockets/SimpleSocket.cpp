// SimpleSocket.cpp
// Andrew Ribeiro 
// March 4, 2011

#include "Stdafx.h"

namespace AndrewRibeiro
{
	string SimpleSocket::init()
	{
		string errorMessage = "";

		int status = WSAStartup(MAKEWORD(1, 1), &wsaData);

		if (status != 0) 
		{
			errorMessage = "ERROR in WSAStartup\n"; 
			return errorMessage; 
		}

		hSocket = socket (AF_INET, SOCK_STREAM, 0);

		if (hSocket == INVALID_SOCKET)
		{ 
			errorMessage = "ERROR in socket create()\n";
			WSACleanup(); 
			return errorMessage; 
		}

		initilized = true;
		return errorMessage;
	}
}