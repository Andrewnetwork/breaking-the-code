// CodeCrackingState.h
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

#include <vector>
#include <string>

#include "HeftyCounter.h"

using namespace std;

class CodeCrackingState
{
public:

	CodeCrackingState( unsigned int alphaSize );

	// Accessors
	string getVerificationArchive(); 
	unsigned int getNClients();
	bool processStatus();
	bool incompleteRangesExist();
	

	// Mutators
	void setVerificationArchive( string binArchive);
	void addClient();
	void removeClient();
	void clearVerificationArchive();
	void newCounter();

	// Accessor/Mutator 
	pair< vector<unsigned char>, vector<unsigned char> > getRange( unsigned long int interval ); 

	// Uses the default interval
	pair< vector<unsigned char>, vector<unsigned char> > getRange(); 

	void addIncompletedRange( pair< vector<unsigned char>, vector<unsigned char> > range );

	// Semaphore Methods
	bool aquire();
	bool release();
	bool wait();


private:
	unsigned int alphaSize;

	unsigned int clients; 

	string verificationArchive; 

	HeftyCounter * permutationCounter;

	HANDLE stateMutex;

	vector< pair< vector<unsigned char>, vector<unsigned char> > > incompleteAssignedRanges;

	static const int DEFAULT_PERM_INTERVAL;
};