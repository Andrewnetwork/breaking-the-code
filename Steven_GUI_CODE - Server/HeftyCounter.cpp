// HeftyCounter.cpp
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

#include <vector>
#include "HeftyCounter.h"
#include <iostream>
#include <string>
using namespace std;

// #Constructors
HeftyCounter::HeftyCounter( unsigned int length , unsigned char base)
{
	this->base = base; 
	this->counterSize = length;
	this->counter.resize( length, 0 );
}

// #Essential Functions 
pair< vector<unsigned char>, vector<unsigned char> > HeftyCounter::getRange(unsigned long int interval)
{
	vector<unsigned char> previousCounterState = counter; 
	incrementCounter(interval);

	bool currentCounterOverflowed = true; 

	return pair< vector<unsigned char>, vector<unsigned char> > (previousCounterState, counter );
	
}

pair< string, string > HeftyCounter::getRangeStrings(unsigned long int interval )
{ 
	string startInterval = "", endInterval = "";
	
	pair< vector<unsigned char>, vector<unsigned char> > range = this->getRange(interval );

	int i; 

	startInterval.resize( range.first.size(), ' ');
	endInterval.resize( range.second.size(), ' ');

	for(i = 0; i < range.first.size(); i++)
	{
		startInterval[i] = range.first[i];
		endInterval[i] = range.second[i];
	}

	for( ; i < range.second.size(); i++ )
	{
		endInterval[i] = range.second[i];
	}

	return pair<string , string>( startInterval, endInterval );
}

// #Overloaded Operators 
const HeftyCounter & HeftyCounter::operator++()
{
	incrementCounter( 1 );

	return *this;

}


// #Utility 
void HeftyCounter::incrementCounter( unsigned long int quantity)
{
	if(  quantity  != 0 )
	{
		unsigned long int        remainder         = quantity; 
		unsigned long long int   additionRemainder = 0;
		unsigned int             counterIndex      = 0;
		unsigned                 additionTemp      = 0;
		int                      digitCount        = 1;


		// Convert quantity to the counters base and add it to the counter at the same time.
		do
		{
			counterIndex = this->counterSize - digitCount;

			additionTemp = ( counter[ counterIndex  ] + (remainder % base ) + additionRemainder);

			counter[ counterIndex ] = additionTemp % base;
			
			additionRemainder = additionTemp / base; 

			remainder = remainder / base;

			if( (additionRemainder > 0 || remainder > 0) && digitCount >= this->counterSize )
			{
				// Overflow.
				counter.insert( counter.begin(), 0 );
				++this->counterSize;
			}
		
			digitCount++;

		}while( remainder > 0  || additionRemainder > 0  );
	
	}
}

