#include "BsonFormParseTraverser.h"

namespace JDA {

	/* static */ const char* BsonFormParseTraverser::stateTypeEnumStrings[] = {
			"STATE_WITHOUT",

				"STATE_IN_CIK",

				"STATE_IN_FILERS",

					"STATE_IN_FILER",

						"STATE_IN_FILER_COMPANY_DATA",
							"STATE_IN_FILER_COMPANY_DATA_DOC",
								"STATE_IN_FILER_COMPANY_DATA_DOC_COMPANY_CONFORMED_NAME",
								"STATE_IN_FILER_COMPANY_DATA_DOC_CENTRAL_INDEX_KEY",
								"STATE_IN_FILER_COMPANY_DATA_DOC_STATE_OF_INCORPORATION",

						"STATE_IN_FILER_FILING_VALUES",
							"STATE_IN_FILER_FILING_VALUES_DOC",

						"STATE_IN_FILER_BUSINESS_ADDRESS",
							"STATE_IN_FILER_BUSINESS_ADDRESS_DOC",

						"STATE_IN_FILER_MAIL_ADDRESS",
							"STATE_IN_FILER_MAIL_ADDRESS_DOC"
	};



	int JDA::BsonFormParseTraverser::parse_it( const bson_t* p_bson ){

		_state = STATE_WITHOUT; 

		traverse_doc( p_bson, NULL );

		return 1;

	}/* parse_it() */

	/* virtual */ void BsonFormParseTraverser::keyee(const string& s_key, int i_level ){

		BsonTraverser::keyee(s_key, i_level );

		const char* sWho = "BsonFormParseTraverser::keyee";

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): BEFORE: _state = " << _state << " = " << stateTypeToString(_state) << "..." << endl;
		}

		switch( _state ){
		
			case STATE_WITHOUT:
				if( s_key.compare("cik") == 0 ){
					_state = STATE_IN_CIK;
				}
				else if( s_key.compare("filers") == 0 ){
					_state = STATE_IN_FILERS;
				}
				break;

			case STATE_IN_FILER:
				if( s_key.compare("company_data") == 0 ){
					_state = STATE_IN_FILER_COMPANY_DATA;
				}
				else if( s_key.compare("filing_values") == 0 ){
					_state = STATE_IN_FILER_FILING_VALUES;
				}
				else if( s_key.compare("business_address") == 0 ){
					_state = STATE_IN_FILER_BUSINESS_ADDRESS;
				}
				else if( s_key.compare("mail_address") == 0 ){
					_state = STATE_IN_FILER_MAIL_ADDRESS;
				}
				break;

			case STATE_IN_FILER_COMPANY_DATA_DOC:
				if( s_key.compare("company_conformed_name") == 0 ){ 
					_state = STATE_IN_FILER_COMPANY_DATA_DOC_COMPANY_CONFORMED_NAME;
				}
				else if( s_key.compare("central_index_key") == 0 ){ 
					_state = STATE_IN_FILER_COMPANY_DATA_DOC_CENTRAL_INDEX_KEY;
				}
				else if( s_key.compare("state_of_incorporation") == 0 ){ 
					_state = STATE_IN_FILER_COMPANY_DATA_DOC_STATE_OF_INCORPORATION;
				}
				break;

		}/* switch(_state) */

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): AFTER: _state = " << _state << " = " << stateTypeToString(_state) << "..." << endl;
		}

	}/* BsonFormParseTraverser::keyee() */
	
	/* virtual */ void BsonFormParseTraverser::subdoc_start( int i_level ){

		BsonTraverser::subdoc_start( i_level );

		const char* sWho = "BsonFormParseTraverser::subdoc_start";

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): BEFORE: _state = " << _state << " = " << stateTypeToString(_state) << "..." << endl;
		}

		switch( _state ){
		
			case STATE_IN_FILERS:
				_state = STATE_IN_FILER;
				break;

			case STATE_IN_FILER_COMPANY_DATA:
				_state = STATE_IN_FILER_COMPANY_DATA_DOC;
				break;

			case STATE_IN_FILER_FILING_VALUES:
				_state = STATE_IN_FILER_FILING_VALUES_DOC;
				break;

			case STATE_IN_FILER_BUSINESS_ADDRESS:
				_state = STATE_IN_FILER_BUSINESS_ADDRESS_DOC;
				break;

			case STATE_IN_FILER_MAIL_ADDRESS:
				_state = STATE_IN_FILER_MAIL_ADDRESS_DOC;
				break;

		}/* switch(_state) */

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): BEFORE: _state = " << _state << " = " << stateTypeToString(_state) << "..." << endl;
		}


	}/* BsonFormParseTraverser::subdoc_start() */


	/* virtual */ void BsonFormParseTraverser::subdoc_end( int i_level ){

		BsonTraverser::subdoc_end( i_level );

		const char* sWho = "BsonFormParseTraverser::subdoc_end";

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): BEFORE: _state = " << _state << " = " << stateTypeToString(_state) << "..." << endl;
		}

		switch( _state ){
		
			case STATE_IN_FILER:
				_state = STATE_IN_FILERS;
				break;

			case STATE_IN_FILER_COMPANY_DATA_DOC:
				_state = STATE_IN_FILER;
				break;

			case STATE_IN_FILER_FILING_VALUES_DOC:
				_state = STATE_IN_FILER;
				break;

			case STATE_IN_FILER_BUSINESS_ADDRESS_DOC:
				_state = STATE_IN_FILER;
				break;

			case STATE_IN_FILER_MAIL_ADDRESS_DOC:
				_state = STATE_IN_FILER;
				break;

		}/* switch(_state) */

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): AFTER: _state = " << _state << " = " << stateTypeToString(_state) << "..." << endl;
		}

	}/* void BsonFormParseTraverser::subdoc_end() */


	/* virtual */ void BsonFormParseTraverser::array_start( int i_level ){

		BsonTraverser::array_start( i_level );

	}/* BsonFormParseTraverser::array_start() */


	/* virtual */ void BsonFormParseTraverser::array_end( int i_level ){

		BsonTraverser::array_end( i_level );

		const char* sWho = "BsonFormParseTraverser::array_end";

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): BEFORE: _state = " << _state << " = " << stateTypeToString(_state) << "..." << endl;
		}

		switch( _state ){
		
			case STATE_IN_FILERS:
				_state = STATE_WITHOUT;
				break;

		}/* switch(_state) */

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): AFTER: _state = " << _state << " = " << stateTypeToString(_state) << "..." << endl;
		}

	}/* BsonFormParseTraverser::array_end( ) */
	
	/* virtual */ void BsonFormParseTraverser::doublee(double d_double, int i_level ){

		BsonTraverser::doublee( d_double, i_level );

	}
	
	/* virtual */ void BsonFormParseTraverser::utf8ee(const string& s_utf8, uint32_t i_length, int i_level ){

		BsonTraverser::utf8ee( s_utf8, i_length, i_level );

		const char* sWho = "BsonFormParseTraverser::utf8ee";

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): BEFORE: _state = " << _state << " = " << stateTypeToString(_state) << "..." << endl;
		}

		switch( _state ){

			case STATE_IN_FILER_COMPANY_DATA_DOC_CENTRAL_INDEX_KEY:
				_filer_company_data_central_index_key = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _filer_company_data_central_index_key = s_utf8 = \"" 
					 << _filer_company_data_central_index_key << "\"..." << endl;
				}

				_state = STATE_IN_FILER_COMPANY_DATA_DOC;
				break;

			case STATE_IN_FILER_COMPANY_DATA_DOC_COMPANY_CONFORMED_NAME:
				_filer_company_data_company_conformed_name = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _filer_company_data_company_conformed_name = s_utf8 = \"" 
					 << _filer_company_data_company_conformed_name << "\"..." << endl;
				}

				_state = STATE_IN_FILER_COMPANY_DATA_DOC;
				break;

			case STATE_IN_FILER_COMPANY_DATA_DOC_STATE_OF_INCORPORATION:
				_filer_company_data_state_of_incorporation = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _filer_company_data_state_of_incorporation = s_utf8 = \"" 
					 << _filer_company_data_state_of_incorporation << "\"..." << endl;
				}

				_state = STATE_IN_FILER_COMPANY_DATA_DOC;
				break;

		}/* switch( _state ) */

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): AFTER: _state = " << _state << " = " << stateTypeToString(_state) << "..." << endl;
		}

	}/* BsonFormParseTraverser::utf8ee() */
	
	/* virtual */ void BsonFormParseTraverser::oidee(const string& s_oid, int i_level ){
		BsonTraverser::oidee( s_oid, i_level );
	}
	
	/* virtual */ void BsonFormParseTraverser::int32ee( int32_t int32, int i_level ){

		BsonTraverser::int32ee( int32, i_level );

		const char* sWho = "BsonFormParseTraverser::int32ee";

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): BEFORE: _state = " << _state << " = " << stateTypeToString(_state) << "..." << endl;
		}

		switch( _state ){
		
			case STATE_IN_CIK:
				_cik = int32;
				_state = STATE_WITHOUT;
				break;

		}/* switch(_state) */

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): AFTER: _state = " << _state << " = " << stateTypeToString(_state) << "..." << endl;
		}
	}/* BsonFormParseTraverser::int32ee() */

	/* virtual */ void BsonFormParseTraverser::int64ee( int64_t int64, int i_level ){
		BsonTraverser::int64ee( int64, i_level );
	}

	/* virtual */ void BsonFormParseTraverser::boolee( bool boolio, int i_level ){
		BsonTraverser::boolee( boolio, i_level );
	}

	/* virtual */ void BsonFormParseTraverser::datetimee( int64_t num_millis_since_epoch, int i_level ){
		BsonTraverser::datetimee( num_millis_since_epoch, i_level );
	}

} /* namespace JDA { */

ostream& operator<<(ostream& out, const JDA::BsonFormParseTraverser& traverser ){
	traverser.print(out);
	return out;
}
