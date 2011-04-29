// CrackProtocolClient.h
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

#include "SocketCommunicator.h"
#include "CrackProtocolConstants.h"

using namespace AndrewRibeiro; 
using namespace std; 

typedef pair<string,string > RangeStrings;

class CrackProtocolClient
{
public:
	CrackProtocolClient( SOCKET activeConnection );

	// ## Protocol Functions ## 
	bool         getProcessStatus();
	bool         startProcess( string name, string email, string verificationArchPath );
	RangeStrings getRange();
	string       requestInformation();
	string       getVerificationArchive();
	void         invalidOperation(string errorMessage);
	void         rangeComplete( string crackedCode );

private:
	SOCKET               connection;
	SocketCommunicator * connectionCom;

	// # Utility Functions #
	string getFileContents( string filePath );
	bool   makeDirectory( string name, string bin );
	bool   unzipFile( string name );
};