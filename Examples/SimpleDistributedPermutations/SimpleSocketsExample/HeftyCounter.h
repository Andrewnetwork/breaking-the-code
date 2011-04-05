// HeftyCounter.h
// Andrew Ribeiro 
// February 23, 2011

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
	pair< string, string > getRange(unsigned long int interval, bool getIntRep );

	// #Overloaded Operators 
	const HeftyCounter & operator++();


	// #Utility 
	void incrementCounter( unsigned long int quanity );

};