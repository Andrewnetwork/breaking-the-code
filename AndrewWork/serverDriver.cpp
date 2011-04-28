// serverDriver.cpp
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
using namespace std; 

#include "..\CrackProtocolServer\CrackingTheCodeServer.h"

// This function will be implemented in the GUI.
void PushStringOntoBuffer(char *str);

void main()
{
	int choice;

	CrackingTheCodeServer server;

	while( true )
	{
	    PushStringOntoBuffer("Stop or Start the server ( 0, 1 )");

		cin>>choice;

		switch(choice)
		{
		case 0:
			server.stop();
			break;
		case 1:
			server.start();
			break;
		default:
			PushStringOntoBuffer("Invalid choice!");
		}
	}

	system("pause");
}

// Temporary definition of this GUI function.
void PushStringOntoBuffer(char *str)
{
	static HANDLE lock = CreateMutex( NULL, false, NULL ); 

	WaitForSingleObject( lock, INFINITE );

	cout<<"> "<<str<<endl;

	ReleaseMutex( lock );
}