// CrackingTheCodeServer.h
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

#pragma once 

#include <Windows.h>
#include <string> 
#include <map>

#include "..\GlobalHeaders\SimpleSocketsLibrary\SimpleServerSocket.h"
#include "CodeCrackingState.h"

using namespace AndrewRibeiro; 
using namespace std; 

class CrackingTheCodeServer 
{
public:
	void start();
	void stop();
	CrackingTheCodeServer();

private:

	// # Server configurations #
	unsigned long int  permutationInt;
	string             port; 
	unsigned int       alphabetSize; 

	map<string,string> getConfigurationVariables();
	friend DWORD WINAPI listenForClientsClients( LPVOID arg );
	SimpleServerSocket serverSocket;
	CodeCrackingState * serverState;
	
	bool isRunning;
};