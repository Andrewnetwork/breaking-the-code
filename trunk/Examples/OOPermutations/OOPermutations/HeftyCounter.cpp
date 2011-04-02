// HeftyCounter.cpp
// Andrew Ribeiro 
// February 23, 2011

#include <vector>
#include "HeftyCounter.h"
#include <iostream>
#include <string>
using namespace std;

// #Constructors
HeftyCounter::HeftyCounter( unsigned int length , unsigned char base)
{
	this->counter.resize( length );
	this->base = base; 
	this->counterSize = length;
}

// #Essential Functions 
pair< vector<unsigned char>, vector<unsigned char> > HeftyCounter::getRange(unsigned long int interval)
{
	vector<unsigned char> previousCounterState = counter; 
	incrementCounter(interval);

	bool currentCounterOverflowed = true; 

	return pair< vector<unsigned char>, vector<unsigned char> > (previousCounterState, counter );
	
}

pair< string, string > HeftyCounter::getRange(unsigned long int interval, bool getIntRep )
{ 
	string startInterval = "", endInterval = "";
	
	pair< vector<unsigned char>, vector<unsigned char> > range = this->getRange(interval );

	if( getIntRep )
	{
		// Get integer representation. 
		// Does not work, need to use stringstreams. 
		for( int i = 0; i < range.first.size(); i++)
		{
			startInterval+= " | " + (int)range.first[i];
			endInterval += " | "+ (int)range.second[i];
		}
	}
	else
	{
		// Get character represnentation. 
		for( int i = 0; i < range.first.size(); i++)
		{
			startInterval+= range.first[i];
			endInterval += range.second[i];
		}
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
		unsigned long int remainder = quantity; 
		unsigned long long int additionRemainder = 0;
		unsigned char modTmp = 0;
		unsigned int counterIndex = 0;
		unsigned additionTemp = 0;

		int digitCount = 1;

		// Convert quantity to the counters base and add it to the counter at the same time.
		do
		{
			counterIndex = this->counterSize - digitCount;

			additionTemp = ( counter[ counterIndex  ] + (remainder % base ) + additionRemainder);

			modTmp = additionTemp % base;

			additionRemainder = additionTemp / base; 

			counter[ counterIndex] = modTmp;

			remainder = remainder / base;

			if( (additionRemainder > 0 || remainder > 0) && digitCount == this->counterSize )
			{
				// Overflow.
				vector<unsigned char> tmp;
				tmp.push_back( 0 );

				this->counterSize++;
				for( int i = 0; i < counter.size(); i++)
				{
					tmp.push_back( counter[i] );
				}

				counter = tmp;
			}

			digitCount++;

		}while( remainder > 0  || additionRemainder > 0  );

	}
}

