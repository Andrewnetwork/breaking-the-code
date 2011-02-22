// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.


/****************************************************************************
						Microsoft RPC
          
                        Hello Example

    FILE:       hellop.c

    PURPOSE:    Remote procedures that are linked with the server
                side of RPC distributed application

    FUNCTIONS:  HelloProc() - prints "hello, world" or other string
                sent by client to server

    COMMENTS:   This version of the distributed application that prints
                "hello, world" (or other string) on the server features
                a client that manages its connection to the server.
                It uses the binding handle hello_IfHandle, defined in
                the file hello.h.

****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "hello.h"    // header file generated by MIDL compiler
#include <iostream>
#include <string>
#include <vector>
using namespace std;

const unsigned long int permutationAssignmentCount = 40000;
const unsigned int hashDecodedSize = 20;
extern char hashToDecode[33]; 


void GetPermutationRange(IN RPC_BINDING_HANDLE hBinding, unsigned long start[1], unsigned long end[1])
{
	static unsigned long int currentPermutation = 0; 

	start[0] = currentPermutation; 
	currentPermutation += permutationAssignmentCount;
	end[0] = currentPermutation; 	
}

void GetHash( IN RPC_BINDING_HANDLE hBinding, unsigned char breakHash[33] )
{
	//strcpy( (char * )breakHash, hashToDecode );
	strncpy( (char * )breakHash, hashToDecode, 32 );
}

void HashDecoded( IN RPC_BINDING_HANDLE hBinding, unsigned char decodedHash[hashDecodedSize] )
{
	cout<<"The hash has been decoded!"<<endl
		<<"Decoded hash: "<<decodedHash<<endl;
}


