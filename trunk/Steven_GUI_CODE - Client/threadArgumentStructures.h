// threadArgumentStructures.h
// Andrew Ribeiro 
// http://www.AndrewRibeiro.com
// April 17, 2011
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
#include <queue>

using namespace std;

#include "CrackProtocolClient.h"

struct CodeCrackingThreadArgs
{
	string serverIP;
	string serverPort;
	bool*  exitFlag;
	string verificationPort;
	string alphabet;
	UINT   alphaSize;

	CodeCrackingThreadArgs( string ip, string port, bool* eFlag, string vPort, string alpha, UINT alphSize)
	{
		serverIP            = ip;
		serverPort          = port;
		exitFlag            = eFlag;
		verificationPort    = vPort;
		alphabet            = alpha;
		alphaSize           = alphSize;
	}
};
