/*****************************************************************************************
*	Author : Jaynam Shah																 *
*																						 *
*	This program would access a database remotely. To run the program you will need some *
*	additional requirement in project properties. Read User Manual for further questions.*
*	This program is able create and make any changes into a web database.				 *
******************************************************************************************/

/**			Imported header files.						*/
#include <iostream>
#include <WinSock.h>
#include <string>
#include <mysql.h>
#include <vector>
#include <time.h>
#include <cstring>
#include <sstream>

using namespace std;

/**			Used Function to work with DataBase.		*/
void check_connection ( MYSQL* );				// Making or checking connection with Database
void execute_command  ( MYSQL*, string );		// Execute and print the result of SQL Query
void starting_info	  ( string, string );		// Get user name and Email
void ending_info	  ( string );				// Get cracking code from client
void poppulate_tables ();						// Poppulate tables ( user and procedure )
void SQL_Query 		  ();						// Make and execute SQL Query

/**				Creating Global variables.				*/
vector <string> user_id;
vector <string>	user_name;
vector <string> user_email;
vector <string> proccess_id;
vector <string> code;
vector <string> time_start;
vector <string> time_end;
string get_user_name, get_email, get_code;	
char u_id = '0';		
char p_id = '0';
int count =  0 ;

/**				Information to connect to Database.		*/
const string host 	 = "173.201.88.97";			// Server Host
const string user	 = "breakingthecode";	    // User name
const string passwrd = "Csisfun1234";			// Password
const string db		 = "breakingthecode";		// Name of the database
const int port 		 = 3306;					// Port number

/**				Some Messages.							*/
string Error 	 = "Error";								// Error message
string connected = " Program is connected to database"; // Connection message

/**				Creating an Object  of MySQL.			*/
MYSQL* connection = mysql_init(NULL);

/**********************************************************************************************/

void main()
{
	check_connection( connection );							// Establishing a connection.
	starting_info	( "Jaynam Shah", "jaynam15@gmail.com"); // Taking info. at start up
	ending_info	 	( " Xtys43" );  						// Taking info. when its end
//poppulate_tables();										// poppulate tables with given info
	SQL_Query		();										// Create and execute a Query

	system("pause");
	
}

void check_connection	( MYSQL* connection )
{
	/*****************************************************************************
	*	This function will connect this program to remote Database. It is using  *
	*	mysql_real_connect (...) function and print appropriate message.		 *
	*	It is important to connect successfully to access any of mysql functions.*
	*****************************************************************************/
	
	if (mysql_real_connect(
							connection,					// MySQL obeject
							host.c_str(),				// Server Host
							user.c_str(),				// User name of your database
							passwrd.c_str(),			// Password of the database
							db.c_str(),					// Database name
							port,						// port number
							NULL,						// Unix socket  ( for us it is null )
							0)== NULL)
   {
      cout << Error << mysql_error(connection) << "\n";	// Printing an Error Message
   }
   else 
   {
	   cout << connected <<endl;						// Printing connection messege.
   }
}

void starting_info		( string u_name, string u_email )
{
	/*******************************************************************************************
	*	This function would take user name, email and also store the starting time cracking	   *
	*	the code, and also store the process Id of cracking procedure.						   *
	*******************************************************************************************/
	stringstream ss;
	string s_u_id;
	++u_id;
	ss << u_id;
	ss >> s_u_id;

    time_t start;
	time ( &start );
	string starttime = ctime (&start);
	
	user_name.push_back ( u_name );					// Storing user name into vector
	user_id.push_back( s_u_id );							// Storing user id into vector
	user_email.push_back ( u_email );				// Storing user email into vector
	time_start.push_back(starttime.c_str());				// Storing current time into vector
	
	~ start;										// Destroying time class object
}

void ending_info		(	string end_code )
{
	/*******************************************************************************************
	*	This function would take user ending code from user. It would also store iformation of *
	*	Process Id, code and time ending for the cracking the code procedure.				   *
	*******************************************************************************************/
		
	time_t end;											// Creating object of time class
	time ( &end );										// Gettting current time
	string endtime = ctime (&end);						// Storing time into a string 
	
	++p_id;
	string s_p_id;
	stringstream ss;
	ss << p_id;
	ss >> s_p_id;

	proccess_id.push_back ( s_p_id );					// Storing proccess id into a vector
	code.push_back ( end_code );						// Storing code into a vector
	time_end.push_back ( endtime.c_str() );				// Storing ending time of proccess
	
	~ end;												// Destroying time classs object
	
	poppulate_tables();									// poppulating tables.
}


void execute_command ( MYSQL* connection, string command )
{
	/******************************************************************************
	*	This function take MYSQL obeject and command ( string ) and perform the   *
	*	Query and also print the result if there is any out put.				  *
	*******************************************************************************/
	
	
	if (mysql_query(
					connection,							// MySQL object
					command.c_str()						// SQL Command String
					) != 0)
   {
      cout << Error << endl << mysql_error(connection)  // Printing error message
	  << endl ;	
      return;
   }

   MYSQL_RES* result = mysql_store_result(connection);	//Storing the result into MYSQL_RES object
   if (result == NULL) return; 

   int rows = mysql_num_rows(result);					// Getting number of rows in result
   int fields = mysql_num_fields(result);				// Getting number of fields in rows
   
   for (int i = 1; i <= rows; i++)
   {
      MYSQL_ROW row = mysql_fetch_row(result);			// Separating rows and storing into MYSQL_ROW object

      for (int j = 0; j < fields; j++)
      {
         string field(row[j]);
         if (j > 0) cout << ",";
         cout << field;									// Printing the attribute
      }
      cout << "\n";										// separating lines in output
   }

   mysql_free_result(result);							// releasing memory
}

void poppulate_tables  ()
{
	/*********************************************************************************************
	*	This function would create a Inser value SQL command for user and process tables and 	 *
	*	poppulate them by calling execute_command (...) function.								 *
	**********************************************************************************************/
	
	// Making nessasary strings to generate SQL command
	string user_tab = "user ( user_id, user_name, user_email )";
	string proc_tab = "proccess ( process_id, user_id, code, start_time, end_time )";
	string insert 	= "Insert into ";
	string values 	= "values ( '";
	string syntax	= "','";
	string end_in	= "')";
	
	string insert_user = insert + user_tab + values + user_id[count] + syntax + user_name[count] +
						syntax + user_email[count] + end_in; // SQL command for inserting values in user table
	string insert_proc = insert + proc_tab + values + proccess_id[count] + syntax + user_id[count] +
						syntax + code[count] + syntax + time_start[count] +syntax + time_end[count] + end_in;
															// SQL command for inserting value in proccess table
	// executing previously generated SQL commands
	execute_command ( connection, insert_user );
	execute_command ( connection, insert_proc );
	
	count++;								// increament into count for vector.
	
}

void SQL_Query 		   ()
{
	
	string Query = "select user_name, user_emaiL, code, start_time, end_time from user, proccess order by proccess.user_id";
														 // Making a SQL Query 
	execute_command ( connection, Query ); 				// executing previously generated SQL commands
}