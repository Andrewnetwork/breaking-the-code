// clientDriver.cpp
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

#include <iostream>

#include "..\CrackProtocol\CrackingTheCodeClient.h"

using namespace std;

// This function will be implemented in the GUI.
void PushStringOntoBuffer(char *str);

void main()
{
	int choice; 

	CrackingTheCodeClient test;

	while( true )
	{
	    PushStringOntoBuffer("Start cracking, start process, get info, or stop cracking (1,2,3,4)");

		cin>>choice;

		switch(choice)
		{
		case 1:
			test.crackCode();
			break;
		case 2:
			test.startProcess("Andrew's hash","andrewnetwork@gmail.com","t.zip");
			break;
		case 3:
			test.displayInfo();
			break;
		case 4:
			//test.stopCrackingCode();
			break;
		default:
			PushStringOntoBuffer("Invalid choice!");
		}
	}
}

// Temporary definition of this GUI function.
void PushStringOntoBuffer(char *str)
{
	static HANDLE lock = CreateMutex( NULL, false, NULL ); 

	WaitForSingleObject( lock, INFINITE );

	cout<<"> "<<str<<endl;

	ReleaseMutex( lock );
}