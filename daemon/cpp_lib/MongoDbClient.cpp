#include "MongoDbClient.h"

	/* static */ /* string JDA::MongoDbClient::m_indent_char = " "; */
	/* static */ string JDA::MongoDbClient::m_indent_char = "\t";

	/** constructor */
	//JDA::MongoDbClient::MongoDbClient() : m_p_logger(NULL), m_p_client(NULL), m_s_uri_str("")
	JDA::MongoDbClient::MongoDbClient() : m_p_client(NULL), m_s_uri_str("")
   	{
		const char* sWho = "JDA::MongoDbClient::MongoDbClient";

		m_p_logger = &m_default_logger;

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


	/** utilities-start */
	string JDA::MongoDbClient::bson_as_json_string( const bson_t* p_bson ){

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

	string JDA::MongoDbClient::bson_as_pretty_json_string( const bson_t* p_bson ){

		ostringstream oss_out;

		oss_out << "{\n";

		bson_traverse_doc( p_bson, 1, &oss_out );

		oss_out << "\n}";

		return oss_out.str();

	}/* bson_as_pretty_json_string() */

	void JDA::MongoDbClient::bson_traverse_doc( const bson_t* p_bson_doc, int i_level, ostream* p_oss_out  ){

		const char* sWho = "JDA::MongoDbClient::bson_traverse_doc";

		(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << "..." << endl;

		bson_iter_t iter;
		//const char* key;
		//bson_iter_t child;
		//int j = 0;

		if (bson_iter_init (&iter, p_bson_doc)) {

			bson_traverse_iter( &iter, i_level, p_oss_out );

		}/* if (bson_iter_init (&iter, p_bson_doc)) */

	}/* bson_traverse_doc() */

	/* static */ string JDA::MongoDbClient::get_indent_prefix( int i_level ){

		ostringstream oss;

		for( int i = 0; i < i_level; i++ ){
			oss << m_indent_char;
		}

		return oss.str();
	}

	void JDA::MongoDbClient::bson_traverse_iter( bson_iter_t* p_bson_iter, int i_level, ostream* p_oss_out ){

		const char* sWho = "JDA::MongoDbClient::bson_traverse_iter";

		(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << "..." << endl;

		string s_indent_prefix = get_indent_prefix( i_level );

		const char* key;

		const char* utf8;
		uint32_t utf8_len;

		int32_t int32;
		int64_t int64;

		const bson_oid_t* p_bson_oid;
		char bson_oid_str[25];

		bson_iter_t child;
		int j = 0;

			while (bson_iter_next (p_bson_iter)) {

				j++;

				key = bson_iter_key( p_bson_iter );

				if( BSON_ITER_IS_KEY( p_bson_iter, key ) ){ 
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": found a field with key \"" << key << "\"..." << endl;

					if( p_oss_out ){	
						//(*p_oss_out) << s_indent_prefix << "\"" << key << "\" : ";
						(*p_oss_out) << "\n" << s_indent_prefix << "\"" << key << "\" : ";
					}
				}
				else {
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": found a field \"" << key << "\" that is not a key..." << endl;
				}

				// http://cpansearch.perl.org/src/MONGODB/MongoDB-v1.4.2/bson/bson-iter.h

				if( BSON_ITER_HOLDS_DOCUMENT( p_bson_iter ) ){

					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_DOCUMENT..." << endl;


					if( bson_iter_recurse(p_bson_iter, &child) ){
						(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << " j = " << j << ": found a sub-document child via bson_iter_recurse()...traversing it via bson_traverse_iter()..." << endl;

						if( p_oss_out ){	
							//(*p_oss_out) << s_indent_prefix << "{\n";
							(*p_oss_out) << "{\n";
						}

						bson_traverse_iter( &child, i_level + 1, p_oss_out );

						if( p_oss_out ){	
							(*p_oss_out) << s_indent_prefix << " },\n";
						}
					}


				}/* if( BSON_ITER_HOLDS_DOCUMENT( &iter ) */
				else if( BSON_ITER_HOLDS_ARRAY( p_bson_iter ) ){

					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_ARRAY..." << endl;

					if( bson_iter_recurse(p_bson_iter, &child) ){
						(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << " j = " << j << ": found an array-element child via bson_iter_recurse()...traversing it via bson_traverse_iter()..." << endl;

						if( p_oss_out ){	
							//(*p_oss_out) << s_indent_prefix << " [\n";
							(*p_oss_out) << "[\n";
						}

						bson_traverse_iter( &child, i_level + 1, p_oss_out );

						if( p_oss_out ){	
							(*p_oss_out) << s_indent_prefix << " ],\n";
						}
					}

				}/* if( BSON_ITER_HOLDS_ARRAY( &iter ) */
				else if( BSON_ITER_HOLDS_DOUBLE( p_bson_iter ) ){
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_DOUBLE..." << endl;

					double le_double = bson_iter_double( p_bson_iter );  

					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": le_double = " << le_double << "..." << endl;

					if( p_oss_out ){	
						//(*p_oss_out) << s_indent_prefix << " " << int32 << ",\n";
						//(*p_oss_out) << "" << le_double << ",\n";
						(*p_oss_out) << "" << le_double << ",";
					}

				}
				else if( BSON_ITER_HOLDS_UTF8( p_bson_iter ) ){

					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_UTF8..." << endl;

					utf8 = bson_iter_utf8( p_bson_iter, &utf8_len );

					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": utf8_len = " << utf8_len << "..." << endl;

					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": utf8 = \"" << utf8 << "\"..." << endl;

					if( p_oss_out ){	
						//(*p_oss_out) << s_indent_prefix << " \"" << utf8 << "\",\n";
						//(*p_oss_out) << "\"" << utf8 << "\",\n";
						(*p_oss_out) << "\"" << utf8 << "\",";
					}

				}/* if( BSON_ITER_HOLDS_UTF8( &iter ) */
				else if( BSON_ITER_HOLDS_BINARY( p_bson_iter ) ){
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_BINARY..." << endl;
				}
				else if( BSON_ITER_HOLDS_UNDEFINED( p_bson_iter ) ){
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_UNDEFINED..." << endl;
				}
				else if( BSON_ITER_HOLDS_OID( p_bson_iter ) ){

					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_OID..." << endl;

					p_bson_oid = bson_iter_oid( p_bson_iter );
					bson_oid_to_string( p_bson_oid, bson_oid_str );

					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": bson_oid_str = \"" << bson_oid_str << "\"..." << endl;

					if( p_oss_out ){	
						//(*p_oss_out) << s_indent_prefix << " { $oid: \"" << bson_oid_str << "\" },\n";
						//(*p_oss_out) << "{ $oid: \"" << bson_oid_str << "\" },\n";
						(*p_oss_out) << "{ $oid: \"" << bson_oid_str << "\" },";
					}

					//uint8_t le_byte;
					unsigned int le_byte;

					for( size_t i = 0; i < sizeof(p_bson_oid->bytes)/sizeof(uint8_t); i++ ){
						le_byte = p_bson_oid->bytes[i];
						(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": p_bson_oid->bytes[" << setw(2) << setfill('0') << i << "] = 0x" << setfill('0') << setw(2) << hex << le_byte << " = " << dec << le_byte << "..." << endl;
					}

					#ifdef WIN32 
						//DWORD WINAPI GetCurrentProcessId(void);
						DWORD our_process_id = GetCurrentProcessId();
						(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": [WIN32] GetCurrentProcessId() = " << "0x" << hex << setfill('0') << setw(sizeof(our_process_id)*2) << our_process_id << " = " << dec << our_process_id << "..." << endl;
					#endif

					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": sizeof(oid_struct) = " << sizeof(oid_struct) << "..." << endl;
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": sizeof(p_bson_oid->bytes) = " << sizeof(p_bson_oid->bytes) << "..." << endl;

					oid_struct our_oid_struct;
					memcpy( &our_oid_struct, p_bson_oid->bytes, sizeof( p_bson_oid->bytes) );

					time_t seconds_since_unix_epoch;
					memcpy( &seconds_since_unix_epoch, our_oid_struct.seconds_since_unix_epoch, sizeof(our_oid_struct.seconds_since_unix_epoch)  );

					unsigned int machine_id;
					memcpy( &machine_id, our_oid_struct.machine_id, sizeof(our_oid_struct.machine_id)  );

					unsigned int process_id;
					memcpy( &process_id, our_oid_struct.process_id, sizeof(our_oid_struct.process_id)  );

					unsigned int counter;
					memcpy( &counter, our_oid_struct.counter, sizeof(our_oid_struct.counter)  );

					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": seconds_since_unix_epoch = 0x" << hex << setw(sizeof(seconds_since_unix_epoch)*2) << setfill('0') << seconds_since_unix_epoch << " = " << dec << seconds_since_unix_epoch << "..." << endl;

					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": machine_id = 0x" << hex << setw(sizeof(machine_id)*2) << setfill('0') << machine_id << " = " << dec << machine_id << "..." << endl;

					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": process_id = 0x" << hex << setw(sizeof(process_id)*2) << setfill('0') << process_id << " = " << dec << process_id << "..." << endl;

					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": counter = 0x" << hex << setw(sizeof(counter)*2) << setfill('0') << counter << " = " << dec << counter << "..." << endl;
					
					time_t oid_unix_time = bson_oid_get_time_t( p_bson_oid );
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": oid_unix_time = " << hex << "0x" << oid_unix_time << " = " << dec << oid_unix_time << " = \"" << JDA::Utils::get_nyc_pretty_timestamp(oid_unix_time) << "\"..." << endl;

				}/* if( BSON_ITER_HOLDS_OID( &iter ) */
				else if( BSON_ITER_HOLDS_BOOL( p_bson_iter ) ){
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_BOOL..." << endl;
				}
				else if( BSON_ITER_HOLDS_DATE_TIME( p_bson_iter ) ){
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_DATE_TIME..." << endl;
				}
				else if( BSON_ITER_HOLDS_NULL( p_bson_iter ) ){
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_NULL..." << endl;
				}
				else if( BSON_ITER_HOLDS_REGEX( p_bson_iter ) ){
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_REGEX..." << endl;
				}
				else if( BSON_ITER_HOLDS_DBPOINTER( p_bson_iter ) ){
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_DBPOINTER..." << endl;
				}
				else if( BSON_ITER_HOLDS_CODE( p_bson_iter ) ){
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_CODE..." << endl;
				}
				else if( BSON_ITER_HOLDS_SYMBOL( p_bson_iter ) ){
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_SYMBOL..." << endl;
				}
				else if( BSON_ITER_HOLDS_CODEWSCOPE( p_bson_iter ) ){
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_CODEWSCOPE..." << endl;
				}
				else if( BSON_ITER_HOLDS_INT32( p_bson_iter ) ){

					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_INT32..." << endl;

					int32 = bson_iter_int32( p_bson_iter );

					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": int32 = \"" << int32 << "\"..." << endl;

					if( p_oss_out ){	
						//(*p_oss_out) << s_indent_prefix << " " << int32 << ",\n";
						//(*p_oss_out) << "" << int32 << ",\n";
						(*p_oss_out) << "" << int32 << ",";
					}

				}/* if( BSON_ITER_HOLDS_INT32( &iter ) */
				else if( BSON_ITER_HOLDS_TIMESTAMP( p_bson_iter ) ){
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_TIMESTAMP..." << endl;
				}
				else if( BSON_ITER_HOLDS_INT64( p_bson_iter ) ){

					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_INT64..." << endl;

					int64 = bson_iter_int64( p_bson_iter );

					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": int64 = \"" << int64 << "\"..." << endl;

					if( p_oss_out ){	
						//(*p_oss_out) << s_indent_prefix << " " << int64 << ",\n";
						//(*p_oss_out) << "" << int64 << ",\n";
						(*p_oss_out) << "" << int64 << ",";
					}

				}/* if( BSON_ITER_HOLDS_INT64( &iter ) */
				else if( BSON_ITER_HOLDS_MAXKEY( p_bson_iter ) ){
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_MAXKEY..." << endl;
				}
				else if( BSON_ITER_HOLDS_MINKEY( p_bson_iter ) ){
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_MINKEY..." << endl;
				}

			}/* while (bson_iter_next (p_bson_iter)) */

	}/* bson_traverse_iter() */

#ifdef _WIN32
	/** Since gettimeofday() is not defined on Windows, we'll define
	* our own version...(on UNIX, it's found in sys/time.h)
	* @reference http://stackoverflow.com/questions/10905892/equivalent-of-gettimeday-for-windows
	*/
	#include <Windows.h>
	#include <stdint.h> // portable: uint64_t   MSVC: __int64 
	
	// MSVC defines timeval in winsock2.h!?
	typedef struct wintimeval {
	    long tv_sec;
	    long tv_usec;
	} wintimeval;
	
	int gettimeofday(struct wintimeval * tp, struct timezone * tzp)
	{
	    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
	    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);
	
	    SYSTEMTIME  system_time;
	    FILETIME    file_time;
	    uint64_t    time;
	
	    GetSystemTime( &system_time );
	    SystemTimeToFileTime( &system_time, &file_time );
	    time =  ((uint64_t)file_time.dwLowDateTime )      ;
	    time += ((uint64_t)file_time.dwHighDateTime) << 32;
	
	    tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
	    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
	    return 0;
	}
#endif

	/* static */
	//time_t JDA::MongoDbClient::seconds_since_unix_epoch(){
	//	time_t seconds_since_unix_epoch = time( NULL );
	//	return seconds_since_unix_epoch;
	//}/* seconds_since_unix_epoch() */

	/* static */ int64_t JDA::MongoDbClient::milliseconds_since_unix_epoch(){
		//return JDA::MongoDbClient::seconds_to_milliseconds( JDA::MongoDbClient::seconds_since_unix_epoch() );

		#ifdef __linux__
		struct timeval tp; // from <sys/time.h>
		#elif _WIN32
		struct wintimeval tp;
		#endif

	    gettimeofday(&tp, NULL);

		int64_t mslong = (int64_t) tp.tv_sec * 1000L + tp.tv_usec / 1000; //get current timestamp in milliseconds
		return mslong;
	}/* milliseconds_since_unix_epoch() */

	/* static */
	//int64_t JDA::MongoDbClient::seconds_to_milliseconds( time_t seconds ){
	//	return seconds * 1000;
	//}/* seconds_to_milliseconds() */

	/* static */
	//time_t JDA::MongoDbClient::milliseconds_to_seconds( int64_t milliseconds ){
	//	return milliseconds / 1000;
	//}/* milliseconds_to_seconds() */
	
	/** utilities-end */

	int JDA::MongoDbClient::command( const string& s_db_name, const string& s_collection_name, const string& s_json_command, JDA::MongoDbClient::IMongoDbClientCallback* p_callback ){

		const char* sWho = "MongoDbClient::command";

		mongoc_client_t *p_client;
		mongoc_collection_t *p_collection;

		bson_t* p_bson_command;
		string s_bson_as_json;

		mongoc_cursor_t *p_cursor;
		bson_error_t bson_error;
		const bson_t *p_bson_doc;

		p_client = this->getPMongocClient();

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): Calling bson_new_from_json( \"" << s_json_command << "\" )..." << endl;
		}
		p_bson_command = bson_new_from_json( (const uint8_t *) s_json_command.c_str(), -1, &bson_error );

		if( ! p_bson_command ){
			ostringstream oss_out;
			oss_out << "Trouble converting json to bson: \"" << bson_error.message << "\"" << endl;
			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): " << oss_out.str() << "...tossing JDA::MongoDbClient::Exception()..." << endl;
			}
			throw JDA::MongoDbClient::Exception( oss_out.str() ); 
		}

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "Getting s_db_name = \"" << s_db_name << "\", s_collection_name = \"" << s_collection_name << "\"..." << endl;
		}
		// Note: mongoc_client_get_collection() cannot fail...collection is created if it doesn't yet exist...
		p_collection = mongoc_client_get_collection (p_client, s_db_name.c_str(), s_collection_name.c_str() );

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "Running db." << s_collection_name << ".command( " << this->bson_as_json_string( p_bson_command) << " )..." << endl;
		}

		p_cursor = mongoc_collection_command (p_collection,
                                    MONGOC_QUERY_NONE,
                                    0,
                                    0,
                                    0,
									p_bson_command,
                                    NULL,  /* Fields, NULL for all. */
                                    NULL /* Read Prefs, NULL for default */
									);

		int row_number = 0;

		while (mongoc_cursor_next (p_cursor, &p_bson_doc)) {

			row_number++;

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "[" << row_number << "]: json_string = " << this->bson_as_json_string( p_bson_doc ) << "\n" << endl; 
			}

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "[" << row_number << "]: pretty_json_string =\n" << this->bson_as_pretty_json_string( p_bson_doc ) << "\n" << endl; 
			}

			if( p_callback != NULL ){
				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "[" << row_number << "]: Callin' p_callback->documentRecieved( p_bson_doc )..." << endl;
				}
				p_callback->documentRecieved( p_bson_doc );
			}

		}/* while (mongoc_cursor_next (p_cursor, &p_bson_doc)) */

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << row_number << " doc(s) returned..." << endl;
		}

		if (mongoc_cursor_error (p_cursor, &bson_error)) {
			ostringstream oss_out;
			oss_out << "Cursor Failure: \"" << bson_error.message << "\"" << endl;
			throw JDA::MongoDbClient::Exception( oss_out.str() ); 
		}

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling bson_destroy( p_bson_command )..." << endl;
		}
   		bson_destroy(p_bson_command); 

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling mongoc_cursor_destroy( p_cursor )..." << endl;
		}
		mongoc_cursor_destroy (p_cursor);

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling mongoc_collection_destroy(p_collection)..." << endl;
		}
		mongoc_collection_destroy (p_collection);

		return row_number;

	}/* JDA::MongoDbClient::command() */

	int JDA::MongoDbClient::find( const string& s_db_name, const string& s_collection_name, const string& s_json_query ){

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
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << row_number << " doc(s) returned..." << endl;
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



	int JDA::MongoDbClient::insert( const string& s_db_name, const string& s_collection_name, const string& s_json_doc ){

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

		// New _id should be filled in for you now...
		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "After insert, p_bson_doc = " << this->bson_as_json_string( p_bson_doc ) << "..." << endl;
		}


		// Extra Credit: Use mongoc_collection_get_last_error() to ascertain _id of newly inserted document...or try to...
		// const bson_t * mongoc_collection_get_last_error (const mongoc_collection_t *collection);
		
		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "Running bson_last_insert = mongoc_collection_get_last_error( " << s_collection_name << " ) to view insert record..." << endl;
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


	int JDA::MongoDbClient::update( const string& s_db_name, const string& s_collection_name, const string& s_json_query, const string& s_json_update ){

		// http://api.mongodb.com/c/current/tutorial.html#update

		const char* sWho = "MongoDbClient::update";

  		mongoc_client_t *p_client = this->getPMongocClient();

	    bson_error_t bson_error;

		bson_t *p_bson_query;

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): Calling bson_new_from_json( s_json_query = \"" << s_json_query << "\" )..." << endl;
		}
		p_bson_query = bson_new_from_json( (const uint8_t *) s_json_query.c_str(), -1, &bson_error );

		if( ! p_bson_query ){
			ostringstream oss_out;
			oss_out << "Trouble converting s_json_query = \"" << s_json_query << "\" to bson: \"" << bson_error.message << "\"" << endl;
			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): " << oss_out.str() << "...tossing JDA::MongoDbClient::Exception()..." << endl;
			}
			throw JDA::MongoDbClient::Exception( oss_out.str() ); 
		}

		bson_t *p_bson_update;

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): Calling bson_new_from_json( s_json_update = \"" << s_json_update << "\" )..." << endl;
		}
		p_bson_update = bson_new_from_json( (const uint8_t *) s_json_update.c_str(), -1, &bson_error );

		if( ! p_bson_update ){

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling bson_destroy( p_bson_query )..." << endl;
			}
	   		bson_destroy(p_bson_query); 

			ostringstream oss_out;
			oss_out << "Trouble converting s_json_update = \"" << s_json_update << "\" to bson: \"" << bson_error.message << "\"" << endl;

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): " << oss_out.str() << "...tossing JDA::MongoDbClient::Exception()..." << endl;
			}
			throw JDA::MongoDbClient::Exception( oss_out.str() ); 
		}

	    mongoc_collection_t *p_collection;

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "Calling mongoc_client_get_collection( p_client, \"" << s_db_name << "\", \"" << s_collection_name << "\" )..." << endl;
		}

		// Note: mongoc_client_get_collection() cannot fail...collection is created if it doesn't yet exist...
		p_collection = mongoc_client_get_collection (p_client, s_db_name.c_str(), s_collection_name.c_str() );

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "Running db." << s_collection_name << ".update( " << this->bson_as_json_string( p_bson_query ) << ", " << this->bson_as_json_string( p_bson_update ) << " )..." << endl;
		}


		if (!mongoc_collection_update (p_collection, MONGOC_UPDATE_NONE, p_bson_query, p_bson_update, NULL, &bson_error)) {

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling bson_destroy( p_bson_query )..." << endl;
			}
	   		bson_destroy(p_bson_query); 

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling bson_destroy( p_bson_update )..." << endl;
			}
   			bson_destroy(p_bson_update); 

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling mongoc_collection_destroy(p_collection)..." << endl;
			}
			mongoc_collection_destroy (p_collection);

			ostringstream oss_out;
			oss_out << "Trouble updating: \"" << bson_error.message << "\"" << endl;
			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): " << oss_out.str() << "...tossing JDA::MongoDbClient::Exception()..." << endl;
			}
			throw JDA::MongoDbClient::Exception( oss_out.str() ); 
		}

		// Extra Credit: Use mongoc_collection_get_last_error() to ascertain _id of newly inserted document...or try to...
		// const bson_t * mongoc_collection_get_last_error (const mongoc_collection_t *collection);
		
		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "Running bson_last_update = mongoc_collection_get_last_error( " << s_collection_name << " ) to view update record..." << endl;
		}

		//const bson_t* p_bson_last_insert = mongoc_collection_get_last_error( p_collection ); 
		bson_t* p_bson_last_update = (bson_t * ) mongoc_collection_get_last_error( p_collection ); 

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "bson_as_json_string( p_bson_last_update ) = \"" << this->bson_as_json_string( p_bson_last_update ) << "\"..." << endl;
		}

		// Not a good idea...
		//if( m_p_logger ){
		//	(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling bson_destroy( p_bson_last_insert )..." << endl;
		//}
   		//bson_destroy(p_bson_last_insert ); 

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling bson_destroy( p_bson_query )..." << endl;
		}
   		bson_destroy(p_bson_query); 

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling bson_destroy( p_bson_update )..." << endl;
		}
   		bson_destroy(p_bson_update); 

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "Calling mongoc_collection_destroy(p_collection)..." << endl;
		}
		mongoc_collection_destroy (p_collection);

		return 1;

	}/* int JDA::MongoDbClient::update( const string& s_db_name, const string& s_collection_name, const string& s_json_query, const string& s_json_update ) */



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


