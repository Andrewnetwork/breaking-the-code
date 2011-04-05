// SimpleServerSocket.h
// Andrew Ribeiro 
// March 4, 2011
// Sources used: TCP_Server_Mul by Marshall Brain modified extensively by RMJ

#pragma once

#include <windows.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <winsock.h>
#include "SimpleSocket.h"

using namespace std; 

namespace AndrewRibeiro
{
	class SimpleServerSocket : public SimpleSocket
	{

	public:
		SOCKET listenForConnection( );
		bool bindToPort( string port );

	};
}