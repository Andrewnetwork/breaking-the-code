#include "Permutation.h"
#include "HeftyCounter.h"
#include <iostream>
using namespace std;

unsigned int alphabetCount( vector<char> * alphabet );
void printCounterVector( vector<unsigned char> vect , unsigned char base);

void main()
{

	// Setting up the alphabet. 
	// ACII Ranges of characters. Works in pairs. { lowerBound, upperBound }. 
	vector<char> alpha;
	alpha.push_back( 'A' );
	alpha.push_back( 'Z' );
	alpha.push_back( 'a' );
	alpha.push_back( 'z' );
	alpha.push_back( '0' );
	alpha.push_back( '9' );

	const unsigned int alphaSize = alphabetCount(&alpha);

	HeftyCounter c1(1, alphaSize);

	while( true )
	{
	
		pair<vector<unsigned char>, vector<unsigned char>> range = c1.getRange( 10 );
			
		cout<<"--------------- Generating permutation on range ------------------"<<endl;
		printCounterVector( range.first , 62);
		printCounterVector( range.second, 62 );
		cout<<"------------------------------------------------------------------"<<endl;
		system("pause");

		Permutation perm(&alpha, alphaSize );
		perm.setRange( &range );
			
		while( !perm.haveCompletedRange() )
		{
			
			// Print the permutation. 
			cout<<perm<<endl;
				
			// Increment permutation. 
			++perm;
			
		}

	
	}

	system("pause");
}