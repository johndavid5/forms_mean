#ifndef _BSON_PRETTY_PRINT_TRAVERSER_H_
#define _BSON_PRETTY_PRINT_TRAVERSER_H_

#include "BsonTraverser.h"

namespace JDA {

class BsonPrettyPrintTraverser : public BsonTraverser {

	public:
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

	protected:
		ostringstream m_oss;

};

} /* namespace JDA { */

#endif /* #ifndef _BSON_PRETTY_PRINT_TRAVERSER_H_ */
