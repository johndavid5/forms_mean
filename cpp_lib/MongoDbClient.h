#ifndef _MONGO_DB_CLIENT_H_
#define _MONGO_DB_CLIENT_H_

#include <iostream>
#include <iomanip>
#include <string>

#ifdef __linux__
	#include <sys/time.h> // UNIX-specific: struct timeval tp and gettimeofday()...
#endif
using namespace std;

#include <mongoc.h>
#include "BsonTraverser.h"

#include "logger.h"

namespace JDA
{

class MongoDbClient { 

	protected:


		//static string m_indent_char;

		//static string get_indent_prefix( int i_level );

		JDA::Logger* m_p_logger; // User supplies a pointer to a JDA::Logger object if they would like logging enabled...

		JDA::Logger m_default_logger;

		mongoc_client_t* m_p_client; // Lazy initialize this one...

		string m_s_uri_str; // e.g., "mongodb://127.0.0.1/"

		/*
		* Performs lazy initialization if necessary,
		* of m_p_client.
		*
		* @throws MongoDbClient::Exception 
		*/
		void lazyInitPMongocClient();

		/*
		* Performs lazy initialization if necessary,
		* and returns a pointer to a mongoc_client_t*.
		*
		* @throws MongoDbClient::Exception 
		*/
		mongoc_client_t* getPMongocClient();

	public:

	/* If you'd like to observe bson documents as they are received... */
	class IMongoDbClientCallback {
		public:
			virtual void documentRecieved( const bson_t *p_bson_doc ) = 0; 
	};

	class Exception : std::exception {
		public:
			string errString;

			Exception(const string& errString){
				this->errString = errString;
			}

			virtual ~Exception() throw(){}

			virtual const char* what() const throw()
			{
				return this->errString.c_str();
			}
	}; /* class Exception : std::exception */


	void setPLogger( JDA::Logger* p_logger ){
		m_p_logger = p_logger;
	}

	/** You need to set this before connecting to client... */
	void setSUriStr( const string& s_uri_str ){
		m_s_uri_str = s_uri_str;
	}

	JDA::Logger* getPLogger(){
		return m_p_logger;
	}



	/** constructor */
	MongoDbClient();

	/** destructor */
	virtual ~MongoDbClient();

	/** utilities-start */
	string bson_as_json_string( const bson_t* p_bson );

	/** Experimental utility using bson_iter_recurse() */
	//string bson_as_pretty_json_string( const bson_t* p_bson );

	//void bson_traverse_doc( const bson_t* p_bson, int i_level, ostream* p_oss_out=NULL );
	//void bson_traverse_iter( bson_iter_t* p_bson, int i_level, ostream* p_oss_out=NULL );

	//static time_t seconds_since_unix_epoch();
	static int64_t milliseconds_since_unix_epoch();

	//static int64_t seconds_to_milliseconds( time_t seconds );
	//static time_t milliseconds_to_seconds( int64_t milliseconds );
	/** utilities-end */

	/** @throws MongoDbClient::Exception if something goes wrong with the command. */
	int command( const string& s_db_name, const string& s_collection_name, const string& s_json_command, JDA::MongoDbClient::IMongoDbClientCallback* p_callback = NULL );

	/** @throws MongoDbClient::Exception if something goes wrong with the command. */
	int find( const string& s_db_name, const string& s_collection_name, const string& s_json_query );

	/** "i-re-te ku-da-sa-i"
	*
	* @throws MongoDbClient::Exception if something goes wrong with the command.
	*/
	int insert( const string& s_db_name, const string& s_collection_name, const string& s_json_doc );

	/** "i-re-te ku-da-sa-i"
	* @throws MongoDbClient::Exception if something goes wrong with the command.
	*
	* https://docs.mongodb.com/manual/reference/method/db.collection.update/
	*
	* db.collection.update(
    *  <query>,
    *  <update>,
    *  {
    *   upsert: <boolean>,
    *   multi: <boolean>,
    *   writeConcern: <document>
    *  }
	* )
	*
	* @throws MongoDbClient::Exception if something goes wrong with the command.
	*/
	int update( const string& s_db_name, const string& s_collection_name, const string& s_json_query, const string& s_json_update );

}; /* class MongoDbClient */

} /* namespace JDA */

#endif /* #ifndef _MONGO_DB_CLIENT_H_ */
