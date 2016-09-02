#ifndef _BSON_TRAVERSER_H_
#define _BSON_TRAVERSER_H_

#include "bson.h"
#include "logger.h"

namespace JDA
{


class BsonTraverser {

	public:
		BsonTraverser() : m_p_logger(NULL) {}

		virtual ~BsonTraverser(){}

		void setPLogger( JDA::Logger* p_logger ){
			m_p_logger = p_logger;
		}
	
		void traverse_doc( const bson_t* p_bson_doc, ostream* p_oss_out );

		string bson_as_pretty_json_string( const bson_t* p_bson );
	
		// BEGIN: OVERRIDE THESE FOR ELEMENT YOU WISH TO "CATCH"...
		virtual void keyee(const string& s_key, int i_level );
	
		virtual void subdoc_start( int i_level );
		virtual void subdoc_end( int i_level );
	
		virtual void array_start( int i_level );
		virtual void array_end( int i_level );
	
		virtual void doublee(double d_double, int i_level );
	
		virtual void utf8ee(const string& s_utf8, uint32_t i_length, int i_level );
	
		virtual void oidee(const string& s_oid, int i_level );
	
		virtual void int32ee( int32_t int32, int i_level );
		virtual void int64ee( int64_t int64, int i_level );

		virtual void boolee( bool boolio, int i_level );
		virtual void datetimee( int64_t num_millis_since_epoch, int i_level );
		
		// END: OVERRIDE THESE FOR ELEMENT YOU WISH TO "CATCH"...

	protected: 
		void traverse_iter( bson_iter_t* p_bson_iter, int i_level, ostream* p_oss_out );

		JDA::Logger* m_p_logger;

		static string m_indent_char;

		static string get_indent_prefix( int i_level ){

			ostringstream oss;

			for( int i = 0; i < i_level; i++ ){
				oss << m_indent_char;
			}

			return oss.str();
		} /* get_indent_prefix() */

		// ObjectId is a 12-byte BSON type, constructed using:
		// a 4-byte value representing the seconds since the Unix epoch (in Big Endian)
		// a 3-byte machine identifier
		// a 2-byte process id (Big Endian), and
		// a 3-byte counter (Big Endian), starting with a random value.
		struct oid_struct {
			char seconds_since_unix_epoch[4];
			char machine_id[3];
			char process_id[2];
			char counter[3];
		};

};/* class BsonTraverser */

} /* namespace JDA */

#endif /* #ifndef _BSON_TRAVERSER_H_ */
