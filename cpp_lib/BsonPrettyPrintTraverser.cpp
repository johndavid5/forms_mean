#include "BsonPrettyPrintTraverser.h"

	string JDA::BsonPrettyPrintTraverser::bson_as_pretty_json_string( const bson_t* p_bson ){

		m_oss.str();

		m_oss << "{\n";

		traverse_doc( p_bson, NULL );

		m_oss << "\n}";

		return m_oss.str();

	}/* bson_as_pretty_json_string() */

	/* virtual */ void JDA::BsonPrettyPrintTraverser::keyee(const string& s_key, int i_level ){
		string s_indent_prefix = get_indent_prefix( i_level );
		m_oss << "\n" << s_indent_prefix << "\"" << s_key << "\" : ";
	}
	
	/* virtual */ void JDA::BsonPrettyPrintTraverser::subdoc_start( int i_level ){
		m_oss << " {\n";
	}

	/* virtual */ void JDA::BsonPrettyPrintTraverser::subdoc_end( int i_level ){
		m_oss << " },\n";
	}

	/* virtual */ void JDA::BsonPrettyPrintTraverser::array_start( int i_level ){
		m_oss << " [\n";
	}

	/* virtual */ void JDA::BsonPrettyPrintTraverser::array_end( int i_level ){
		m_oss << " ],\n";
	}
	
	/* virtual */ void JDA::BsonPrettyPrintTraverser::doublee(double d_double, int i_level ){
		m_oss << d_double << ",";
	}
	
	/* virtual */ void JDA::BsonPrettyPrintTraverser::utf8ee(const string& s_utf8, uint32_t i_length, int i_level ){
		m_oss << "\"" << s_utf8 << "\",";
	}
	
	/* virtual */ void JDA::BsonPrettyPrintTraverser::oidee(const string& s_oid, int i_level ){
		m_oss << "{ $oid: \"" << s_oid << "\" },";
	}
	
	/* virtual */ void JDA::BsonPrettyPrintTraverser::int32ee( int32_t int32, int i_level ){
		m_oss << int32 << ",";
	}

	/* virtual */ void JDA::BsonPrettyPrintTraverser::int64ee( int64_t int64, int i_level ){
		m_oss << int64 << ",";
	}

	/* virtual */ void JDA::BsonPrettyPrintTraverser::boolee( bool boolio, int i_level ){
		m_oss << std::boolalpha << boolio << ",";
	}

	/* virtual */ void JDA::BsonPrettyPrintTraverser::datetimee( int64_t num_millis_since_epoch, int i_level ){
		m_oss << "{ $date: \"" << num_millis_since_epoch << "\" },";
	}
