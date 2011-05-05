#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include "SQL_XML.h"

using namespace std;

void main()
{
	const string host 	 = "173.201.88.97";			// Server Host
const string user	 = "breakingthecode";	    // User name
const string passwrd = "Csisfun1234";			// Password
const string db		 = "breakingthecode";		// Name of the database
const int port 		 = 3306;					// Port number

	SQL_XML run ( host, user, passwrd, db , port );


	/*run.getStartingInfo( "Jay Shah", "Yahoo Mail");
	cout <<" insert is done " <<endl;
	string pass = run.passXML();
	cout << " XML form of resulting Query : \n\n" << pass << endl;

	system("pause");

	cout << " Parse of XML is \n"<< run.parseXML(pass);
	system("pause");

	cout << endl << endl;
	run.getEndingInfo("SDwerw");
	string pass2 = run.passXML();
	cout << " XML form of resulting Query : \n\n" << pass2 << endl;
*/
	string t_name, t_email,t_code;
	vector < string > name1;
	vector < string > email1;
	vector < string > code1;

	for ( int i = 0; i < 3 ; i++ )
	{
		cout << "\n Enter User name :\t" ;
		cin >> t_name;
		name1.push_back(t_name);
		cout << "\n Enter Email : \t";
		cin >> t_email;
		email1.push_back(t_email);
	}

	for (int i = 0; i < name1.size(); i++)
	{
		run.getStartingInfo( name1[i],email1[i]);
		//run.getEndingInfo ( code1[i] );
	}

	cout << "\n Enter code: \t";
	cin >> t_code;
	//code1.push_back(t_code);

	run.getEndingInfo ( t_code );

	cout << " Insert is Done . ! \n\n"<<endl;
	system("pause");

	string pass = run.passXML();
	cout << pass;
	cout << endl;
	system("pause");

	cout << run.parseXML(pass);
	system("pause");
}