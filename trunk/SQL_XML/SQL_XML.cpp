#include "SQL_XML.h"
using namespace std;

SQL_XML::SQL_XML( string get_host, string get_username, string get_password,string get_db, int get_port)
		: host(get_host), username(get_username), password(get_password), db(get_db), port(get_port)
{
	connection = mysql_init(NULL);

	host = get_host; username = get_username; password = get_password;
	db = get_db; port = get_port;
	count =  0;
	
	connect ( connection );		// Connect to data base
	
	string que = "select process_id from proccess  ORDER BY proccess.process_id  DESC";
	string id = execute_command( connection , que );
	if ( id.length() == 0 ) 
	{
		t_id = 0;
	} 
	else
	{
		int i = 0;
		string passId;
		while ( i < id.length() && id[i] != ' ' && id[i] != ',')
		{
			passId = passId + id[i];
			i++;
		}
		
		istringstream myStream(passId);
		myStream >> t_id;
	}
}

void SQL_XML::connect ( MYSQL* connection )
{
	/*****************************************************************************
	*	This function will connect this program to remote Database. It is using  *
	*	mysql_real_connect (...) function and print appropriate message.		 *
	*	It is important to connect successfully to access any of mysql functions.*
	*****************************************************************************/
	
	if (mysql_real_connect(
							connection,					// MySQL obeject
							host.c_str(),				// Server Host
							username.c_str(),				// User name of your database
							password.c_str(),			// Password of the database
							db.c_str(),					// Database name
							port,						// port number
							NULL,						// Unix socket  ( for us it is null )
							0)== NULL)
   {
      cout << "Error" << mysql_error(connection) << "\n";	// Printing an Error Message
	  connect(connection);
   }
   else 
   {
	   cout << "DBS is connected." <<endl;						// Printing connection messege.
   }
}

void SQL_XML::getStartingInfo ( string u_name, string u_email )
{
	/*******************************************************************************************
	*	This function would take user name, email and also store the starting time cracking	   *
	*	the code, and also store the process Id of cracking procedure.						   *
	*******************************************************************************************/
	t_id ++;
	stringstream ss;
	ss << t_id;
	string id = ss.str();

    time_t start;
	time ( &start );
	string starttime = ctime (&start);
	
	user_name.push_back ( u_name );							// Storing user name into vector
	user_id.push_back( id );						    	// Storing user id into vector
	user_email.push_back ( u_email );						// Storing user email into vector
	
	proccess_id.push_back(id);
	code.push_back("NULL");
	time_start.push_back(starttime.c_str());				// Storing current time into vector
	time_end.push_back("NULL");

	string user_tab = "user ( user_id, user_name, user_email )";
	string proc_tab = "proccess ( process_id, user_id, code, start_time, end_time )";
	string insert 	= "Insert into ";
	string values 	= "values ('";
	string syntax	= "','";
	string end_in	= "')";
	
	string insert_user = insert + user_tab + values + user_id[count] + syntax + user_name[count] +
						syntax + user_email[count] + end_in;
	string insert_proc = insert + proc_tab + values + proccess_id[count] + syntax + user_id[count] +
						syntax + code[count] + syntax + time_start[count] +syntax + time_end[count] + end_in;
	
	execute_command ( connection, insert_user);
	execute_command ( connection, insert_proc);
	
	++count;
}

void SQL_XML::getEndingInfo ( string end_code )
{
	/*******************************************************************************************
	*	This function would take user ending code from user. It would also store iformation of *
	*	Process Id, code and time ending for the cracking the code procedure.				   *
	*******************************************************************************************/

	count= count - 1;
		
	time_t end;											// Creating object of time class
	time ( &end );										// Gettting current time
	string end_time = ctime (&end);						// Storing time into a string 
	
	code[count] = end_code;								// Storing code into a vector
	time_end[count] = end_time.c_str() ;				// Storing ending time of proccess
	
	string insert_proc = "UPDATE proccess SET code ='" + code [count] + "' , end_time ='" + time_end [count] +"' WHERE process_id ='" + proccess_id[count] + "' AND user_id ='" + user_id[count]+ "' ";
	execute_command ( connection, insert_proc );

	++count;
}

string SQL_XML::passXML	()
{
	// Making a SQL Query 
	string Query = "select user.user_name, user.user_email, proccess.code, proccess.start_time, proccess.end_time from user, proccess where user.user_id = proccess.user_id group by user.user_name ORDER BY proccess.start_time";
		
	// executing previously generated SQL commands
	string str = execute_command ( connection, Query ); 	
	
	// making an XML file of result
	if ( remove("sample.xml") != 0 )
		cout << " Error in deleting a file. " <<endl;
	else
		cout << " Delete is succeessful. " << endl;

	ofstream f("sample.xml");		// Creating an XML file.
	string temp;
	vector < string > v;

	for ( int i = 0; i < str.size(); i++ )
	{
		temp = "";
		while ( i < str.size() && str[i] != ',' && str[i] != '\n')
		{
			temp =  temp + str[i] ;
			i++;
		}
		if ( temp !="" && temp !="\n" && temp != "," && temp != " ")
			v.push_back(temp);
	}
	reverse( v.begin(), v.end());

	/*	Making an XML */
	f << version() << tag ("DataBase") ;
	for ( int i = 0; i < v.size(); i++ )
	{		 
		// Storing Data in a XML formate
			f<< tag ( "User") 
				<< tag ( "Name") << v[i++] <<endtag("Name")
				<< tag( "Email") << v[i++] << endtag ("Email")
				<< tag( "Code") << v[i++] << endtag ("Code")
				<< tag( "Start_Time") << v[i++] << endtag ("Start_Time")
				<< tag( "End_Time") << v[i++] << endtag ("End_Time")
			<< endtag ("User");
		i--;
	}
	f << endtag("DataBase");		
	f.close();

	/*	Reading back an XML file.	*/
	string line, line1;
	ifstream myfile("sample.xml");
	if (myfile.is_open())
	{
		while ( myfile.good() )
		{
			getline(myfile,line);
			line1 = line1 + line;
		}
		myfile.close();
		return line1;
	}
	else
	{
		return "\n Error. Cannot open file . \n";	// Returning an Error message
	}
}

string SQL_XML::parseXML ( string temp )
{
	if ( temp.size() == 0 )
	{
		return "Empty string \n" ;
	}
	else
	{
		string result;
		int count = 0; string name = " User Name : \t";
		string email = " Email : \t"; string code = " Cracked Code :\t";
		string time_s = " Start Time :\t"; string time_e = " End Time :\t";
		temp.pop_back();
		for ( int i = 0; i < temp.size(); i++)
		{
			if (( temp[i] =='>')&&(temp[++i]!='<'))
			{
				switch ( count )
				{
					case 0:
							result = result + name;
							break;
					case 1 :
							result = result + email;
							break;
					case 2 :
							result = result + code;
							break;
					case 3 : 
							result = result + time_s;
							break;
					case 4 :
							result = result + time_e;
							break;
				}
				while ( i<temp.size()&&temp[i]!='<')
				{	
					result.push_back(temp[i]);
					++i;
				}
				result.push_back('\n');
				count = count + 1;
				if ( count > 4 )
				{
					result = result + "=========================================\n";
					count = 0;
				}
			}
		}
		return result;
	}
}

string SQL_XML::execute_command ( MYSQL* connection, string command )
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
      cout << "ERROR" << endl << mysql_error(connection)  // Printing error message
			<< endl ;	
      return  "";
   }

   MYSQL_RES* result = mysql_store_result(connection);	//Storing the result into MYSQL_RES object
  
   if (result == NULL)
	   return ""; 
   else 
   {
	   string prnt="";
	   int rows = mysql_num_rows(result);					// Getting number of rows in result
	   int fields = mysql_num_fields(result);				// Getting number of fields in rows
	   for (int i = 1; i <= rows; i++)
	   {
		  MYSQL_ROW row = mysql_fetch_row(result);			// Separating rows and storing into MYSQL_ROW object

		  for (int j = 0; j < fields; j++)
		  {
			 string field(row[j]);
			 
			 if (j > 0) 
			 {
				 prnt = prnt +  ",";
			 }
			prnt = prnt +  field ;							// adding the attribute
		  }
		  prnt = prnt + "\n";								// separating lines in output
	   }
	   mysql_free_result(result);							// releasing memory   
	   return prnt;
	} 
}

string SQL_XML::version ()
{
	// Creating an Version
	string title = "<?xml version=\"1.0\"?>";
	return title;
}

string SQL_XML::tag(string heading)
{
	// Creating a tag
	string start = "<";
	string end   = ">";
	return start+heading+end;
}

string SQL_XML::endtag ( string heading )
{
	// Closing tag
	string start = "</";
	string end   = ">";
	return start+heading+end;
}
