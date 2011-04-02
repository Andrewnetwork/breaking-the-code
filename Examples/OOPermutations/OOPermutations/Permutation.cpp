// Permutation.cpp
// Andrew Ribeiro 
// February 23, 2011

#include <vector> 
#include <string>
#include "Permutation.h"
using namespace std; 

void printCounterVector( vector<unsigned char> vect , unsigned char base);

// #Constructors/Deconstructor
Permutation::Permutation( vector<char> * alphabet, unsigned int alphabetSize)
{
	this->alphabet = alphabet; 
	this->alphabetSize = alphabetSize;
	this->haveCompletedRangeFlag = false;
	this->permutationRange = NULL;

}
Permutation::~Permutation()
{
	
}

// #Essentials
const Permutation & Permutation::operator++()
{

	// Incrementing
	for(int digitIndex = this->countersSize - 1; digitIndex >= 0; digitIndex--)
	{
		if(counters[digitIndex] + 1 >= alphabetSize )
		{
			counters[digitIndex] = 0;
		}
		else
		{
			counters[digitIndex] += 1; 
			digitIndex = -1; // Done
		}
	}

	//Range check
	if( permutationRange != NULL )
	{
		bool haveCompletedRange = true; 
		bool counterOverflowed = true;

		// Working on a range. 
		for( int i = this->countersSize - 1; i >= 0; i--)
		{
			if( counters[i] != permutationRange->second[i] )
			{
				haveCompletedRange = false;
			}
		
			if( counters[i] != 0 )
			{
				
				counterOverflowed = false;
			}
		}

		if(  counterOverflowed || haveCompletedRange  )
		{
			if( this->countersSize < permutationRange->second.size() )
			{
				counters.resize( counters.size() + 1, 0 );
				this->countersSize++;
			}
			else
			{
				this->haveCompletedRangeFlag = true; 
			}
		}
	}

	return *this;
}

string Permutation::getPemutationString()
{
	string outStr = "";

	for(int i = 0; i < this->countersSize; i++ )
	{
		outStr+=characterAtPositionInAlphabet( alphabet, getCountersReference()[i]  );
	}

	return outStr;
}

// #Utility 
unsigned int Permutation::characterPositionInAlphabet( vector< char > * alphabet,  char ch )
{
	unsigned int position = 0;

	for( unsigned int i = 0; i < alphabetSize; i = i + 2 )
	{
		for( unsigned int charOnRange = (*alphabet)[i]; charOnRange <= (*alphabet)[i+1]; charOnRange++ )
		{
			if( charOnRange == ch )
			{
				return position; 
			}

			position++;
		}
	}
}

char Permutation::characterAtPositionInAlphabet( vector< char > *alphabet, unsigned int pos )
{
	unsigned int position = 0;

	for( unsigned int i = 0; i < alphabetSize; i = i + 2 )
	{
		for( unsigned int charOnRange = (*alphabet)[i]; charOnRange <= (*alphabet)[i+1]; charOnRange++ )
		{
			if( position == pos )
			{
				return charOnRange; 
			}

			position++;
		}
	}
}

void Permutation::initCounters( unsigned int value )
{
	for( int i = 0; i < this->countersSize; i++)
	{
		counters[i] = value; 
	}
}

void Permutation::incrementCounters(unsigned int incrementAmt)
{
	if( incrementAmt != 0 )
	{
		unsigned long int remainder = incrementAmt; 

		int digitCount = 1;

		do
		{
			counters[this->countersSize - digitCount ] = remainder % alphabetSize; 

			remainder = remainder / alphabetSize;

			digitCount++;

		}while(remainder > 0);



	}

}

void Permutation::clearPermutationRange() 
{ 
	permutationRange = NULL; 
	haveCompletedRangeFlag = false; 
}

// #Mutatiors
void Permutation::setRange( pair<vector<unsigned char> , vector<unsigned char> > * permutationRange )
{
	this->countersSize = permutationRange->first.size();
	this->counters.resize( this->countersSize );

	this->permutationRange = permutationRange;
	this->counters = permutationRange->first;
	haveCompletedRangeFlag = false; 
}

// #Overloads

unsigned char & Permutation::operator[]( unsigned int index )
{
	return counters[index]; 
}

// #Friends 
ostream& operator<<(ostream& output, Permutation p) 
{
	output<<p.getPemutationString();
	
	return output;
}