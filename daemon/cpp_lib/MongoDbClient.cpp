#include "MongoDbClient.h"

	/** constructor */
	JDA::MongoDbClient::MongoDbClient() : m_p_logger(NULL)
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

	}/* JDA::MongoDbClient::~MongoDbClient() */

	int JDA::MongoDbClient::find( const string& uri_str, const string& db_name, const string& collection_name, const string& json_query ){

		mongoc_client_t *p_client;
		mongoc_collection_t *p_collection;
		mongoc_cursor_t *p_cursor;
		bson_error_t bson_error;
		const bson_t *p_bson_doc;

		if( m_p_logger != NULL ){
			(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling mongoc_init()..." << endl;
		}
		mongoc_init ();

		if( m_p_logger != NULL ){
			(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling mongoc_client_new( uri_str = \"" << uri_str << "\" )..." << endl;
		}
		p_client = mongoc_client_new (uri_str);

		if (!p_client) {
			ostringstream oss_out;
			oss_out << "Failed to parse URI \"" << uri_str << "\"";
			throw new JDA::MongoDbException( oss_out.str() ); 
		}


	}

