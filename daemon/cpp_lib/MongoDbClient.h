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

	public:

	class MongoDbException : std::exception {
		public:
			string errString;

			MongoDbException(const string& errString){
				this->errString = errString;
			}

			virtual const char* what() const throw()
			{
				return this->errString.c_str();
			}
	}; /* class MongoDbException : std::exception */


	void setPLogger( JDA::Logger* p_logger ){
		m_p_logger = p_logger;
	}

	JDA::Logger* getPLogger(){
		return m_p_logger;
	}

	/** constructor */
	MongoDbClient();

	/** destructor */
	virtual ~MongoDbClient();

	/** @throws MongoDbClient::DbException if something goes wrong with the command. */
	int find( const string& uri, const string& db_name, const string& collection_name, const string& json_query );

	/** <<irete kudasai>>
	* @throws MongoDbClient::DbException if something goes wrong with the command.
	*/
	int insert( const string& uri, const string& db_name, const string& collection_name, const string& json_query );

}; /* class MongoDbClient */

} /* namespace JDA */

#endif /* #ifndef _MONGO_DB_CLIENT_H_ */
