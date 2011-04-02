// Permutation.h
// Andrew Ribeiro 
// February 23, 2011

#include <vector> 
#include <iostream>
#include <string>
using namespace std; 

class Permutation 
{
private:
	vector<unsigned char> counters;
	vector<char> * alphabet; 
	unsigned int alphabetSize;
	pair<vector<unsigned char> , vector<unsigned char> > * permutationRange;
	bool haveCompletedRangeFlag; 
	unsigned int countersSize;  


public:
	
	// #Constructors/Deconstructors
	Permutation( vector<char> * alphabet, unsigned int alphabetSize);
	~Permutation();

	// #Essentials
	const Permutation & operator++();
	string getPemutationString();

	// #Utility 
	unsigned int characterPositionInAlphabet( vector< char > * alphabet,  char ch );
	char characterAtPositionInAlphabet( vector< char > *alphabet, unsigned int pos );
	void initCounters( unsigned int value );
	void incrementCounters(unsigned int incrementAmt);
	void clearPermutationRange();

	// #Accessors 
	vector< unsigned char > & getCountersReference() { return counters; }
	vector< char > * getAlphabetReference() { return alphabet; }
	unsigned int size(){ return counters.size(); }
	bool haveCompletedRange(){ return haveCompletedRangeFlag; }

	// #Mutatiors 
	void setRange( pair<vector<unsigned char> , vector<unsigned char> > * permutationRange );

	// #Overloads
	unsigned char & operator[]( unsigned int index );

	// #Friends 
	friend ostream& operator<<(ostream& output, Permutation p);
};