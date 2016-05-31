/* gcc example.c -o example $(pkg-config --cflags --libs libmongoc-1.0) */

/* ./example-client [CONNECTION_STRING [COLLECTION_NAME]] */

#include <mongoc.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
using namespace std;

#include "logger.h"
#include "MongoDbClient.h"

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
	string s_json_query = ""; // e.g., "{ \"student_id\": 2 }"

	JDA::Logger le_logger = JDA::Logger();
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
			mongoDbClient.find( s_uri, s_db_name, s_collection_name, s_json_query );					
		}
		else {
			print_format( cerr );
			return 255;
		}
	}
	catch( JDA::MongoDbClient::MongoDbException e ){
		cout << "Trouble with mongo: \"" << e.what() << "\"..." << endl;
	}

	cout << "Let off some steam, Bennett!" << endl;
	return 0;

}/* main() */

void print_format( ostream& oss_out ){
	oss_out << "FORMAT: " << G_S_ARGV_ZERO << " (find|insert) -uri <uri> -db <db_name> -collection <collection_name> -query <json_query>" << endl;
}
