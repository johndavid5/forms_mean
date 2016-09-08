#ifndef _BSON_FORM_PARSE_TRAVERSER_H_
#define _BSON_FORM_PARSE_TRAVERSER_H_

#include "BsonTraverser.h"

namespace JDA {

/** Your mission, Jim, should you decide to accept it, is
* to traverse the tree, find a filer or issuer whose
* central_index_key matches the top-level _cik, and
* copy its information to the top level..."company"...
*/
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
								STATE_IN_FILER_COMPANY_DATA_DOC_STANDARD_INDUSTRIAL_CLASSIFICATION,
								STATE_IN_FILER_COMPANY_DATA_DOC_STATE_OF_INCORPORATION,

						STATE_IN_FILER_FILING_VALUES,
							STATE_IN_FILER_FILING_VALUES_DOC,

						STATE_IN_FILER_BUSINESS_ADDRESS,
							STATE_IN_FILER_BUSINESS_ADDRESS_DOC,
								STATE_IN_FILER_BUSINESS_ADDRESS_DOC_STREET_1,
								STATE_IN_FILER_BUSINESS_ADDRESS_DOC_STREET_2,
								STATE_IN_FILER_BUSINESS_ADDRESS_DOC_CITY,
								STATE_IN_FILER_BUSINESS_ADDRESS_DOC_STATE,
								STATE_IN_FILER_BUSINESS_ADDRESS_DOC_ZIP,
								STATE_IN_FILER_BUSINESS_ADDRESS_DOC_BUSINESS_PHONE,

						STATE_IN_FILER_MAIL_ADDRESS,
							STATE_IN_FILER_MAIL_ADDRESS_DOC,

			STATE_IN_ISSUER,
					STATE_IN_ISSUER_COMPANY_DATA,
						STATE_IN_ISSUER_COMPANY_DATA_DOC,
							STATE_IN_ISSUER_COMPANY_DATA_DOC_COMPANY_CONFORMED_NAME,
							STATE_IN_ISSUER_COMPANY_DATA_DOC_CENTRAL_INDEX_KEY,
							STATE_IN_ISSUER_COMPANY_DATA_DOC_STANDARD_INDUSTRIAL_CLASSIFICATION,
							STATE_IN_ISSUER_COMPANY_DATA_DOC_STATE_OF_INCORPORATION,

						STATE_IN_ISSUER_FILING_VALUES,
							STATE_IN_ISSUER_FILING_VALUES_DOC,

						STATE_IN_ISSUER_BUSINESS_ADDRESS,
							STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC,
								STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_STREET_1,
								STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_STREET_2,
								STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_CITY,
								STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_STATE,
								STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_ZIP,
								STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_BUSINESS_PHONE,

						STATE_IN_ISSUER_MAIL_ADDRESS,
							STATE_IN_ISSUER_MAIL_ADDRESS_DOC

		}; /* enum StateType */

		static string stateTypeToString( StateType stateType ){
			switch( stateType ){
				case STATE_WITHOUT:
					return "STATE_WITHOUT";
				case STATE_IN_CIK:
					return "STATE_IN_CIK";
				case STATE_IN_FILERS:
					return "STATE_IN_FILERS";
				case STATE_IN_FILER:
					return "STATE_IN_FILER";
				case STATE_IN_FILER_COMPANY_DATA:
					return "STATE_IN_FILER_COMPANY_DATA";
				case STATE_IN_FILER_COMPANY_DATA_DOC:
					return "STATE_IN_FILER_COMPANY_DATA_DOC";
				case STATE_IN_FILER_COMPANY_DATA_DOC_COMPANY_CONFORMED_NAME:
					return "STATE_IN_FILER_COMPANY_DATA_DOC_COMPANY_CONFORMED_NAME";
				case STATE_IN_FILER_COMPANY_DATA_DOC_CENTRAL_INDEX_KEY:
					return "STATE_IN_FILER_COMPANY_DATA_DOC_CENTRAL_INDEX_KEY";
				case STATE_IN_FILER_COMPANY_DATA_DOC_STANDARD_INDUSTRIAL_CLASSIFICATION:
					return "STATE_IN_FILER_COMPANY_DATA_DOC_STANDARD_INDUSTRIAL_CLASSIFICATION";
				case STATE_IN_FILER_COMPANY_DATA_DOC_STATE_OF_INCORPORATION:
					return "STATE_IN_FILER_COMPANY_DATA_DOC_STATE_OF_INCORPORATION";
				case STATE_IN_FILER_FILING_VALUES:
					return "STATE_IN_FILER_FILING_VALUES";
				case STATE_IN_FILER_FILING_VALUES_DOC:
					return "STATE_IN_FILER_FILING_VALUES_DOC";
				case STATE_IN_FILER_BUSINESS_ADDRESS:
					return "STATE_IN_FILER_BUSINESS_ADDRESS";
				case STATE_IN_FILER_BUSINESS_ADDRESS_DOC:
					return "STATE_IN_FILER_BUSINESS_ADDRESS_DOC";
				case STATE_IN_FILER_BUSINESS_ADDRESS_DOC_STREET_1:
					return "STATE_IN_FILER_BUSINESS_ADDRESS_DOC_STREET_1";
				case STATE_IN_FILER_BUSINESS_ADDRESS_DOC_STREET_2:
					return "STATE_IN_FILER_BUSINESS_ADDRESS_DOC_STREET_2";
				case STATE_IN_FILER_BUSINESS_ADDRESS_DOC_CITY:
					return "STATE_IN_FILER_BUSINESS_ADDRESS_DOC_CITY";
				case STATE_IN_FILER_BUSINESS_ADDRESS_DOC_STATE:
					return "STATE_IN_FILER_BUSINESS_ADDRESS_DOC_STATE";
				case STATE_IN_FILER_BUSINESS_ADDRESS_DOC_ZIP:
					return "STATE_IN_FILER_BUSINESS_ADDRESS_DOC_ZIP";
				case STATE_IN_FILER_BUSINESS_ADDRESS_DOC_BUSINESS_PHONE:
					return "STATE_IN_FILER_BUSINESS_ADDRESS_DOC_BUSINESS_PHONE";
				case STATE_IN_FILER_MAIL_ADDRESS:
					return "STATE_IN_FILER_MAIL_ADDRESS";
				case STATE_IN_FILER_MAIL_ADDRESS_DOC:
					return "STATE_IN_FILER_MAIL_ADDRESS_DOC";
				case STATE_IN_ISSUER:
					return "STATE_IN_ISSUER";
				case STATE_IN_ISSUER_COMPANY_DATA:
					return "STATE_IN_ISSUER_COMPANY_DATA";
				case STATE_IN_ISSUER_COMPANY_DATA_DOC:
					return "STATE_IN_ISSUER_COMPANY_DATA_DOC";
				case STATE_IN_ISSUER_COMPANY_DATA_DOC_COMPANY_CONFORMED_NAME:
					return "STATE_IN_ISSUER_COMPANY_DATA_DOC_COMPANY_CONFORMED_NAME";
				case STATE_IN_ISSUER_COMPANY_DATA_DOC_CENTRAL_INDEX_KEY:
					return "STATE_IN_ISSUER_COMPANY_DATA_DOC_CENTRAL_INDEX_KEY";
				case STATE_IN_ISSUER_COMPANY_DATA_DOC_STANDARD_INDUSTRIAL_CLASSIFICATION:
					return "STATE_IN_ISSUER_COMPANY_DATA_DOC_STANDARD_INDUSTRIAL_CLASSIFICATION";
				case STATE_IN_ISSUER_COMPANY_DATA_DOC_STATE_OF_INCORPORATION:
					return "STATE_IN_ISSUER_COMPANY_DATA_DOC_STATE_OF_INCORPORATION";
				case STATE_IN_ISSUER_FILING_VALUES:
					return "STATE_IN_ISSUER_FILING_VALUES";
				case STATE_IN_ISSUER_FILING_VALUES_DOC:
					return "STATE_IN_ISSUER_FILING_VALUES_DOC";
				case STATE_IN_ISSUER_BUSINESS_ADDRESS:
					return "STATE_IN_ISSUER_BUSINESS_ADDRESS";
				case STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC:
					return "STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC";
				case STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_STREET_1:
					return "STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_STREET_1";
				case STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_STREET_2:
					return "STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_STREET_2";
				case STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_CITY:
					return "STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_CITY";
				case STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_STATE:
					return "STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_STATE";
				case STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_ZIP:
					return "STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_ZIP";
				case STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_BUSINESS_PHONE:
					return "STATE_IN_ISSUER_BUSINESS_ADDRESS_DOC_BUSINESS_PHONE";
				case STATE_IN_ISSUER_MAIL_ADDRESS:
					return "STATE_IN_ISSUER_MAIL_ADDRESS";
				case STATE_IN_ISSUER_MAIL_ADDRESS_DOC:
					return "STATE_IN_ISSUER_MAIL_ADDRESS_DOC";
				default:
					return "???";
			}/* switch( stateType ) */
		}/* stateTypeToString() */


		// The Central Index Key or CIK is a 10-digit number (think of it
		// as a 32-bit integer right-padded with zero's...for example the CIK
		// for WESTERN INTERNATIONAL SECURITIES, INC. is "0001000152")
		// used on the Securities and Exchange Commission's computer systems to
		// identify corporations and individuals who have filed disclosure with the SEC.
		int _cik;

		string _company_company_data_central_index_key; 
		string _company_company_data_company_conformed_name;
		string _company_company_data_standard_industrial_classification;
		string _company_company_data_state_of_incorporation;
		string _company_business_address_street_1;
		string _company_business_address_street_2;
		string _company_business_address_city;
		string _company_business_address_state;
		string _company_business_address_zip;
		string _company_business_address_business_phone;             

		string _filer_company_data_central_index_key;
		string _filer_company_data_company_conformed_name;
		string _filer_company_data_standard_industrial_classification;
		string _filer_company_data_state_of_incorporation;
		string _filer_business_address_street_1;
		string _filer_business_address_street_2;
		string _filer_business_address_city;
		string _filer_business_address_state;
		string _filer_business_address_zip;
		string _filer_business_address_business_phone;

		string _issuer_company_data_central_index_key;
		string _issuer_company_data_company_conformed_name;
		string _issuer_company_data_standard_industrial_classification;
		string _issuer_company_data_state_of_incorporation;
		string _issuer_business_address_street_1;
		string _issuer_business_address_street_2;
		string _issuer_business_address_city;
		string _issuer_business_address_state;
		string _issuer_business_address_zip;
		string _issuer_business_address_business_phone;

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
			<< "\t" << "_cik = " << _cik << ".\n"
			<< "\n" 
			<< "\t" << "_company_company_data_central_index_key = \"" << _company_company_data_central_index_key << "\",\n" 
			<< "\t" << "_company_company_data_company_conformed_name = \"" << _company_company_data_company_conformed_name << "\",\n" 
			<< "\t" << "_company_company_data_standard_industrial_classification = \"" << _company_company_data_standard_industrial_classification << "\",\n" 
			<< "\t" << "_company_company_data_state_of_incorporation = \"" << _company_company_data_state_of_incorporation << "\",\n" 
			<< "\t" << "_company_business_address_street_1 = \"" << _company_business_address_street_1 << "\",\n"
			<< "\t" << "_company_business_address_street_2 = \"" << _company_business_address_street_2 << "\",\n"
			<< "\t" << "_company_business_address_city = \"" << _company_business_address_city << "\",\n"
			<< "\t" << "_company_business_address_state = \"" << _company_business_address_state << "\",\n"
			<< "\t" << "_company_business_address_zip = \"" << _company_business_address_zip << "\",\n"
			<< "\t" << "_company_business_address_business_phone = \"" << _company_business_address_business_phone << "\".\n"
			<< "\n" 
			<< "\t" << "_filer_company_data_central_index_key = \"" << _filer_company_data_central_index_key << "\",\n" 
			<< "\t" << "_filer_company_data_company_conformed_name = \"" << _filer_company_data_company_conformed_name << "\",\n" 
			<< "\t" << "_filer_company_data_standard_industrial_classification = \"" << _filer_company_data_standard_industrial_classification << "\",\n" 
			<< "\t" << "_filer_company_data_state_of_incorporation = \"" << _filer_company_data_state_of_incorporation << "\",\n" 
			<< "\t" << "_filer_business_address_street_1 = \"" << _filer_business_address_street_1 << "\",\n"
			<< "\t" << "_filer_business_address_street_2 = \"" << _filer_business_address_street_2 << "\",\n"
			<< "\t" << "_filer_business_address_city = \"" << _filer_business_address_city << "\",\n"
			<< "\t" << "_filer_business_address_state = \"" << _filer_business_address_state << "\",\n"
			<< "\t" << "_filer_business_address_zip = \"" << _filer_business_address_zip << "\",\n"
			<< "\t" << "_filer_business_address_business_phone = \"" << _filer_business_address_business_phone << "\".\n"
			<< "\n" 
			<< "\t" << "_issuer_company_data_central_index_key = \"" << _issuer_company_data_central_index_key << "\",\n" 
			<< "\t" << "_issuer_company_data_company_conformed_name = \"" << _issuer_company_data_company_conformed_name << "\",\n" 
			<< "\t" << "_issuer_company_data_standard_industrial_classification = \"" << _issuer_company_data_standard_industrial_classification << "\",\n" 
			<< "\t" << "_issuer_company_data_company_conformed_name = \"" << _issuer_company_data_company_conformed_name << "\",\n" 
			<< "\t" << "_issuer_company_data_state_of_incorporation = \"" << _issuer_company_data_state_of_incorporation << "\",\n" 
			<< "\t" << "_issuer_business_address_street_1 = \"" << _issuer_business_address_street_1 << "\",\n"
			<< "\t" << "_issuer_business_address_street_2 = \"" << _issuer_business_address_street_2 << "\",\n"
			<< "\t" << "_issuer_business_address_city = \"" << _issuer_business_address_city << "\",\n"
			<< "\t" << "_issuer_business_address_state = \"" << _issuer_business_address_state << "\",\n"
			<< "\t" << "_issuer_business_address_zip = \"" << _issuer_business_address_zip << "\",\n"
			<< "\t" << "_issuer_business_address_business_phone = \"" << _issuer_business_address_business_phone << "\"."
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
