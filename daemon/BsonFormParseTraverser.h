#ifndef _BSON_FORM_PARSE_TRAVERSER_H_
#define _BSON_FORM_PARSE_TRAVERSER_H_

#include "BsonTraverser.h"

namespace JDA {

class BsonFormParseTraverser : public BsonTraverser {

	protected:

		enum StateType {

			STATE_WITHOUT,

				STATE_IN_CIK,

				STATE_IN_FILERS,

					STATE_IN_FILER,

						STATE_IN_FILER_COMPANY_DATA,
							STATE_IN_FILER_COMPANY_DATA_DOC,
								STATE_IN_FILER_COMPANY_DATA_DOC_COMPANY_CONFORMED_NAME,
								STATE_IN_FILER_COMPANY_DATA_DOC_CENTRAL_INDEX_KEY,
								STATE_IN_FILER_COMPANY_DATA_DOC_STATE_OF_INCORPORATION,

						STATE_IN_FILER_FILING_VALUES,
							STATE_IN_FILER_FILING_VALUES_DOC,

						STATE_IN_FILER_BUSINESS_ADDRESS,
							STATE_IN_FILER_BUSINESS_ADDRESS_DOC,

						STATE_IN_FILER_MAIL_ADDRESS,
							STATE_IN_FILER_MAIL_ADDRESS_DOC
		};

		// Used by stateTypeToString()
		static const char* stateTypeEnumStrings[];

		static string stateTypeToString( StateType stateType ){
			return stateTypeEnumStrings[ stateType ];
		}

		// The Central Index Key or CIK is a 10-digit number (think of it
		// as a 32-bit integer right-padded with zero's...for example the CIK
		// for WESTERN INTERNATIONAL SECURITIES, INC. is "0001000152")
		// used on the Securities and Exchange Commission's computer systems to
		// identify corporations and individuals who have filed disclosure with the SEC.
		int _cik;
		string _filer_company_data_central_index_key;
		string _filer_company_data_company_conformed_name;
		string _filer_company_data_state_of_incorporation;
		string _filer_business_address_street_1;
		string _filer_business_address_street_2;
		string _filer_business_address_city;
		string _filer_business_address_state;
		string _filer_business_address_zip;
		string _filer_business_address_business_phone;

		StateType _state;

	public:
		int parse_it( const bson_t* p_bson );

		int getCik() const { return _cik; }
		string getFilerCompanyDataCentralIndexKey() const { return _filer_company_data_central_index_key; }
		string getFilerCompanyDataCompanyConformedName() const { return _filer_company_data_company_conformed_name; }

		ostream& print(ostream& out) const {
			out
			//<< "\t" << "getCik() = " << this->getCik() << ",\n"
			//<< "\t" << "getFilerCompanyDataCentralIndexKey() = \"" << this->getFilerCompanyDataCentralIndexKey() << "\",\n" 
			//<< "\t" << "getFilerCompanyDataCompanyConformedName() = \"" << this->getFilerCompanyDataCompanyConformedName() << "\",\n"
			<< "\t" << "_cik = " << this->_cik << ",\n"
			<< "\t" << "_filer_company_data_central_index_key = \"" << _filer_company_data_central_index_key << "\",\n" 
			<< "\t" << "_filer_company_data_company_conformed_name = \"" << _filer_company_data_company_conformed_name << "\",\n" 
			<< "\t" << "_filer_company_data_state_of_incorporation = \"" << _filer_company_data_state_of_incorporation << "\"." 
			;
			return out;
		}

	public:
		// BEGIN: OVERRIDE THESE FOR ELEMENTS YOU WISH TO "CATCH"...
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
		// END: OVERRIDE THESE FOR ELEMENTS YOU WISH TO "CATCH"...

};

} /* namespace JDA { */

ostream& operator<<(ostream& out, const JDA::BsonFormParseTraverser& traverser );

#endif /* #ifndef _BSON_FORM_PARSE_TRAVERSER_H_ */
