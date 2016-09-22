#include "BsonFormParseTraverser.h"

namespace JDA {

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
				else if( s_key.compare("issuer") == 0 ){
					_state = STATE_IN_ISSUER;
				}
				else if( s_key.compare("filed_by") == 0 ){
					// Just pretend we're in ISSUER even though we're in FILED_BY...
					_state = STATE_IN_ISSUER;
				}
				else if( s_key.compare("reporting_owner") == 0 ){
					_state = STATE_IN_REPORTING_OWNER;
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
				else if( s_key.compare("standard_industrial_classification") == 0 ){ 
					_state = STATE_IN_FILER_COMPANY_DATA_DOC_STANDARD_INDUSTRIAL_CLASSIFICATION;
				}
				else if( s_key.compare("state_of_incorporation") == 0 ){ 
					_state = STATE_IN_FILER_COMPANY_DATA_DOC_STATE_OF_INCORPORATION;
				}
				break;

			case STATE_IN_FILER_BUSINESS_ADDRESS_DOC:
				if( s_key.compare("street_1") == 0 ){ 
					_state = STATE_IN_FILER_BUSINESS_ADDRESS_DOC_STREET_1;
				}
				else if( s_key.compare("street_2") == 0 ){ 
					_state = STATE_IN_FILER_BUSINESS_ADDRESS_DOC_STREET_2;
				}
				else if( s_key.compare("city") == 0 ){ 
					_state = STATE_IN_FILER_BUSINESS_ADDRESS_DOC_CITY;
				}
				else if( s_key.compare("state") == 0 ){ 
					_state = STATE_IN_FILER_BUSINESS_ADDRESS_DOC_STATE;
				}
				else if( s_key.compare("zip") == 0 ){ 
					_state = STATE_IN_FILER_BUSINESS_ADDRESS_DOC_ZIP;
				}
				else if( s_key.compare("business_phone") == 0 ){ 
					_state = STATE_IN_FILER_BUSINESS_ADDRESS_DOC_BUSINESS_PHONE;
				}
				break;

			////
			
			case STATE_IN_ISSUER:
				if( s_key.compare("company_data") == 0 ){
					_state = STATE_IN_ISSUER_COMPANY_DATA;
				}
				else if( s_key.compare("filing_values") == 0 ){
					_state = STATE_IN_ISSUER_FILING_VALUES;
				}
				else if( s_key.compare("business_address") == 0 ){
					_state = STATE_IN_ISSUER_BUSINESS_ADDRESS;
				}
				else if( s_key.compare("mail_address") == 0 ){
					_state = STATE_IN_ISSUER_MAIL_ADDRESS;
				}
				break;

			case STATE_IN_ISSUER_COMPANY_DATA_DOC:
				if( s_key.compare("company_conformed_name") == 0 ){ 
					_state = STATE_IN_ISSUER_COMPANY_DATA_DOC_COMPANY_CONFORMED_NAME;
				}
				else if( s_key.compare("central_index_key") == 0 ){ 
					_state = STATE_IN_ISSUER_COMPANY_DATA_DOC_CENTRAL_INDEX_KEY;
				}
				else if( s_key.compare("standard_industrial_classification") == 0 ){ 
					_state = STATE_IN_ISSUER_COMPANY_DATA_DOC_STANDARD_INDUSTRIAL_CLASSIFICATION;
				}
				else if( s_key.compare("state_of_incorporation") == 0 ){ 
					_state = STATE_IN_ISSUER_COMPANY_DATA_DOC_STATE_OF_INCORPORATION;
				}
				break;

			case STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC:
				if( s_key.compare("street_1") == 0 ){ 
					_state = STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_STREET_1;
				}
				else if( s_key.compare("street_2") == 0 ){ 
					_state = STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_STREET_2;
				}
				else if( s_key.compare("city") == 0 ){ 
					_state = STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_CITY;
				}
				else if( s_key.compare("state") == 0 ){ 
					_state = STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_STATE;
				}
				else if( s_key.compare("zip") == 0 ){ 
					_state = STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_ZIP;
				}
				else if( s_key.compare("business_phone") == 0 ){ 
					_state = STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_BUSINESS_PHONE;
				}
				break;

			////
			
			case STATE_IN_REPORTING_OWNER:
				if( s_key.compare("owner_data") == 0 ){
					_state = STATE_IN_REPORTING_OWNER_OWNER_DATA;
				}
				else if( s_key.compare("filing_values") == 0 ){
					_state = STATE_IN_REPORTING_OWNER_FILING_VALUES;
				}
				else if( s_key.compare("business_address") == 0 ){
					_state = STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS;
				}
				else if( s_key.compare("mail_address") == 0 ){
					_state = STATE_IN_REPORTING_OWNER_MAIL_ADDRESS;
				}
				break;

			case STATE_IN_REPORTING_OWNER_OWNER_DATA_DOC:
				if( s_key.compare("company_conformed_name") == 0 ){ 
					_state = STATE_IN_REPORTING_OWNER_OWNER_DATA_DOC_COMPANY_CONFORMED_NAME;
				}
				else if( s_key.compare("central_index_key") == 0 ){ 
					_state = STATE_IN_REPORTING_OWNER_OWNER_DATA_DOC_CENTRAL_INDEX_KEY;
				}
				break;

			case STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS_DOC:
				if( s_key.compare("street_1") == 0 ){ 
					_state = STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS_DOC_STREET_1;
				}
				else if( s_key.compare("street_2") == 0 ){ 
					_state = STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS_DOC_STREET_2;
				}
				else if( s_key.compare("city") == 0 ){ 
					_state = STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS_DOC_CITY;
				}
				else if( s_key.compare("state") == 0 ){ 
					_state = STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS_DOC_STATE;
				}
				else if( s_key.compare("zip") == 0 ){ 
					_state = STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS_DOC_ZIP;
				}
				else if( s_key.compare("business_phone") == 0 ){ 
					_state = STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS_DOC_BUSINESS_PHONE;
				}
				break;

			case STATE_IN_REPORTING_OWNER_MAIL_ADDRESS_DOC:
				if( s_key.compare("street_1") == 0 ){ 
					_state = STATE_IN_REPORTING_OWNER_MAIL_ADDRESS_DOC_STREET_1;
				}
				else if( s_key.compare("street_2") == 0 ){ 
					_state = STATE_IN_REPORTING_OWNER_MAIL_ADDRESS_DOC_STREET_2;
				}
				else if( s_key.compare("city") == 0 ){ 
					_state = STATE_IN_REPORTING_OWNER_MAIL_ADDRESS_DOC_CITY;
				}
				else if( s_key.compare("state") == 0 ){ 
					_state = STATE_IN_REPORTING_OWNER_MAIL_ADDRESS_DOC_STATE;
				}
				else if( s_key.compare("zip") == 0 ){ 
					_state = STATE_IN_REPORTING_OWNER_MAIL_ADDRESS_DOC_ZIP;
				}
				//else if( s_key.compare("business_phone") == 0 ){ 
				//	_state = STATE_IN_REPORTING_OWNER_MAIL_ADDRESS_DOC_BUSINESS_PHONE;
				//}
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

			////

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

			////

			case STATE_IN_ISSUER_COMPANY_DATA:
				_state = STATE_IN_ISSUER_COMPANY_DATA_DOC;
				break;

			case STATE_IN_ISSUER_FILING_VALUES:
				_state = STATE_IN_ISSUER_FILING_VALUES_DOC;
				break;

			case STATE_IN_ISSUER_BUSINESS_ADDRESS:
				_state = STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC;
				break;

			case STATE_IN_ISSUER_MAIL_ADDRESS:
				_state = STATE_IN_ISSUER_MAIL_ADDRESS_DOC;
				break;

			////

			case STATE_IN_REPORTING_OWNER_OWNER_DATA:
				_state = STATE_IN_REPORTING_OWNER_OWNER_DATA_DOC;
				break;

			case STATE_IN_REPORTING_OWNER_FILING_VALUES:
				_state = STATE_IN_REPORTING_OWNER_FILING_VALUES_DOC;
				break;

			case STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS:
				_state = STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS_DOC;
				break;

			case STATE_IN_REPORTING_OWNER_MAIL_ADDRESS:
				_state = STATE_IN_REPORTING_OWNER_MAIL_ADDRESS_DOC;
				break;

		}/* switch(_state) */

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): AFTER: _state = " << _state << " = " << stateTypeToString(_state) << "..." << endl;
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
				{
					_state = STATE_IN_FILERS;
					// We've reached the end of a filer...check to see if its central_index_key matches the top level
					// _cik...if so, copy its data into the "company" fields...
					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): SHEMP: Looks like the end of a \"filer\", Moe...let's see if its central_index_key matches the top-level _cik..." << endl;
					}
					int i_filer_central_index_key = JDA::Utils::stringToInt( _filer_company_data_central_index_key, -1 );	
					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): SHEMP: i_filer_central_index_key = " << i_filer_central_index_key << "..." << endl;
					}
					if( i_filer_central_index_key == this->_cik ){
						if( m_p_logger ){
							(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): SHEMP: They're a match, Moe...copyin' over to dhe \"_company\" fields..." << endl;
						}
	
						_company_company_data_central_index_key = _filer_company_data_central_index_key; 
						_company_company_data_company_conformed_name = _filer_company_data_company_conformed_name; 
						_company_company_data_standard_industrial_classification =_filer_company_data_standard_industrial_classification;
						_company_company_data_state_of_incorporation =_filer_company_data_state_of_incorporation;
						_company_business_address_street_1 = _filer_business_address_street_1;
						_company_business_address_street_2 = _filer_business_address_street_2;
						_company_business_address_city = _filer_business_address_city;
						_company_business_address_state = _filer_business_address_state;
						_company_business_address_zip = _filer_business_address_zip;
						_company_business_address_business_phone = _filer_business_address_business_phone;
	
					}
					else {
						if( m_p_logger ){
							(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): SHEMP: Sorry, They ain't a match, Moe...not copyin' nuttin' to nuttin'..." << endl;
						}
					}
				}
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

			///
			
			case STATE_IN_ISSUER:
				{
					_state = STATE_WITHOUT;
					// We've reached the end of an issuer...check to see if its central_index_key matches the top level
					// _cik...if so, copy its data into the "company" fields...
					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): SHEMP: Looks like the end of an \"issuer\", Moe...let's see if its central_index_key matches the top-level _cik..." << endl;
					}
					int i_issuer_central_index_key = JDA::Utils::stringToInt( _issuer_company_data_central_index_key, -1 );	
					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): SHEMP: i_issuer_central_index_key = " << i_issuer_central_index_key << "..." << endl;
					}
					if( i_issuer_central_index_key == this->_cik ){
						if( m_p_logger ){
							(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): SHEMP: They're a match, Moe...copyin' over to dhe \"_company\" fields..." << endl;
						}
	
						_company_company_data_central_index_key = _issuer_company_data_central_index_key; 
						_company_company_data_company_conformed_name = _issuer_company_data_company_conformed_name; 
						_company_company_data_standard_industrial_classification =_issuer_company_data_standard_industrial_classification;
						_company_company_data_state_of_incorporation =_issuer_company_data_state_of_incorporation;
						_company_business_address_street_1 = _issuer_business_address_street_1;
						_company_business_address_street_2 = _issuer_business_address_street_2;
						_company_business_address_city = _issuer_business_address_city;
						_company_business_address_state = _issuer_business_address_state;
						_company_business_address_zip = _issuer_business_address_zip;
						_company_business_address_business_phone = _issuer_business_address_business_phone;
	
					}
					else {
						if( m_p_logger ){
							(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): SHEMP: Sorry, They ain't a match, Moe...not copyin' nuttin' to nuttin'..." << endl;
						}
					}
				}
				break;

			case STATE_IN_ISSUER_COMPANY_DATA_DOC:
				_state = STATE_IN_ISSUER;
				break;

			case STATE_IN_ISSUER_FILING_VALUES_DOC:
				_state = STATE_IN_ISSUER;
				break;

			case STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC:
				_state = STATE_IN_ISSUER;
				break;

			case STATE_IN_ISSUER_MAIL_ADDRESS_DOC:
				_state = STATE_IN_ISSUER;
				break;


			case STATE_IN_REPORTING_OWNER:
				{
					_state = STATE_WITHOUT;
					// We've reached the end of an REPORTING_OWNER...check to see if its central_index_key matches the top level
					// _cik...if so, copy its data into the "company" fields...
					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): SHEMP: Looks like the end of an \"REPORTING_OWNER\", Moe...let's see if its central_index_key matches the top-level _cik..." << endl;
					}
					int i_reporting_owner_central_index_key = JDA::Utils::stringToInt( _reporting_owner_owner_data_central_index_key, -1 );	
					if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): SHEMP: i_reporting_owner_central_index_key = " << i_reporting_owner_central_index_key << "..." << endl;
					}
					if( i_reporting_owner_central_index_key == this->_cik ){
						if( m_p_logger ){
							(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): SHEMP: They're a match, Moe...copyin' over to dhe \"_company\" fields..." << endl;
						}
	
						_company_company_data_central_index_key = _reporting_owner_owner_data_central_index_key; 
						_company_company_data_company_conformed_name = _reporting_owner_owner_data_company_conformed_name; 
						//_owner_owner_data_standard_industrial_classification =_reporting_owner_owner_data_standard_industrial_classification;
						//_owner_owner_data_state_of_incorporation =_reporting_owner_owner_data_state_of_incorporation;
						if( _reporting_owner_business_address_street_1.length() > 0 ){
							_company_business_address_street_1 = _reporting_owner_business_address_street_1;
							_company_business_address_street_2 = _reporting_owner_business_address_street_2;
							_company_business_address_city = _reporting_owner_business_address_city;
							_company_business_address_state = _reporting_owner_business_address_state;
							_company_business_address_zip = _reporting_owner_business_address_zip;
							_company_business_address_business_phone = _reporting_owner_business_address_business_phone;
						}
						else if( _reporting_owner_mail_address_street_1.length() > 0 ){
							_company_business_address_street_1 = _reporting_owner_mail_address_street_1;
							_company_business_address_street_2 = _reporting_owner_mail_address_street_2;
							_company_business_address_city = _reporting_owner_mail_address_city;
							_company_business_address_state = _reporting_owner_mail_address_state;
							_company_business_address_zip = _reporting_owner_mail_address_zip;
							//_company_business_address_business_phone = _reporting_owner_mail_address_business_phone;
						}
	
					}
					else {
						if( m_p_logger ){
							(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): SHEMP: Sorry, They ain't a match, Moe...not copyin' nuttin' to nuttin'..." << endl;
						}
					}
				}
				break;

			case STATE_IN_REPORTING_OWNER_OWNER_DATA_DOC:
				_state = STATE_IN_REPORTING_OWNER;
				break;

			case STATE_IN_REPORTING_OWNER_FILING_VALUES_DOC:
				_state = STATE_IN_REPORTING_OWNER;
				break;

			case STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS_DOC:
				_state = STATE_IN_REPORTING_OWNER;
				break;

			case STATE_IN_REPORTING_OWNER_MAIL_ADDRESS_DOC:
				_state = STATE_IN_REPORTING_OWNER;
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

			case STATE_IN_FILER_COMPANY_DATA_DOC_STANDARD_INDUSTRIAL_CLASSIFICATION:
				_filer_company_data_standard_industrial_classification = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _filer_company_data_standard_industrial_classification = s_utf8 = \"" 
					 << _filer_company_data_standard_industrial_classification << "\"..." << endl;
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

			case STATE_IN_FILER_BUSINESS_ADDRESS_DOC_STREET_1:
				_filer_business_address_street_1 = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _filer_business_address_street_1 = s_utf8 = \"" 
					 << _filer_business_address_street_1 << "\"..." << endl;
				}

				_state = STATE_IN_FILER_BUSINESS_ADDRESS_DOC;
				break;

			case STATE_IN_FILER_BUSINESS_ADDRESS_DOC_STREET_2:
				_filer_business_address_street_2 = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _filer_business_address_street_2 = s_utf8 = \"" 
					 << _filer_business_address_street_2 << "\"..." << endl;
				}

				_state = STATE_IN_FILER_BUSINESS_ADDRESS_DOC;
				break;

			case STATE_IN_FILER_BUSINESS_ADDRESS_DOC_CITY:
				_filer_business_address_city = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _filer_business_address_city = s_utf8 = \"" 
					 << _filer_business_address_city << "\"..." << endl;
				}

				_state = STATE_IN_FILER_BUSINESS_ADDRESS_DOC;
				break;

			case STATE_IN_FILER_BUSINESS_ADDRESS_DOC_STATE:
				_filer_business_address_state = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _filer_business_address_state = s_utf8 = \"" 
					 << _filer_business_address_state << "\"..." << endl;
				}

				_state = STATE_IN_FILER_BUSINESS_ADDRESS_DOC;
				break;

			case STATE_IN_FILER_BUSINESS_ADDRESS_DOC_ZIP:
				_filer_business_address_zip = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _filer_business_address_zip = s_utf8 = \"" 
					 << _filer_business_address_zip << "\"..." << endl;
				}

				_state = STATE_IN_FILER_BUSINESS_ADDRESS_DOC;
				break;

			case STATE_IN_FILER_BUSINESS_ADDRESS_DOC_BUSINESS_PHONE:
				_filer_business_address_business_phone = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _filer_business_address_business_phone = s_utf8 = \"" 
					 << _filer_business_address_business_phone << "\"..." << endl;
				}

				_state = STATE_IN_FILER_BUSINESS_ADDRESS_DOC;
				break;

			///////////
			
			case STATE_IN_ISSUER_COMPANY_DATA_DOC_CENTRAL_INDEX_KEY:
				_issuer_company_data_central_index_key = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _issuer_company_data_central_index_key = s_utf8 = \"" 
					 << _issuer_company_data_central_index_key << "\"..." << endl;
				}

				_state = STATE_IN_ISSUER_COMPANY_DATA_DOC;
				break;

			case STATE_IN_ISSUER_COMPANY_DATA_DOC_COMPANY_CONFORMED_NAME:
				_issuer_company_data_company_conformed_name = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _issuer_company_data_company_conformed_name = s_utf8 = \"" 
					 << _issuer_company_data_company_conformed_name << "\"..." << endl;
				}

				_state = STATE_IN_ISSUER_COMPANY_DATA_DOC;
				break;

			case STATE_IN_ISSUER_COMPANY_DATA_DOC_STANDARD_INDUSTRIAL_CLASSIFICATION:
				_issuer_company_data_standard_industrial_classification = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _issuer_company_data_standard_industrial_classification = s_utf8 = \"" 
					 << _issuer_company_data_standard_industrial_classification << "\"..." << endl;
				}

				_state = STATE_IN_ISSUER_COMPANY_DATA_DOC;
				break;

			case STATE_IN_ISSUER_COMPANY_DATA_DOC_STATE_OF_INCORPORATION:
				_issuer_company_data_state_of_incorporation = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _issuer_company_data_state_of_incorporation = s_utf8 = \"" 
					 << _issuer_company_data_state_of_incorporation << "\"..." << endl;
				}

				_state = STATE_IN_ISSUER_COMPANY_DATA_DOC;
				break;

			case STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_STREET_1:
				_issuer_business_address_street_1 = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _issuer_business_address_street_1 = s_utf8 = \"" 
					 << _issuer_business_address_street_1 << "\"..." << endl;
				}

				_state = STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC;
				break;

			case STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_STREET_2:
				_issuer_business_address_street_2 = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _issuer_business_address_street_2 = s_utf8 = \"" 
					 << _issuer_business_address_street_2 << "\"..." << endl;
				}

				_state = STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC;
				break;

			case STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_CITY:
				_issuer_business_address_city = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _issuer_business_address_city = s_utf8 = \"" 
					 << _issuer_business_address_city << "\"..." << endl;
				}

				_state = STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC;
				break;

			case STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_STATE:
				_issuer_business_address_state = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _issuer_business_address_state = s_utf8 = \"" 
					 << _issuer_business_address_state << "\"..." << endl;
				}

				_state = STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC;
				break;

			case STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_ZIP:
				_issuer_business_address_zip = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _ISSUER_business_address_zip = s_utf8 = \"" 
					 << _issuer_business_address_zip << "\"..." << endl;
				}

				_state = STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC;
				break;

			case STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_BUSINESS_PHONE:
				_issuer_business_address_business_phone = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _issuer_business_address_business_phone = s_utf8 = \"" 
					 << _issuer_business_address_business_phone << "\"..." << endl;
				}

				_state = STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC;
				break;

			////

			case STATE_IN_REPORTING_OWNER_OWNER_DATA_DOC_CENTRAL_INDEX_KEY:
				_reporting_owner_owner_data_central_index_key = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _reporting_owner_owner_data_central_index_key = s_utf8 = \"" 
					 << _reporting_owner_owner_data_central_index_key << "\"..." << endl;
				}

				_state = STATE_IN_REPORTING_OWNER_OWNER_DATA_DOC;
				break;

			case STATE_IN_REPORTING_OWNER_OWNER_DATA_DOC_COMPANY_CONFORMED_NAME:
				_reporting_owner_owner_data_company_conformed_name = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _reporting_owner_owner_data_company_conformed_name = s_utf8 = \"" 
					 << _reporting_owner_owner_data_company_conformed_name << "\"..." << endl;
				}

				_state = STATE_IN_REPORTING_OWNER_OWNER_DATA_DOC;
				break;

			case STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS_DOC_STREET_1:
				_reporting_owner_business_address_street_1 = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _reporting_owner_business_address_street_1 = s_utf8 = \"" 
					 << _reporting_owner_business_address_street_1 << "\"..." << endl;
				}

				_state = STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS_DOC;
				break;

			case STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS_DOC_STREET_2:
				_reporting_owner_business_address_street_2 = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _reporting_owner_business_address_street_2 = s_utf8 = \"" 
					 << _reporting_owner_business_address_street_2 << "\"..." << endl;
				}

				_state = STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS_DOC;
				break;

			case STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS_DOC_CITY:
				_reporting_owner_business_address_city = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _reporting_owner_business_address_city = s_utf8 = \"" 
					 << _reporting_owner_business_address_city << "\"..." << endl;
				}

				_state = STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS_DOC;
				break;

			case STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS_DOC_STATE:
				_reporting_owner_business_address_state = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _reporting_owner_business_address_state = s_utf8 = \"" 
					 << _reporting_owner_business_address_state << "\"..." << endl;
				}

				_state = STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS_DOC;
				break;

			case STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS_DOC_ZIP:
				_reporting_owner_business_address_zip = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _REPORTING_OWNER_business_address_zip = s_utf8 = \"" 
					 << _reporting_owner_business_address_zip << "\"..." << endl;
				}

				_state = STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS_DOC;
				break;

			case STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS_DOC_BUSINESS_PHONE:
				_reporting_owner_business_address_business_phone = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _reporting_owner_business_address_business_phone = s_utf8 = \"" 
					 << _reporting_owner_business_address_business_phone << "\"..." << endl;
				}

				_state = STATE_IN_REPORTING_OWNER_BUSINESS_ADDRESS_DOC;
				break;
			////
			case STATE_IN_REPORTING_OWNER_MAIL_ADDRESS_DOC_STREET_1:
				_reporting_owner_mail_address_street_1 = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _reporting_owner_mail_address_street_1 = s_utf8 = \"" 
					 << _reporting_owner_mail_address_street_1 << "\"..." << endl;
				}

				_state = STATE_IN_REPORTING_OWNER_MAIL_ADDRESS_DOC;
				break;

			case STATE_IN_REPORTING_OWNER_MAIL_ADDRESS_DOC_STREET_2:
				_reporting_owner_mail_address_street_2 = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _reporting_owner_mail_address_street_2 = s_utf8 = \"" 
					 << _reporting_owner_mail_address_street_2 << "\"..." << endl;
				}

				_state = STATE_IN_REPORTING_OWNER_MAIL_ADDRESS_DOC;
				break;

			case STATE_IN_REPORTING_OWNER_MAIL_ADDRESS_DOC_CITY:
				_reporting_owner_mail_address_city = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _reporting_owner_mail_address_city = s_utf8 = \"" 
					 << _reporting_owner_mail_address_city << "\"..." << endl;
				}

				_state = STATE_IN_REPORTING_OWNER_MAIL_ADDRESS_DOC;
				break;

			case STATE_IN_REPORTING_OWNER_MAIL_ADDRESS_DOC_STATE:
				_reporting_owner_mail_address_state = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _reporting_owner_mail_address_state = s_utf8 = \"" 
					 << _reporting_owner_mail_address_state << "\"..." << endl;
				}

				_state = STATE_IN_REPORTING_OWNER_MAIL_ADDRESS_DOC;
				break;

			case STATE_IN_REPORTING_OWNER_MAIL_ADDRESS_DOC_ZIP:
				_reporting_owner_mail_address_zip = s_utf8;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): Set _REPORTING_OWNER_mail_address_zip = s_utf8 = \"" 
					 << _reporting_owner_mail_address_zip << "\"..." << endl;
				}

				_state = STATE_IN_REPORTING_OWNER_MAIL_ADDRESS_DOC;
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
