#include "BsonTraverser.h"

	/* static */ /* string JDA::BsonTraverser::m_indent_char = " "; */
	/* static */ string JDA::BsonTraverser::m_indent_char = "\t";

	string JDA::BsonTraverser::bson_as_pretty_json_string( const bson_t* p_bson ){

		ostringstream oss_out;

		oss_out << "{\n";

		traverse_doc( p_bson, &oss_out );

		oss_out << "\n}";

		return oss_out.str();

	}/* bson_as_pretty_json_string() */

	void JDA::BsonTraverser::traverse_doc( const bson_t* p_bson_doc, ostream* p_oss_out ){

		bson_iter_t iter;

		if (bson_iter_init (&iter, p_bson_doc)) {

			traverse_iter( &iter, 1, p_oss_out );

		}/* if (bson_iter_init (&iter, p_bson_doc)) */
	}

	/* virtual */ void JDA::BsonTraverser::keyee(const string& s_key, int i_level ){

		const char* sWho = "JDA::BsonTraverser::keyee";

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): i_level = " << i_level << ", s_key = \"" << s_key << "\"..." << endl;
		}
	}

	/* virtual */ void JDA::BsonTraverser::subdoc_start( int i_level ){

		const char* sWho = "JDA::BsonTraverser::subdoc_start";

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): i_level = " << i_level << "..." << endl;
		}
	}

	/* virtual */ void JDA::BsonTraverser::subdoc_end( int i_level ){

		const char* sWho = "JDA::BsonTraverser::subdoc_end";

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): i_level = " << i_level << "..." << endl;
		}
	}

	/* virtual */ void JDA::BsonTraverser::array_start( int i_level ){
		const char* sWho = "JDA::BsonTraverser::array_start";

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): i_level = " << i_level << "..." << endl;
		}
	}

	/* virtual */ void JDA::BsonTraverser::array_end( int i_level ){
		const char* sWho = "JDA::BsonTraverser::array_end";

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): i_level = " << i_level << "..." << endl;
		}
	}

	/* virtual */ void JDA::BsonTraverser::doublee(double d_double, int i_level ){

		const char* sWho = "JDA::BsonTraverser::double";

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): i_level = " << i_level << ", d_double = " << d_double << "..." << endl;
		}
	}

	/* virtual */ void JDA::BsonTraverser::utf8ee(const string& s_utf8, uint32_t i_length, int i_level ){

		const char* sWho = "JDA::BsonTraverser::utf8ee";

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): i_level = " << i_level << ", i_length = " << i_length << ", s_utf8 = \"" << s_utf8 << "\"..." << endl;
		}
	}

	/* virtual */ void JDA::BsonTraverser::oidee(const string& s_oid, int i_level ){

		const char* sWho = "JDA::BsonTraverser::oidee";

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): i_level = " << i_level << ", s_oid = \"" << s_oid << "\"..." << endl;
		}
	}

	/* virtual */ void JDA::BsonTraverser::int32ee( int32_t int32, int i_level ){

		const char* sWho = "JDA::BsonTraverser::int32";

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): i_level = " << i_level << ", int32 = " << int32 << "..." << endl;
		}
	}

	/* virtual */ void JDA::BsonTraverser::int64ee( int64_t int64, int i_level ){

		const char* sWho = "JDA::BsonTraverser::int64ee";

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): i_level = " << i_level << ", int64 = " << int64 << "..." << endl;
		}
	}

	/* virtual */ void JDA::BsonTraverser::boolee( bool boolio, int i_level ){

		const char* sWho = "JDA::BsonTraverser::boolee";

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): i_level = " << i_level << ", boolee = " << std::boolalpha << boolio << "..." << endl;
		}
	}

	/* virtual */ void JDA::BsonTraverser::datetimee( int64_t num_millis_since_epoch, int i_level ){

		const char* sWho = "JDA::BsonTraverser::datetimee";

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): i_level = " << i_level << ", num_millis_since_epoch = " << num_millis_since_epoch << "..." << endl;
		}
	}

	void JDA::BsonTraverser::traverse_iter( bson_iter_t* p_bson_iter, int i_level, ostream* p_oss_out ){

		const char* sWho = "JDA::BsonTraverser::traverse_iter";

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << "..." << endl;
		}

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
					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": found a field with key \"" << key << "\"..." << endl;
					}

					if( p_oss_out ){	
						//(*p_oss_out) << s_indent_prefix << "\"" << key << "\" : ";
						(*p_oss_out) << "\n" << s_indent_prefix << "\"" << key << "\" : ";
					}

					keyee( key, i_level );
				}
				else {
					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": found a field \"" << key << "\" that is not a key..." << endl;
					}
				}

				// http://cpansearch.perl.org/src/MONGODB/MongoDB-v1.4.2/bson/bson-iter.h

				if( BSON_ITER_HOLDS_DOCUMENT( p_bson_iter ) ){

					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_DOCUMENT..." << endl;
					}

					subdoc_start( i_level );

					if( p_oss_out ){	
						//(*p_oss_out) << s_indent_prefix << " { \n";
						(*p_oss_out) << " {\n";
					}

					if( bson_iter_recurse(p_bson_iter, &child) ){
						if( m_p_logger ){
							(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << " j = " << j << ": found a sub-document child via bson_iter_recurse()...traversing it via traverse_iter()..." << endl;
						}

						traverse_iter( &child, i_level + 1, p_oss_out );

						subdoc_end( i_level );

						if( p_oss_out ){	
							//(*p_oss_out) << s_indent_prefix << " },\n";
							(*p_oss_out) << " },\n";
						}
					}


				}/* if( BSON_ITER_HOLDS_DOCUMENT( &iter ) */
				else if( BSON_ITER_HOLDS_ARRAY( p_bson_iter ) ){

					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_ARRAY..." << endl;
					}

					array_start( i_level );

					if( p_oss_out ){	
						//(*p_oss_out) << s_indent_prefix << " [\n";
						(*p_oss_out) << "[\n";
					}

					if( bson_iter_recurse(p_bson_iter, &child) ){
						if( m_p_logger ){
							(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << " j = " << j << ": found an array-element child via bson_iter_recurse()...traversing it via bson_traverse_iter()..." << endl;
						}

						traverse_iter( &child, i_level + 1, p_oss_out );

						array_end( i_level );

						if( p_oss_out ){	
							(*p_oss_out) << s_indent_prefix << " ],\n";
						}
					}

				}/* if( BSON_ITER_HOLDS_ARRAY( &iter ) */
				else if( BSON_ITER_HOLDS_DOUBLE( p_bson_iter ) ){
					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_DOUBLE..." << endl;
					}
					double le_double = bson_iter_double( p_bson_iter );  


					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": le_double = " << le_double << "..." << endl;
					}

					doublee( le_double, i_level );

					if( p_oss_out ){	
						//(*p_oss_out) << s_indent_prefix << " " << int32 << ",\n";
						//(*p_oss_out) << "" << le_double << ",\n";
						(*p_oss_out) << "" << le_double << ",";
					}

				}
				else if( BSON_ITER_HOLDS_UTF8( p_bson_iter ) ){

					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_UTF8..." << endl;
					}

					utf8 = bson_iter_utf8( p_bson_iter, &utf8_len );


					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": utf8_len = " << utf8_len << "..." << endl;
					}

					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": utf8 = \"" << utf8 << "\"..." << endl;
					}

					utf8ee( (const string&)utf8, utf8_len, i_level );

					if( p_oss_out ){	
						//(*p_oss_out) << s_indent_prefix << " \"" << utf8 << "\",\n";
						//(*p_oss_out) << "\"" << utf8 << "\",\n";
						(*p_oss_out) << "\"" << utf8 << "\",";
					}

				}/* if( BSON_ITER_HOLDS_UTF8( &iter ) */
				else if( BSON_ITER_HOLDS_BINARY( p_bson_iter ) ){

					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_BINARY..." << endl;
					}
				}
				else if( BSON_ITER_HOLDS_UNDEFINED( p_bson_iter ) ){
					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_UNDEFINED..." << endl;
					}
				}
				else if( BSON_ITER_HOLDS_OID( p_bson_iter ) ){

					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_OID..." << endl;
					}

					p_bson_oid = bson_iter_oid( p_bson_iter );
					bson_oid_to_string( p_bson_oid, bson_oid_str );

					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": bson_oid_str = \"" << bson_oid_str << "\"..." << endl;
					}

					oidee( bson_oid_str, i_level ); 

					if( p_oss_out ){	
						//(*p_oss_out) << s_indent_prefix << " { $oid: \"" << bson_oid_str << "\" },\n";
						//(*p_oss_out) << "{ $oid: \"" << bson_oid_str << "\" },\n";
						(*p_oss_out) << "{ $oid: \"" << bson_oid_str << "\" },";
					}

					//uint8_t le_byte;
					////unsigned int le_byte;

					//for( size_t i = 0; i < sizeof(p_bson_oid->bytes)/sizeof(uint8_t); i++ ){
					//	le_byte = p_bson_oid->bytes[i];
					//
					//	if( m_p_logger ){
					//		(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": p_bson_oid->bytes[" << setw(2) << setfill('0') << i << "] = 0x" << setfill('0') << setw(2) << hex << le_byte << " = " << dec << le_byte << "..." << endl;
					//	}
					//}

					#ifdef WIN32 
						//DWORD WINAPI GetCurrentProcessId(void);
						DWORD our_process_id = GetCurrentProcessId();

						if( m_p_logger ){
							(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": [WIN32] GetCurrentProcessId() = " << "0x" << hex << setfill('0') << setw(sizeof(our_process_id)*2) << our_process_id << " = " << dec << our_process_id << "..." << endl;
						}
					#endif

//						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": sizeof(oid_struct) = " << sizeof(oid_struct) << "..." << endl;
//						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": sizeof(p_bson_oid->bytes) = " << sizeof(p_bson_oid->bytes) << "..." << endl;
//	
//						oid_struct our_oid_struct;
//						memcpy( &our_oid_struct, p_bson_oid->bytes, sizeof( p_bson_oid->bytes) );
//	
//						time_t seconds_since_unix_epoch;
//						memcpy( &seconds_since_unix_epoch, our_oid_struct.seconds_since_unix_epoch, sizeof(our_oid_struct.seconds_since_unix_epoch)  );
//	
//						unsigned int machine_id;
//						memcpy( &machine_id, our_oid_struct.machine_id, sizeof(our_oid_struct.machine_id)  );
//	
//						unsigned int process_id;
//						memcpy( &process_id, our_oid_struct.process_id, sizeof(our_oid_struct.process_id)  );
//	
//						unsigned int counter;
//						memcpy( &counter, our_oid_struct.counter, sizeof(our_oid_struct.counter)  );
//	
//						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": seconds_since_unix_epoch = 0x" << hex << setw(sizeof(seconds_since_unix_epoch)*2) << setfill('0') << seconds_since_unix_epoch << " = " << dec << seconds_since_unix_epoch << "..." << endl;
//	
//						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": machine_id = 0x" << hex << setw(sizeof(machine_id)*2) << setfill('0') << machine_id << " = " << dec << machine_id << "..." << endl;
//	
//						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": process_id = 0x" << hex << setw(sizeof(process_id)*2) << setfill('0') << process_id << " = " << dec << process_id << "..." << endl;
//	
//						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": counter = 0x" << hex << setw(sizeof(counter)*2) << setfill('0') << counter << " = " << dec << counter << "..." << endl;
//	
					
					time_t oid_unix_time = bson_oid_get_time_t( p_bson_oid );

					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": oid_unix_time = " << hex << "0x" << oid_unix_time << " = " << dec << oid_unix_time << " = \"" << JDA::Utils::get_nyc_pretty_timestamp(oid_unix_time) << "\"..." << endl;
					}

				}/* if( BSON_ITER_HOLDS_OID( &iter ) */
				else if( BSON_ITER_HOLDS_BOOL( p_bson_iter ) ){
					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_BOOL..." << endl;
					}

					bool boolio = bson_iter_bool( p_bson_iter );

					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": boolio = " << std::boolalpha << boolio << "..." << endl;
					}

					boolee( boolio, i_level );

					if( p_oss_out ){	
						//(*p_oss_out) << s_indent_prefix << " " << boolio << ",\n";
						//(*p_oss_out) << "" << boolio << ",\n";
						(*p_oss_out) << "" << std::boolalpha << boolio << ",";
					}

				}
				else if( BSON_ITER_HOLDS_DATE_TIME( p_bson_iter ) ){
					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_DATE_TIME..." << endl;
					}

					int64_t num_millis_since_epoch = bson_iter_date_time( p_bson_iter );

					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": num_millis_since_epoch = " << num_millis_since_epoch << "..." << endl;
					}

					datetimee( num_millis_since_epoch, i_level );

					if( p_oss_out ){	
						//(*p_oss_out) << s_indent_prefix << " { $date: \"" << num_millis_since_epoch << "\" },\n";
						//(*p_oss_out) << "{ $date: \"" << num_millis_since_epoch << "\" },\n";
						(*p_oss_out) << "{ $date: \"" << num_millis_since_epoch << "\" },";
					}
				}
				else if( BSON_ITER_HOLDS_NULL( p_bson_iter ) ){

					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_NULL..." << endl;
					}
				}
				else if( BSON_ITER_HOLDS_REGEX( p_bson_iter ) ){

					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_REGEX..." << endl;
					}
				}
				else if( BSON_ITER_HOLDS_DBPOINTER( p_bson_iter ) ){

					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_DBPOINTER..." << endl;
					}
				}
				else if( BSON_ITER_HOLDS_CODE( p_bson_iter ) ){

					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_CODE..." << endl;
					}
				}
				else if( BSON_ITER_HOLDS_SYMBOL( p_bson_iter ) ){

					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_SYMBOL..." << endl;
					}
				}
				else if( BSON_ITER_HOLDS_CODEWSCOPE( p_bson_iter ) ){

					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_CODEWSCOPE..." << endl;
					}
				}
				else if( BSON_ITER_HOLDS_INT32( p_bson_iter ) ){

					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_INT32..." << endl;
					}

					int32 = bson_iter_int32( p_bson_iter );

					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": int32 = \"" << int32 << "\"..." << endl;
					}

					int32ee( int32, i_level );

					if( p_oss_out ){	
						//(*p_oss_out) << s_indent_prefix << " " << int32 << ",\n";
						//(*p_oss_out) << "" << int32 << ",\n";
						(*p_oss_out) << "" << int32 << ",";
					}

				}/* if( BSON_ITER_HOLDS_INT32( &iter ) */
				else if( BSON_ITER_HOLDS_TIMESTAMP( p_bson_iter ) ){

					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_TIMESTAMP..." << endl;
					}
				}
				else if( BSON_ITER_HOLDS_INT64( p_bson_iter ) ){

					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_INT64..." << endl;
					}

					int64 = bson_iter_int64( p_bson_iter );

					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": int64 = \"" << int64 << "\"..." << endl;
					}

					int64ee( int64, i_level );

					if( p_oss_out ){	
						//(*p_oss_out) << s_indent_prefix << " " << int64 << ",\n";
						//(*p_oss_out) << "" << int64 << ",\n";
						(*p_oss_out) << "" << int64 << ",";
					}

				}/* if( BSON_ITER_HOLDS_INT64( &iter ) */
				else if( BSON_ITER_HOLDS_MAXKEY( p_bson_iter ) ){

					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_MAXKEY..." << endl;
					}
				}
				else if( BSON_ITER_HOLDS_MINKEY( p_bson_iter ) ){
					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): i_level = " << i_level << ": j = " << j << ": BSON_ITER_HOLDS_MINKEY..." << endl;
					}
				}

			}/* while (bson_iter_next (p_bson_iter)) */

	}/* bson_traverse_iter() */


