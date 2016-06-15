#include "MongoDbClient.h"

	/** constructor */
	JDA::MongoDbClient::MongoDbClient() : m_p_logger(NULL), m_p_client(NULL), m_s_uri_str("")
   	{
		const char* sWho = "JDA::MongoDbClient::MongoDbClient";

		if( m_p_logger != NULL ){
			(*m_p_logger)(JDA::Logger::TRACE) << sWho << "()..." << endl;
		}
	}

	/** destructor */
	JDA::MongoDbClient::~MongoDbClient() {

		const char* sWho = "JDA::MongoDbClient::~MongoDbClient(destructor)";

		if( m_p_logger != NULL ){
			(*m_p_logger)(JDA::Logger::TRACE) << sWho << "()..." << endl;
		}

		if( m_p_client != NULL ){
			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling mongoc_client_destroy(p_client)..." << endl;
			}
			mongoc_client_destroy (m_p_client);

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling mongoc_cleanup()..." << endl;
			}
   			mongoc_cleanup();
		}

	}/* JDA::MongoDbClient::~MongoDbClient() */

	string JDA::MongoDbClient::bson_as_json_string( bson_t* p_bson ){

		const char* sWho = "MongoDbClient::bson_as_json_string";
		(void)*sWho; /* Unused variable...who says? */

		char *cp_bson_as_json;
		string s_bson_as_json;

		if( p_bson == NULL ){
			return "<NULL-INPUT>";
		}

		cp_bson_as_json = bson_as_json (p_bson, NULL);

		if( cp_bson_as_json != NULL ){
			//if( m_p_logger != NULL ){
			//	(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "cp_bson_as_json = \"" << cp_bson_as_json << "\"..." << endl;
			//} 
			s_bson_as_json = cp_bson_as_json;
			bson_free(cp_bson_as_json);
		}
		else {
			//if( m_p_logger != NULL ){
			//	(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "cp_bson_as_json is NULL..." << endl;
			//} 
			s_bson_as_json = "<NULL-OUTPUT>";
		}

		return s_bson_as_json;

	}/* string JDA::MongoDbClient::bson_as_json_string( bson_t* p_bson ) */


	int JDA::MongoDbClient::find( const string& s_uri_str, const string& s_db_name, const string& s_collection_name, const string& s_json_query ){

		const char* sWho = "MongoDbClient::find";

		mongoc_client_t *p_client;
		mongoc_collection_t *p_collection;

		bson_t* p_bson_query;
		char *cp_bson_as_json;
		string s_bson_as_json;

		mongoc_cursor_t *p_cursor;
		bson_error_t bson_error;
		const bson_t *p_bson_doc;

		//if( m_p_logger != NULL ){
		//	(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling mongoc_init()..." << endl;
		//}
		//mongoc_init ();

		//if( m_p_logger != NULL ){
		//	(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling mongoc_client_new( s_uri_str = \"" << s_uri_str << "\" )..." << endl;
		//}
		//p_client = mongoc_client_new (s_uri_str.c_str());

		//if (!p_client) {
		//	ostringstream oss_out;
		//	oss_out << "Failed to parse URI \"" << s_uri_str << "\"";
		//	throw JDA::MongoDbClient::Exception( oss_out.str() ); 
		//}

		p_client = this->getPMongocClient();

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): Calling bson_new_from_json( \"" << s_json_query << "\" )..." << endl;
		}
		p_bson_query = bson_new_from_json( (const uint8_t *) s_json_query.c_str(), -1, &bson_error );

		if( ! p_bson_query ){
			ostringstream oss_out;
			oss_out << "Trouble converting json to bson: \"" << bson_error.message << "\"" << endl;
			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): " << oss_out.str() << "...tossing JDA::MongoDbClient::Exception()..." << endl;
			}
			throw JDA::MongoDbClient::Exception( oss_out.str() ); 
		}

		// For debug purposes, convert the BSON back to JSON to see if looks the same
		// as the original...
		// REFERENCE: https://api.mongodb.com/libbson/current/bson_as_json.html
		cp_bson_as_json = bson_as_json (p_bson_query, NULL);
		if( cp_bson_as_json != NULL ){
			if( m_p_logger != NULL ){
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "cp_bson_as_json = \"" << cp_bson_as_json << "\"..." << endl;
			} 
			s_bson_as_json = cp_bson_as_json;
			bson_free(cp_bson_as_json);
		}
		else {
			if( m_p_logger != NULL ){
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "cp_bson_as_json is NULL..." << endl;
			} 
			s_bson_as_json = "<NULL>";
		}

		if( m_p_logger != NULL ){
			(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "s_bson_as_json = \"" << s_bson_as_json.c_str() << "\"..." << endl;
		} 

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "Getting s_db_name = \"" << s_db_name << "\", s_collection_name = \"" << s_collection_name << "\"..." << endl;
		}
		// Note: mongoc_client_get_collection() cannot fail...collection is created if it doesn't yet exist...
		p_collection = mongoc_client_get_collection (p_client, s_db_name.c_str(), s_collection_name.c_str() );


		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "Running db." << s_collection_name << ".find( " << s_bson_as_json << " )..." << endl;
		}

		p_cursor = mongoc_collection_find (p_collection,
                                    MONGOC_QUERY_NONE,
                                    0,
                                    0,
                                    0,
									p_bson_query,
                                    NULL,  /* Fields, NULL for all. */
                                    NULL /* Read Prefs, NULL for default */
									);

		int row_number = 0;

		while (mongoc_cursor_next (p_cursor, &p_bson_doc)) {

			row_number++;

			cp_bson_as_json = bson_as_json (p_bson_doc, NULL);

			if( cp_bson_as_json != NULL ){
				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "[" << row_number << "]: " << cp_bson_as_json << "\n" << endl; 
				}
				bson_free (cp_bson_as_json);
			}
			else {
				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "[" << row_number << "]: cp_bson_as_json is NULL" << "\n" << endl; 
				}
			}
		}

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << row_number << " row(s) returned..." << endl;
		}

		if (mongoc_cursor_error (p_cursor, &bson_error)) {
			ostringstream oss_out;
			oss_out << "Cursor Failure: \"" << bson_error.message << "\"" << endl;
			throw JDA::MongoDbClient::Exception( oss_out.str() ); 
		}

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling bson_destroy( p_bson_query )..." << endl;
		}
   		bson_destroy(p_bson_query); 

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling mongoc_cursor_destroy( p_cursor )..." << endl;
		}
		mongoc_cursor_destroy (p_cursor);

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling mongoc_collection_destroy(p_collection)..." << endl;
		}
		mongoc_collection_destroy (p_collection);

		//if( m_p_logger ){
		//	(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling mongoc_client_destroy(p_client)..." << endl;
		//}
		//mongoc_client_destroy (p_client);

		//if( m_p_logger ){
		//	(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling mongoc_cleanup()..." << endl;
		//}
   		//mongoc_cleanup();

		return row_number;

	}/* JDA::MongoDbClient::find() */



	int JDA::MongoDbClient::insert( const string& s_uri_str, const string& s_db_name, const string& s_collection_name, const string& s_json_doc){
		// http://api.mongodb.com/c/current/tutorial.html#insert

		const char* sWho = "MongoDbClient::insert";

  		mongoc_client_t *p_client = this->getPMongocClient();
	    mongoc_collection_t *p_collection;
	    bson_error_t bson_error;

		bson_t *p_bson_doc;

		//if( m_p_logger != NULL ){
		//	(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling mongoc_init()..." << endl;
		//}
		//mongoc_init ();

		//if( m_p_logger != NULL ){
		//	(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling mongoc_client_new( s_uri_str = \"" << s_uri_str << "\" )..." << endl;
		//}
		//p_client = mongoc_client_new (s_uri_str.c_str());

		//if (!p_client) {
		//	ostringstream oss_out;
		//	oss_out << "Failed to parse URI \"" << s_uri_str << "\"";
		//	throw JDA::MongoDbClient::Exception( oss_out.str() ); 
		//}

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): Calling bson_new_from_json( \"" << s_json_doc << "\" )..." << endl;
		}
		p_bson_doc = bson_new_from_json( (const uint8_t *) s_json_doc.c_str(), -1, &bson_error );

		if( ! p_bson_doc ){
			ostringstream oss_out;
			oss_out << "Trouble converting json to bson: \"" << bson_error.message << "\"" << endl;
			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): " << oss_out.str() << "...tossing JDA::MongoDbClient::Exception()..." << endl;
			}
			throw JDA::MongoDbClient::Exception( oss_out.str() ); 
		}

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "Calling mongoc_client_get_collection( p_client, \"" << s_db_name << "\", \"" << s_collection_name << "\" )..." << endl;
		}

		// Note: mongoc_client_get_collection() cannot fail...collection is created if it doesn't yet exist...
		p_collection = mongoc_client_get_collection (p_client, s_db_name.c_str(), s_collection_name.c_str() );

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "Running db." << s_collection_name << ".insert( " << this->bson_as_json_string( p_bson_doc ) << " )..." << endl;
		}


		if (!mongoc_collection_insert (p_collection, MONGOC_INSERT_NONE, p_bson_doc, NULL, &bson_error)) {
			ostringstream oss_out;
			oss_out << "Trouble inserting document: \"" << bson_error.message << "\"" << endl;
			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): " << oss_out.str() << "...tossing JDA::MongoDbClient::Exception()..." << endl;
			}
			throw JDA::MongoDbClient::Exception( oss_out.str() ); 
		}

		// Extra Credit: Use mongoc_collection_get_last_error() to ascertain _id of newly inserted document...or try to...
		// const bson_t * mongoc_collection_get_last_error (const mongoc_collection_t *collection);
		
		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "Running bson_last_insert = mongoc_collection_get_last_error( " << s_collection_name << " ) to view newly inserted record..." << endl;
		}

		//const bson_t* p_bson_last_insert = mongoc_collection_get_last_error( p_collection ); 
		bson_t* p_bson_last_insert = (bson_t * ) mongoc_collection_get_last_error( p_collection ); 

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "bson_as_json_string( bson_last_insert ) = \"" << this->bson_as_json_string( p_bson_last_insert ) << "\"..." << endl;
		}

		// Not a good idea...
		//if( m_p_logger ){
		//	(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling bson_destroy( p_bson_last_insert )..." << endl;
		//}
   		//bson_destroy(p_bson_last_insert ); 

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling bson_destroy( p_bson_doc )..." << endl;
		}
   		bson_destroy(p_bson_doc); 

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling mongoc_collection_destroy(p_collection)..." << endl;
		}
		mongoc_collection_destroy (p_collection);

		//if( m_p_logger ){
		//	(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling mongoc_client_destroy(p_client)..." << endl;
		//}
		//mongoc_client_destroy (p_client);

		//if( m_p_logger ){
		//	(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling mongoc_cleanup()..." << endl;
		//}
   		//mongoc_cleanup();

		return 1;

	}/* int JDA::MongoDbClient::insert() */


	void JDA::MongoDbClient::lazyInitPMongocClient(){

		const char* sWho = "JDA::MongoDbClient::lazyInitPMongocClient";

		if( m_p_logger != NULL ){
			(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "m_p_client = " << m_p_client << endl;
		}

		if( m_p_client == NULL ){

			if( m_p_logger != NULL ){
				(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): " << "m_p_client is NULL, going ahead with lazy initialization..." << endl;
			}

			if( m_p_logger != NULL ){
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling mongoc_init()..." << endl;
			}
			mongoc_init ();
		
			if( m_p_logger != NULL ){
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling mongoc_client_new( m_s_uri_str = \"" << m_s_uri_str << "\" )..." << endl;
			}
			m_p_client = mongoc_client_new (m_s_uri_str.c_str());

			if( m_p_logger != NULL ){
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "After mongoc_client_new(), m_p_client = " << m_p_client << "..." << endl;
			}
		
			if (!m_p_client) {
				ostringstream oss_out;
				oss_out << "Failed to parse URI \"" << m_s_uri_str << "\"";
				throw JDA::MongoDbClient::Exception( oss_out.str() ); 
			}

		} /* if( m_p_client == NULL ) */
		else {
			if( m_p_logger != NULL ){
				(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): " << "m_p_client is not NULL, no need to lazy initialize it..." << endl;
			}
		}

	}/* void JDA::MongoDbClient::lazyInitPMongocClient() */


	mongoc_client_t* JDA::MongoDbClient::getPMongocClient(){
		this->lazyInitPMongocClient();
		return m_p_client;
	}


