// SimpleSocket.h
// Andrew Ribeiro 
// Andrew@AndrewRibeiro.com
// March 4, 2011
/*******************************************************
** This code was created by Andrew Ribeiro whom        *
** retains the rights to all the material presented in *
** this document, unless stated otherwise. The usage   *
** of this code is granded with no liability to the    *
** creator of this material.                           *
********************************************************/

#pragma once

#include <windows.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <winsock.h>

using namespace std; 

namespace AndrewRibeiro
{
	const class SimpleSocket
	{
	protected:
		WSADATA       wsaData;
		SOCKET        hSocket;
		SOCKADDR_IN   SockAddr;
		bool initilized; 

	public:

		string init();

	};
}