// CodeCrackingState.cpp
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

#include <vector>
#include <iostream>
#include <string>
#include <Windows.h>

#include "HeftyCounter.h"
#include "CodeCrackingState.h"

using namespace std;

const int CodeCrackingState::DEFAULT_PERM_INTERVAL = 1000000;

CodeCrackingState::CodeCrackingState( unsigned int alphaSize )
{
	this->permutationCounter = NULL;
	this->verificationArchive = "";
	this->stateMutex = CreateMutex(NULL,false, NULL);
	this->clients = 0;
	this->alphaSize = alphaSize; 

	newCounter();
}

// # Accessors #
string CodeCrackingState::getVerificationArchive()
{
	return this->verificationArchive;
}

unsigned int CodeCrackingState::getNClients()
{
	return this->clients;
}

bool CodeCrackingState::processStatus()
{
	this->aquire();

	bool processStatus = ( verificationArchive.size() > 0 );

	this->release();
	
	return processStatus;
}

bool CodeCrackingState::incompleteRangesExist()
{
	if( this->incompleteAssignedRanges.size() > 0 )
	{
		return true; 
	}
	else
	{
		return false;
	}
}

// # Mutators #
void CodeCrackingState::newCounter()
{
	if( this->permutationCounter != NULL )
	{
		delete this->permutationCounter;
	}

	this->permutationCounter = new HeftyCounter(1, alphaSize);
}

void CodeCrackingState::clearVerificationArchive()
{
	this->verificationArchive = "";
}

void CodeCrackingState::setVerificationArchive( string binArchive)
{
	this->aquire();

	this->verificationArchive = binArchive;

	this->release();
}

void CodeCrackingState::addClient()
{
	this->aquire();

	this->clients++;

	this->release();
}

void CodeCrackingState::removeClient()
{
	this->aquire();

	this->clients--;

	this->release();
}
	
void CodeCrackingState::addIncompletedRange( pair< vector<unsigned char>, vector<unsigned char> > range )
{
	this->aquire();

	this->incompleteAssignedRanges.push_back( range );

	this->release();
}

// # Accessor/Mutator #
pair< vector<unsigned char>, vector<unsigned char> > CodeCrackingState::getRange( unsigned long int interval )
{
	this->aquire();

	pair< vector<unsigned char>, vector<unsigned char> > range;

	if( incompleteRangesExist() )
	{
		range = this->incompleteAssignedRanges[0];

		this->incompleteAssignedRanges.erase( this->incompleteAssignedRanges.begin() );
	}
	else
	{
		range = this->permutationCounter->getRange( interval );
	}

	this->release();

	return range;
}

// Default interval of 100,000
pair< vector<unsigned char>, vector<unsigned char> > CodeCrackingState::getRange()
{
	return getRange( DEFAULT_PERM_INTERVAL );
}


// # Semaphore Methods #
bool CodeCrackingState::aquire()
{
	WaitForSingleObject( this->stateMutex , INFINITE );

	return true;
}

bool CodeCrackingState::release()
{
	ReleaseMutex( this->stateMutex );

	return true;
}
