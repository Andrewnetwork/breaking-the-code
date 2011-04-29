#include <iostream>
#include <WinSock.h>
#include <string>
#include <mysql.h>
#include <vector>
#include <time.h>
#include <cstring>
#include <sstream>
#include "xmlwriter.h"
#include "SQL_XML.h"

using namespace std;
using namespace xmlw;

void main()
{
	const string host 	 = "173.201.88.97";			// Server Host
const string user	 = "breakingthecode";	    // User name
const string passwrd = "Csisfun1234";			// Password
const string db		 = "breakingthecode";		// Name of the database
const int port 		 = 3306;					// Port number

	SQL_XML run ( host, user, passwrd, db , port );

	/*string trial = " select * from user ";
	MYSQL* connection = mysql_init(NULL);
	run.connect( connection );
	cout << run.execute_command(connection,trial);*/

	run.getStartingInfo( "Jaynam Shah", "yahoo Mail");
	cout <<" insert is done " <<endl;
	string pass = run.passXML();
	cout << " XML form of resulting Query : \n\n" << pass << endl;

	system("pause");

	cout << " Parse of XML is \n"<< run.parseXML(pass);
	system("pause");

	cout << endl << endl;
	run.getEndingInfo("sdhfi24DS");
	string pass2 = run.passXML();
	cout << " XML form of resulting Query : \n\n" << pass2 << endl;

	system("pause");

	cout << " Parse of XML is \n"<< run.parseXML(pass2);
	system("pause");
}