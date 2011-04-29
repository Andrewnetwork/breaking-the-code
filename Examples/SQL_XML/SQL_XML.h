#ifndef SQL_XML_H
#define SQL_XML_H

#include <iostream>
#include <WinSock.h>
#include <string>
#include <vector>
#include <time.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include <fstream>

#include <mysql.h>
#include "xmlwriter.h"

class SQL_XML {

public :
	
	SQL_XML( string get_host, string get_username, string get_password,
				string get_db, int get_port);
	
	void connect		 ( MYSQL* connection );		// Connect to data base
	
	void getStartingInfo ( string u_name, string u_email );	// get Starting info from user
	void getEndingInfo	 ( string end_code );				// get code from the client
	string passXML		 ();								// This function return Data in XML formate
	
	string execute_command ( MYSQL* connection, string command );				// Perform an Query and return result
	static string parseXML ( string xml );					// It will parse XML and return plain text
private :
	
	
	string host, username, password,db;							// variable to connect to database
	int port ;
	
	MYSQL* connection;
	
	vector <string> user_id,user_name,user_email;
	vector <string> proccess_id,code;
	vector <string> time_start,time_end;

	char u_id , p_id ;
	int count;

	};
#endif