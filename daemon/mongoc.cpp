/* gcc example.c -o example $(pkg-config --cflags --libs libmongoc-1.0) */

/* ./example-client [CONNECTION_STRING [COLLECTION_NAME]] */

#include <mongoc.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <iostream>

#include "logger.h"
#include "MongoDbClient.h"

using namespace std;

string G_S_ARGV_ZERO = "";

void print_format( ostream& oss_out );

int
main (int   argc,
      char *argv[])
{
	/* Get C and C++ I/O to play nice together... */
	cout.sync_with_stdio(true);

	G_S_ARGV_ZERO = argv[0];

	string s_verb = ""; // e.g., "find"
	string s_uri = ""; // e.g., "mongodb://127.0.0.1/"
	string s_db_name = ""; // e.g., "test"
	string s_collection_name = ""; // e.g., "grades"
	//string s_json_query = "{}"; // e.g., "{ \"student_id\": 2 }"
	string s_json_query = ""; // e.g., "{ \"student_id\": 2 }"

	//JDA::Logger le_logger = JDA::Logger();
	JDA::Logger le_logger;
	le_logger.setDebugLevel( JDA::Logger::TRACE );

	cout << "argc = " << argc << "..." << endl;
	for(int i = 0 ; i < argc; i++ ){
		cout << "argv[" << i << "] = " << argv[i] << "..." << endl;
	}

	if( argc >= 2 ){
		s_verb = argv[1];
	}

	for(int i = 2 ; i < argc; i++ ){
		if( strcmp( argv[i], "-uri" ) == 0 && i+1 < argc ){ 
			s_uri = argv[++i];
		}
		else if( strcmp( argv[i], "-db" ) == 0 && i+1 < argc ){ 
			s_db_name = argv[++i];
		}
		else if( strcmp( argv[i], "-collection" ) == 0 && i+1 < argc ){ 
			s_collection_name = argv[++i];
		}
		else if( strcmp( argv[i], "-query" ) == 0 && i+1 < argc ){ 
			s_json_query = argv[++i];
		}
	}

	cout << "s_verb = \"" << s_verb << "\"..." << endl;
	cout << "s_uri = \"" << s_uri << "\"..." << endl;
	cout << "s_db_name = \"" << s_db_name << "\"..." << endl;
	cout << "s_collection_name = \"" << s_collection_name << "\"..." << endl;
	cout << "s_json_query = \"" << s_json_query.c_str() << "\"..." << endl;

	JDA::MongoDbClient mongoDbClient;
	mongoDbClient.setPLogger( & le_logger );

	try {
		if( s_verb.compare("find") == 0 ){
			cout << "Calling mongoDbClient.find( \"" << s_uri << "\", \"" << s_db_name << "\", \"" << s_collection_name << "\", \"" << s_json_query << "\")..." << endl;
			try {
				int i_ret_code = mongoDbClient.find( s_uri, s_db_name, s_collection_name, s_json_query );					
				cout << i_ret_code << " row(s) returned." << endl;
			}
			catch( JDA::MongoDbClient::Exception& e ){
				cout << "Caught JDA::MongoDbClient::Exception during MongoDbClient::find(): \"" << e.what() << "\"..." << endl;
			}
			catch( ... ){
				cout << "Caught unknown exception during MongoDbClient::find()." << endl;
			}
		}
		else if( s_verb.compare("insert") == 0 ){
			cout << "Calling mongoDbClient.insert( \"" << s_uri << "\", \"" << s_db_name << "\", \"" << s_collection_name << "\", \"" << s_json_query << "\")..." << endl;
			try {
				int i_ret_code = mongoDbClient.insert( s_uri, s_db_name, s_collection_name, s_json_query );					
				cout << "insert() returned " << i_ret_code << "..." << endl;
			}
			catch( JDA::MongoDbClient::Exception& e ){
				cout << "Caught JDA::MongoDbClient::Exception during MongoDbClient::insert(): \"" << e.what() << "\"..." << endl;
			}
			catch( ... ){
				cout << "Caught unknown exception during MongoDbClient::insert()." << endl;
			}
		}
		else {
			print_format( cerr );
			return 255;
		}
	}
	catch( JDA::MongoDbClient::Exception& e ){
		cout << "Trouble with mongo: \"" << e.what() << "\"..." << endl;
	}
	catch( ... ){
		cout << "Caught unknown exception." << endl;
	}

	cout << "Let off some steam, Bennett!" << endl;
	return 0;

}/* main() */

void print_format( ostream& oss_out ){
	oss_out << "FORMAT: " << G_S_ARGV_ZERO << " (find|insert) -uri <uri> -db <db_name> -collection <collection_name> -query <json_query>" << endl;
}
