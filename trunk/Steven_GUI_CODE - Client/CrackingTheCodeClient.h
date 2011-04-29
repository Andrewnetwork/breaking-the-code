// CrackingTheCodeClient.h
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

#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

#include "threadArgumentStructures.h"

class CrackingTheCodeClient
{
public:
	CrackingTheCodeClient();

	// ## Use Case Methods ##
	void crackCode();
	void displayInfo();
	void startProcess(string processName, string email, string verificationArchivePath);

private:
	// ## Configuration Variables ##
	string serverPort; 
	string serverIP;
	string verificationPort;
	string alphabet; 
	UINT   alphabetSize;
	string numberOfThreads;

	// ## Threading Flags ##
	vector< bool* > codeCrackingExitFlags;

	// ## Threading Methods ##
	HANDLE              startNewCodeCrackingThread( bool * exitFlag );
	friend DWORD WINAPI crackCodeThread( LPVOID arg );
	friend DWORD WINAPI processStopCheck( LPVOID arg );
	void                stopCrackingCodeThreads();

	// ## Utility Methods ##
	bool startVerificationServer();
};