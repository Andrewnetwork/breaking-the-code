#include <vector> 
#include <iostream>
using namespace std; 

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