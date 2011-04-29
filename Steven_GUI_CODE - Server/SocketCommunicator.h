// SocketCommunicator.h
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
	const unsigned int MAX_DIGIT_MESSAGE_SIZE = 50;
	const string NO_MESSAGE_RECEIVED = "";

	class SocketCommunicator
	{
	private:

		SOCKET comStream; 

	public:

		SocketCommunicator( SOCKET sock );
	
		bool write( string message );

		string read();

	};
}