#include <vector> 
#include <iostream>
using namespace std; 

const char RANGE_DILIM = '~';

unsigned int alphabetCount( vector<char> * alphabet )
{
	unsigned int nAlphaLexemes = 0;

	for( int i = 0; i < alphabet->size(); i = i + 2 )
	{
		nAlphaLexemes += ( (*alphabet)[i+1] + 1 ) - (*alphabet)[i];
	}

	return nAlphaLexemes;
}

void printCounterVector( vector<unsigned char> vect , unsigned char base)
{
	unsigned long long int numInBase10 = 0; 
	for(int i = 0; i < vect.size(); i++)
	{
		cout<<" | "<<(int)vect[i];
		numInBase10 += (int)vect[i] * pow((double)base ,(double)(vect.size() - ( i + 1 )) );
	}
	 
	cout<<" |       "<<numInBase10<<endl;
}

pair<vector<unsigned char>,vector<unsigned char> > getRangeFromString( string range )
{
	pair<vector<unsigned char>,vector<unsigned char>> rangePair; 
	vector<unsigned char> lowerBound;
	vector<unsigned char> upperBound;


	int i;

	for(i = 0; i < range.size() && range[i] != RANGE_DILIM; i++)
	{
		lowerBound.push_back( range[i] -1 );
	}

	for(i = i+1; i < range.size(); i++)
	{
		upperBound.push_back(  range[i] -1);
	}

	rangePair.first = lowerBound;
	rangePair.second = upperBound;

	return rangePair;
}