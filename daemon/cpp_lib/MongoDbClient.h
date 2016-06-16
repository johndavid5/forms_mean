#ifndef _MONGO_DB_CLIENT_H_
#define _MONGO_DB_CLIENT_H_

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;


#include <mongoc.h>

#include "logger.h"

namespace JDA
{

class MongoDbClient { 

	protected:

		JDA::Logger* m_p_logger; // User supplies a pointer to a JDA::Logger object if they would like logging enabled...

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

	string bson_as_json_string( bson_t* p_bson );

	/** @throws MongoDbClient::Exception if something goes wrong with the command. */
	int find( const string& s_db_name, const string& s_collection_name, const string& s_json_query );

	/** "irete kudasai"
	* @throws MongoDbClient::Exception if something goes wrong with the command.
	*/
	int insert( const string& s_db_name, const string& s_collection_name, const string& s_json_query );

}; /* class MongoDbClient */

} /* namespace JDA */

#endif /* #ifndef _MONGO_DB_CLIENT_H_ */
