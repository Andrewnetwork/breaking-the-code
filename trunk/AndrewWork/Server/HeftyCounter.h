// HeftyCounter.h
// Andrew Ribeiro 
// http://www.AndrewRibeiro.com
// April 23, 2011
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
using namespace std;

class HeftyCounter
{
private:
	vector< unsigned char > counter;
	unsigned char base; 
	unsigned int counterSize;

public:
	// #Constructors
	HeftyCounter( unsigned int length , unsigned char base); 

	// #Essential Functions 
	pair< vector<unsigned char>, vector<unsigned char> > getRange(unsigned long int interval);
	pair< string, string > getRangeStrings(unsigned long int interval );

	// #Overloaded Operators 
	const HeftyCounter & operator++();

	// #Utility 
	void incrementCounter( unsigned long int quanity );

};