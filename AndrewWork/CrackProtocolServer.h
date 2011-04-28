// CrackProtocolServer.h
// Andrew Ribeiro 
// http://www.AndrewRibeiro.com
// April 10, 2011
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
#include <vector>

#include "..\GlobalHeaders\SimpleSocketsLibrary\SocketCommunicator.h"
#include "..\GlobalHeaders\CrackProtocolConstants.h"

#include "CodeCrackingState.h"


using namespace AndrewRibeiro; 
using namespace std; 


class CrackProtocolServer
{
public:

	CrackProtocolServer( SOCKET activeConnection , CodeCrackingState * globalState, long unsigned int permutationInt);

	bool processRequests();

private:
	SOCKET                connection;
	SocketCommunicator *  connectionCom;
	CodeCrackingState *   gobalState;
	long unsigned int     permutationInt;
	bool                  rangeAssignedInProgress;

	pair<vector<unsigned char>, vector<unsigned char> > rangeAssigned;

	string getFileContents( string filePath );

	bool makeDirectory( string name, string bin );


	// Protocol Functions 

	bool recvRangeComplete(); 

	bool sendProcessStatus();

	void startProcess();

	void sendRange();

	void requestInformation();

	void sendVerificationArchive();

	void sendInvalidOperationMessage(string errorMessage);

	// ## Database Related Methods ##
	bool addProcessRecordToDB( string name, string email );
	//void requestInformation();
};