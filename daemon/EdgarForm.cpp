#include <iostream>
using namespace std;

#include "EdgarForm.h"

		/* static */ const char* JDA::EdgarForm::stateTypeEnumStrings[] = {

			"STATE_START",

			"STATE_IN_SEC_HEADER",
			"STATE_DONE_WITH_SEC_HEADER",

			"STATE_FILER", // SUBJECT COMPANY, FILER, etc. 
				"STATE_FILER_COMPANY_DATA",
				"STATE_FILER_FILING_VALUES",
				"STATE_FILER_BUSINESS_ADDRESS",
				"STATE_FILER_MAIL_ADDRESS",

			"STATE_ISSUER",
				"STATE_ISSUER_COMPANY_DATA",
				"STATE_ISSUER_BUSINESS_ADDRESS",
				"STATE_ISSUER_MAIL_ADDRESS",

			"STATE_REPORTING_OWNER",
				"STATE_REPORTING_OWNER_OWNER_DATA",
				"STATE_REPORTING_OWNER_FILING_VALUES",
				"STATE_REPORTING_OWNER_MAIL_ADDRESS",
				"STATE_REPORTING_OWNER_BUSINESS_ADDRESS", /* Do you think...? */

			"STATE_FILED_BY",
				"STATE_FILED_BY_COMPANY_DATA",
				"STATE_FILED_BY_FILING_VALUES",
				"STATE_FILED_BY_BUSINESS_ADDRESS",
				"STATE_FILED_BY_MAIL_ADDRESS",
		};


		/** @constructor */
		JDA::EdgarForm::EdgarForm() : m_state(STATE_START), m_line_number(0), m_p_logger(NULL) {

			m_new_filer = shared_ptr<FILER>(NULL);

			//m_filer_company_data_map["COMPANY CONFORMED NAME"] = &(m_filer.company_data.company_conformed_name);	
			//m_filer_company_data_map["CENTRAL INDEX KEY"] = &(m_filer.company_data.central_index_key);	
			//m_filer_company_data_map["STANDARD INDUSTRIAL CLASSIFICATION"] = &(m_filer.company_data.standard_industrial_classification);	
			//m_filer_company_data_map["IRS NUMBER"] = &(m_filer.company_data.irs_number);	
			//m_filer_company_data_map["STATE OF INCORPORATION"] = &(m_filer.company_data.state_of_incorporation);	
			//m_filer_company_data_map["FISCAL YEAR END"] = &(m_filer.company_data.fiscal_year_end);	

			//m_filer_filing_values_map["FORM TYPE"] = &(m_filer.filing_values.form_type);	
			//m_filer_filing_values_map["SEC ACT"] = &(m_filer.filing_values.sec_act);	
			//m_filer_filing_values_map["SEC FILE NUMBER"] = &(m_filer.filing_values.sec_file_number);	
			//m_filer_filing_values_map["FILM NUMBER"] = &(m_filer.filing_values.film_number);	

			//m_filer_business_address_map["STREET 1"] = &(m_filer.business_address.street_1);	
			//m_filer_business_address_map["STREET 2"] = &(m_filer.business_address.street_2);	
			//m_filer_business_address_map["CITY"] = &(m_filer.business_address.city);	
			//m_filer_business_address_map["STATE"] = &(m_filer.business_address.state);	
			//m_filer_business_address_map["ZIP"] = &(m_filer.business_address.zip);	
			//m_filer_business_address_map["BUSINESS PHONE"] = &(m_filer.business_address.business_phone);	

			//m_filer_mail_address_map["STREET 1"] = &(m_filer.mail_address.street_1);	
			//m_filer_mail_address_map["STREET 2"] = &(m_filer.mail_address.street_2);	
			//m_filer_mail_address_map["CITY"] = &(m_filer.mail_address.city);	
			//m_filer_mail_address_map["STATE"] = &(m_filer.mail_address.state);	
			//m_filer_mail_address_map["ZIP"] = &(m_filer.mail_address.zip);	

			/* ----------------------- */

			m_issuer_company_data_map["COMPANY CONFORMED NAME"] = &(m_issuer.company_data.company_conformed_name);	
			m_issuer_company_data_map["CENTRAL INDEX KEY"] = &(m_issuer.company_data.central_index_key);	
			m_issuer_company_data_map["STANDARD INDUSTRIAL CLASSIFICATION"] = &(m_issuer.company_data.standard_industrial_classification);	
			m_issuer_company_data_map["IRS NUMBER"] = &(m_issuer.company_data.irs_number);	
			m_issuer_company_data_map["STATE OF INCORPORATION"] = &(m_issuer.company_data.state_of_incorporation);	
			m_issuer_company_data_map["FISCAL YEAR END"] = &(m_issuer.company_data.fiscal_year_end);	

			m_issuer_business_address_map["STREET 1"] = &(m_issuer.business_address.street_1);	
			m_issuer_business_address_map["STREET 2"] = &(m_issuer.business_address.street_2);	
			m_issuer_business_address_map["CITY"] = &(m_issuer.business_address.city);	
			m_issuer_business_address_map["STATE"] = &(m_issuer.business_address.state);	
			m_issuer_business_address_map["ZIP"] = &(m_issuer.business_address.zip);	
			m_issuer_business_address_map["BUSINESS PHONE"] = &(m_issuer.business_address.business_phone);	

			m_issuer_mail_address_map["STREET 1"] = &(m_issuer.mail_address.street_1);	
			m_issuer_mail_address_map["STREET 2"] = &(m_issuer.mail_address.street_2);	
			m_issuer_mail_address_map["CITY"] = &(m_issuer.mail_address.city);	
			m_issuer_mail_address_map["STATE"] = &(m_issuer.mail_address.state);	
			m_issuer_mail_address_map["ZIP"] = &(m_issuer.mail_address.zip);	

			/* ----------------------- */

			m_reporting_owner_owner_data_map["COMPANY CONFORMED NAME"] = &(m_reporting_owner.owner_data.company_conformed_name); 
			m_reporting_owner_owner_data_map["CENTRAL INDEX KEY"] = &(m_reporting_owner.owner_data.central_index_key); 

			m_reporting_owner_filing_values_map["FORM TYPE"] = &(m_reporting_owner.filing_values.form_type);	
			m_reporting_owner_filing_values_map["SEC ACT"] = &(m_reporting_owner.filing_values.sec_act);	
			m_reporting_owner_filing_values_map["SEC FILE NUMBER"] = &(m_reporting_owner.filing_values.sec_file_number);	
			m_reporting_owner_filing_values_map["FILM NUMBER"] = &(m_reporting_owner.filing_values.film_number);	

			m_reporting_owner_mail_address_map["STREET 1"] = &(m_reporting_owner.mail_address.street_1);	
			m_reporting_owner_mail_address_map["STREET 2"] = &(m_reporting_owner.mail_address.street_2);	
			m_reporting_owner_mail_address_map["CITY"] = &(m_reporting_owner.mail_address.city);	
			m_reporting_owner_mail_address_map["STATE"] = &(m_reporting_owner.mail_address.state);	
			m_reporting_owner_mail_address_map["ZIP"] = &(m_reporting_owner.mail_address.zip);	

			m_reporting_owner_business_address_map["STREET 1"] = &(m_reporting_owner.business_address.street_1);	
			m_reporting_owner_business_address_map["STREET 2"] = &(m_reporting_owner.business_address.street_2);	
			m_reporting_owner_business_address_map["CITY"] = &(m_reporting_owner.business_address.city);	
			m_reporting_owner_business_address_map["STATE"] = &(m_reporting_owner.business_address.state);	
			m_reporting_owner_business_address_map["ZIP"] = &(m_reporting_owner.business_address.zip);	
			m_reporting_owner_business_address_map["BUSINESS PHONE"] = &(m_reporting_owner.business_address.business_phone);	

			/* ----------------------- */

			m_filed_by_company_data_map["COMPANY CONFORMED NAME"] = &(m_filed_by.company_data.company_conformed_name);	
			m_filed_by_company_data_map["CENTRAL INDEX KEY"] = &(m_filed_by.company_data.central_index_key);	
			m_filed_by_company_data_map["STANDARD INDUSTRIAL CLASSIFICATION"] = &(m_filed_by.company_data.standard_industrial_classification);	
			m_filed_by_company_data_map["IRS NUMBER"] = &(m_filed_by.company_data.irs_number);	
			m_filed_by_company_data_map["STATE OF INCORPORATION"] = &(m_filed_by.company_data.state_of_incorporation);	
			m_filed_by_company_data_map["FISCAL YEAR END"] = &(m_filed_by.company_data.fiscal_year_end);	

			m_filed_by_filing_values_map["FORM TYPE"] = &(m_filed_by.filing_values.form_type);	
			m_filed_by_filing_values_map["SEC ACT"] = &(m_filed_by.filing_values.sec_act);	
			m_filed_by_filing_values_map["SEC FILE NUMBER"] = &(m_filed_by.filing_values.sec_file_number);	
			m_filed_by_filing_values_map["FILM NUMBER"] = &(m_filed_by.filing_values.film_number);	

			m_filed_by_business_address_map["STREET 1"] = &(m_filed_by.business_address.street_1);	
			m_filed_by_business_address_map["STREET 2"] = &(m_filed_by.business_address.street_2);	
			m_filed_by_business_address_map["CITY"] = &(m_filed_by.business_address.city);	
			m_filed_by_business_address_map["STATE"] = &(m_filed_by.business_address.state);	
			m_filed_by_business_address_map["ZIP"] = &(m_filed_by.business_address.zip);	
			m_filed_by_business_address_map["BUSINESS PHONE"] = &(m_filed_by.business_address.business_phone);	

			m_filed_by_mail_address_map["STREET 1"] = &(m_filed_by.mail_address.street_1);	
			m_filed_by_mail_address_map["STREET 2"] = &(m_filed_by.mail_address.street_2);	
			m_filed_by_mail_address_map["CITY"] = &(m_filed_by.mail_address.city);	
			m_filed_by_mail_address_map["STATE"] = &(m_filed_by.mail_address.state);	
			m_filed_by_mail_address_map["ZIP"] = &(m_filed_by.mail_address.zip);	

			/* ----------------------- */

		}/* JDA::EdgarForm::EdgarForm() */

		void JDA::EdgarForm::begin_new_filer(){

			const char* sWho = "JDA::EdgarForm::begin_new_filer";

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): " << "SHEMP: Moe, startin' a new m_new_filer, Moe..." << endl;
			}

			m_new_filer = shared_ptr<FILER>(new FILER());
			this->map_filer( m_new_filer.get() ); 	

		}/* void JDA::EdgarForm::begin_new_filer() */

		void JDA::EdgarForm::save_new_filer(){

			const char* sWho = "JDA::EdgarForm::save_new_filer";

			if( m_new_filer.get() != NULL ){

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): SHEMP: Moe, pushin' m_new_filer = " << m_new_filer->toString() << " onto m_filers, Moe..." << endl;
				}
				
				m_filers.push_back( m_new_filer );	
				m_new_filer = shared_ptr<FILER>(NULL);
			}
			else {
				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): SHEMP: Moe, m_filers.get() is NULL, Moe, so I ain't pushin' it onto m_filers, Moe, Sorry, Moe..." << endl;
				}
				
			}

		}/* void JDA::EdgarForm::save_new_filer() */

		void JDA::EdgarForm::map_filer( FILER* p_filer ){

			m_filer_company_data_map["COMPANY CONFORMED NAME"] = &(p_filer->company_data.company_conformed_name);	
			m_filer_company_data_map["CENTRAL INDEX KEY"] = &(p_filer->company_data.central_index_key);	
			m_filer_company_data_map["STANDARD INDUSTRIAL CLASSIFICATION"] = &(p_filer->company_data.standard_industrial_classification);	
			m_filer_company_data_map["IRS NUMBER"] = &(p_filer->company_data.irs_number);	
			m_filer_company_data_map["STATE OF INCORPORATION"] = &(p_filer->company_data.state_of_incorporation);	
			m_filer_company_data_map["FISCAL YEAR END"] = &(p_filer->company_data.fiscal_year_end);	

			m_filer_filing_values_map["FORM TYPE"] = &(p_filer->filing_values.form_type);	
			m_filer_filing_values_map["SEC ACT"] = &(p_filer->filing_values.sec_act);	
			m_filer_filing_values_map["SEC FILE NUMBER"] = &(p_filer->filing_values.sec_file_number);	
			m_filer_filing_values_map["FILM NUMBER"] = &(p_filer->filing_values.film_number);	

			m_filer_business_address_map["STREET 1"] = &(p_filer->business_address.street_1);	
			m_filer_business_address_map["STREET 2"] = &(p_filer->business_address.street_2);	
			m_filer_business_address_map["CITY"] = &(p_filer->business_address.city);	
			m_filer_business_address_map["STATE"] = &(p_filer->business_address.state);	
			m_filer_business_address_map["ZIP"] = &(p_filer->business_address.zip);	
			m_filer_business_address_map["BUSINESS PHONE"] = &(p_filer->business_address.business_phone);	

			m_filer_mail_address_map["STREET 1"] = &(p_filer->mail_address.street_1);	
			m_filer_mail_address_map["STREET 2"] = &(p_filer->mail_address.street_2);	
			m_filer_mail_address_map["CITY"] = &(p_filer->mail_address.city);	
			m_filer_mail_address_map["STATE"] = &(p_filer->mail_address.state);	
			m_filer_mail_address_map["ZIP"] = &(p_filer->mail_address.zip);	

		}/* JDA::EdgarForm::map_filer( ) */


		/* static */ string JDA::EdgarForm::extract_sic_code( const string& standard_industrial_classification, JDA::Logger* p_logger ) 
		{

			const char* sWho = "JDA::EdgarForm::extract_sic_code";

			if( p_logger ){
				(*p_logger)(JDA::Logger::TRACE) << sWho << "(): standard_industrial_classification = '" << standard_industrial_classification << "'..." << endl;
			}
			

			size_t i_where_right_bracket = standard_industrial_classification.rfind( "]" );


			if( p_logger ){
				(*p_logger)(JDA::Logger::TRACE) << sWho << "(): i_where_right_bracket = " << i_where_right_bracket << "..." << endl;
			}
			

			if( i_where_right_bracket != std::string::npos ){

				size_t i_where_left_bracket = standard_industrial_classification.rfind( "[", i_where_right_bracket );


				if( p_logger ){
					(*p_logger)(JDA::Logger::TRACE) << sWho << "(): i_where_left_bracket = " << i_where_left_bracket << "..." << endl;
				}
				

				if( i_where_left_bracket != std::string::npos ){

					if( p_logger ){
						(*p_logger)(JDA::Logger::TRACE) << sWho << "():\n" 
						<< "Returning substr( i_where_right_bracket+1, i_where_right_bracket-i_where_left_bracket-1 )\n"  
						<< "	    = substr( " << (i_where_right_bracket+1) << ", " << (i_where_right_bracket-i_where_left_bracket-1) << " )..." << endl;
					}

					return standard_industrial_classification.substr( i_where_left_bracket+1, i_where_right_bracket-i_where_left_bracket-1 );
				}
				else{

					if( p_logger ){
						(*p_logger)(JDA::Logger::TRACE) << sWho << "(): Looks like a bust on i_where_left_bracket, so returning \"\"..." << endl;
					}
					return "";
				}
			}
			else{
				if( p_logger ){
					(*p_logger)(JDA::Logger::DEBUG) << sWho << "(): Looks like a bust on i_where_right_bracket, so returning \"\"..." << endl;
				}
			
				return "";
			}
			
		}/* static JDA::EdgarForm::extract_sic_code( const string& standard_industrial_classification ) */


		/* static */ string JDA::EdgarForm::filers_to_string( vector<shared_ptr<FILER>>& filers ){

			ostringstream oss_out;

			oss_out << "size() = " << filers.size() << "\n";

			for(size_t i = 0; i < filers.size(); i++ ){
				oss_out << "FILER[" << i << "] = " << filers[i]->toString() << "\n";
			}

			return oss_out.str();

		}/* \* static *\ filers_to_string() */

		string JDA::EdgarForm::toString(){
			ostringstream oss_out;
			oss_out << "EdgarForm:\n"; 
			oss_out << "\n";
			oss_out << "\t" << "ACCESSION NUMBER: " << this->m_s_accession_number << "\n";
			oss_out << "\n";
			oss_out << "\t" << "FILERS:\n";
			oss_out << EdgarForm::filers_to_string( this->m_filers ) << "\n";
			oss_out << "\t" << "-------\n";
			oss_out << "\t" << "ISSUER:\n";
			oss_out << this->m_issuer.toString() << "\n";
			oss_out << "\t" << "-------\n";
			oss_out << "\t" << "REPORTING OWNER:\n";
			oss_out << this->m_reporting_owner.toString() << "\n";
			oss_out << "\t" << "-------\n";
			oss_out << "\t" << "FILED BY:\n";
			oss_out << this->m_filed_by.toString() << "\n";
			oss_out << "\t" << "-------\n";

			return oss_out.str();
		}/* toString() */


		/* static */ bool JDA::EdgarForm::splitIt( const string& le_input, string* le_left, string* le_right, const string& le_splitteo ){

			size_t i_where;

			if( ( i_where = le_input.find(":") ) != string::npos ){ 
				*le_left = le_input.substr(0, i_where);				
				*le_left = JDA::Utils::trim(*le_left);

				*le_right = le_input.substr(i_where+1);				
				*le_right = JDA::Utils::trim(*le_right);

				return true;
			}
			else {
				*le_left = "";
				*le_right = "";
				return false;
			}
		}/* splitIt() */

		JDA::EdgarForm::StateType JDA::EdgarForm::parseLine( const string& le_line ){			

			const char* sWho = "JDA::EdgarForm::parseLine";

			m_line_number++;

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): line " << m_line_number << ": \"" << le_line << "\"..." << endl; 
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): line " << m_line_number << ": BEFORE: state " << stateTypeToString( m_state ) << "..." << endl;
			}
			
			if( le_line.find("<SEC-HEADER>") != string::npos ){
				m_state = STATE_IN_SEC_HEADER;
			}
			else if( le_line.find("</SEC-HEADER>") != string::npos ){
				// In case there's a leftover m_new_filer out there that we haven't saved...
				this->save_new_filer();

				m_state = STATE_DONE_WITH_SEC_HEADER;
			}

			string s_left;
			string s_right;

			splitIt( le_line, &s_left, &s_right );

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): line " << m_line_number << ": s_left = \"" << s_left << "\", s_right = \"" << s_right << "\"..." << endl;
			}
			

			switch( m_state ){

				case STATE_IN_SEC_HEADER:
					if( s_left.compare("ACCESSION NUMBER") == 0 ){
						m_s_accession_number = s_right;
					}
					// Note: "FILED FOR" is used in an "UPLOAD" form, for example, KRISPY KREME DOUGHNUTS, ftp://ftp.sec.gov/edgar/data/1100270/0000000000-15-000342.txt
					if( s_left.compare("FILER") == 0 || s_left.compare("SUBJECT COMPANY") == 0 || s_left.compare("FILED FOR") == 0 ){
						m_state = STATE_FILER;
						this->begin_new_filer();
					}
					else if( s_left.compare("ISSUER") == 0 ){
						m_state = STATE_ISSUER;
					}
					else if( s_left.compare("REPORTING-OWNER") == 0 ){
						m_state = STATE_REPORTING_OWNER; 
					}
					else if( s_left.compare("FILED BY") == 0 ){
						m_state = STATE_FILED_BY; 
					}
					break;

				case STATE_FILER:
					if( s_left.compare("FILER") == 0 ){

						if( m_p_logger ){
							(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): SHEMP: Moe, goin' from STATE_FILER to STATE_FILER, Moe..." << endl;
						}

						this->save_new_filer();

						m_state = STATE_FILER;
						this->begin_new_filer();
					}
					else if( s_left.compare("ISSUER") == 0 ){
						this->save_new_filer();

						m_state = STATE_ISSUER;
					}
					else if( s_left.compare("REPORTING-OWNER") == 0 ){
						this->save_new_filer();

						m_state = STATE_REPORTING_OWNER; 
					}
					else if( s_left.compare("FILED BY") == 0 ){
						this->save_new_filer();

						m_state = STATE_FILED_BY; 
					}
					else if( s_left.compare("COMPANY DATA") == 0 ){
						m_state = STATE_FILER_COMPANY_DATA;
					}
					else if( s_left.compare("FILING VALUES") == 0 ){
						m_state = STATE_FILER_FILING_VALUES;
					}
					else if( s_left.compare("BUSINESS ADDRESS") == 0 ){
						m_state = STATE_FILER_BUSINESS_ADDRESS;
					}
					else if( s_left.compare("MAIL ADDRESS") == 0 ){
						m_state = STATE_FILER_MAIL_ADDRESS;
					}
					break;

				case STATE_FILER_COMPANY_DATA:
					if( s_left.compare("FILER") == 0 ){
						this->save_new_filer();

						m_state = STATE_FILER;
						this->begin_new_filer();
					}
					else if( s_left.compare("ISSUER") == 0 ){
						this->save_new_filer();

						m_state = STATE_ISSUER;
					}
					else if( s_left.compare("REPORTING-OWNER") == 0 ){
						this->save_new_filer();

						m_state = STATE_REPORTING_OWNER; 
					}
					else if( s_left.compare("FILED BY") == 0 ){
						this->save_new_filer();

						m_state = STATE_FILED_BY; 
					}
					else if( s_left.compare("COMPANY DATA") == 0 ){
						this->save_new_filer();

						m_state = STATE_FILER_COMPANY_DATA;
					}
					else if( s_left.compare("FILING VALUES") == 0 ){
						m_state = STATE_FILER_FILING_VALUES;
					}
					else if( s_left.compare("BUSINESS ADDRESS") == 0 ){
						m_state = STATE_FILER_BUSINESS_ADDRESS;
					}
					else if( s_left.compare("MAIL ADDRESS") == 0 ){
						m_state = STATE_FILER_MAIL_ADDRESS;
					}
					else{
						std::map<string,string*>::iterator it;

						if( ( it = m_filer_company_data_map.find(s_left) ) != m_filer_company_data_map.end() ){ 
							string* pstring = it->second;
							*pstring = s_right;
						}
					}
					break;

				case STATE_FILER_FILING_VALUES:
					if( s_left.compare("FILER") == 0 ){
						this->save_new_filer();

						m_state = STATE_FILER;
						this->begin_new_filer();
					}
					else if( s_left.compare("ISSUER") == 0 ){
						this->save_new_filer();
						m_state = STATE_ISSUER;
					}
					else if( s_left.compare("REPORTING-OWNER") == 0 ){
						this->save_new_filer();
						m_state = STATE_REPORTING_OWNER; 
					}
					else if( s_left.compare("FILED BY") == 0 ){
						this->save_new_filer();
						m_state = STATE_FILED_BY; 
					}
					else if( s_left.compare("COMPANY DATA") == 0 ){
						this->save_new_filer();
						m_state = STATE_FILER_COMPANY_DATA;
					}
					else if( s_left.compare("FILING VALUES") == 0 ){
						this->save_new_filer();
						m_state = STATE_FILER_FILING_VALUES;
					}
					else if( s_left.compare("BUSINESS ADDRESS") == 0 ){
						m_state = STATE_FILER_BUSINESS_ADDRESS;
					}
					else if( s_left.compare("MAIL ADDRESS") == 0 ){
						m_state = STATE_FILER_MAIL_ADDRESS;
					}
					else{
						std::map<string,string*>::iterator it;
						if( ( it = m_filer_filing_values_map.find(s_left) ) != m_filer_filing_values_map.end() ){ 
							string* pstring = it->second;
							*pstring = s_right;
						}
					}
					break;

				case STATE_FILER_BUSINESS_ADDRESS:
					if( s_left.compare("FILER") == 0 ){
						this->save_new_filer();

						m_state = STATE_FILER;
						this->begin_new_filer();
					}
					else if( s_left.compare("ISSUER") == 0 ){
						this->save_new_filer();
						m_state = STATE_ISSUER;
					}
					else if( s_left.compare("REPORTING-OWNER") == 0 ){
						this->save_new_filer();
						m_state = STATE_REPORTING_OWNER; 
					}
					else if( s_left.compare("FILED BY") == 0 ){
						this->save_new_filer();
						m_state = STATE_FILED_BY; 
					}
					else if( s_left.compare("COMPANY DATA") == 0 ){
						m_state = STATE_FILER_COMPANY_DATA;
					}
					else if( s_left.compare("FILING VALUES") == 0 ){
						m_state = STATE_FILER_FILING_VALUES;
					}
					else if( s_left.compare("BUSINESS ADDRESS") == 0 ){
						m_state = STATE_FILER_BUSINESS_ADDRESS;
					}
					else if( s_left.compare("MAIL ADDRESS") == 0 ){
						m_state = STATE_FILER_MAIL_ADDRESS;
					}
					else{
						std::map<string,string*>::iterator it;
						if( ( it = m_filer_business_address_map.find(s_left) ) != m_filer_business_address_map.end() ){ 
							string* pstring = it->second;
							*pstring = s_right;
						}
					}
					break;

				case STATE_FILER_MAIL_ADDRESS:
					if( s_left.compare("FILER") == 0 ){
						this->save_new_filer();

						m_state = STATE_FILER;
						this->begin_new_filer();
					}
					else if( s_left.compare("ISSUER") == 0 ){
						this->save_new_filer();
						m_state = STATE_ISSUER;
					}
					else if( s_left.compare("REPORTING-OWNER") == 0 ){
						this->save_new_filer();
						m_state = STATE_REPORTING_OWNER; 
					}
					else if( s_left.compare("FILED BY") == 0 ){
						this->save_new_filer();
						m_state = STATE_FILED_BY; 
					}
					else if( s_left.compare("COMPANY DATA") == 0 ){
						m_state = STATE_FILER_COMPANY_DATA;
					}
					else if( s_left.compare("FILING VALUES") == 0 ){
						m_state = STATE_FILER_FILING_VALUES;
					}
					else if( s_left.compare("BUSINESS ADDRESS") == 0 ){
						m_state = STATE_FILER_BUSINESS_ADDRESS;
					}
					else if( s_left.compare("MAIL ADDRESS") == 0 ){
						m_state = STATE_FILER_MAIL_ADDRESS;
					}
					else{
						std::map<string,string*>::iterator it;
						if( ( it = m_filer_mail_address_map.find(s_left) ) != m_filer_mail_address_map.end() ){ 
							string* pstring = it->second;
							*pstring = s_right;
						}
					}
					break;

				case STATE_ISSUER:
					if( s_left.compare("FILER") == 0 ){
						m_state = STATE_FILER;
						this->begin_new_filer();
					}
					else if( s_left.compare("ISSUER") == 0 ){
						m_state = STATE_ISSUER;
					}
					else if( s_left.compare("REPORTING-OWNER") == 0 ){
						m_state = STATE_REPORTING_OWNER; 
					}
					else if( s_left.compare("FILED BY") == 0 ){
						m_state = STATE_FILED_BY; 
					}
					else if( s_left.compare("COMPANY DATA") == 0 ){
						m_state = STATE_ISSUER_COMPANY_DATA;
					}
					else if( s_left.compare("BUSINESS ADDRESS") == 0 ){
						m_state = STATE_ISSUER_BUSINESS_ADDRESS;
					}
					else if( s_left.compare("MAIL ADDRESS") == 0 ){
						m_state = STATE_ISSUER_MAIL_ADDRESS;
					}
					break;

				case STATE_ISSUER_COMPANY_DATA:
					if( s_left.compare("FILER") == 0 ){
						m_state = STATE_FILER;
						this->begin_new_filer();
					}
					else if( s_left.compare("ISSUER") == 0 ){
						m_state = STATE_ISSUER;
					}
					else if( s_left.compare("REPORTING-OWNER") == 0 ){
						m_state = STATE_REPORTING_OWNER; 
					}
					else if( s_left.compare("FILED BY") == 0 ){
						m_state = STATE_FILED_BY; 
					}
					else if( s_left.compare("COMPANY DATA") == 0 ){
						m_state = STATE_ISSUER_COMPANY_DATA;
					}
					else if( s_left.compare("BUSINESS ADDRESS") == 0 ){
						m_state = STATE_ISSUER_BUSINESS_ADDRESS;
					}
					else if( s_left.compare("MAIL ADDRESS") == 0 ){
						m_state = STATE_ISSUER_MAIL_ADDRESS;
					}
					else{
						std::map<string,string*>::iterator it;

						if( ( it = m_issuer_company_data_map.find(s_left) ) != m_issuer_company_data_map.end() ){ 
							string* pstring = it->second;
							*pstring = s_right;
						}
					}
					break;

				case STATE_ISSUER_BUSINESS_ADDRESS:
					if( s_left.compare("FILER") == 0 ){
						m_state = STATE_FILER;
						this->begin_new_filer();
					}
					else if( s_left.compare("ISSUER") == 0 ){
						m_state = STATE_ISSUER;
					}
					else if( s_left.compare("REPORTING-OWNER") == 0 ){
						m_state = STATE_REPORTING_OWNER; 
					}
					else if( s_left.compare("FILED BY") == 0 ){
						m_state = STATE_FILED_BY; 
					}
					else if( s_left.compare("COMPANY DATA") == 0 ){
						m_state = STATE_ISSUER_COMPANY_DATA;
					}
					else if( s_left.compare("BUSINESS ADDRESS") == 0 ){
						m_state = STATE_ISSUER_BUSINESS_ADDRESS;
					}
					else if( s_left.compare("MAIL ADDRESS") == 0 ){
						m_state = STATE_ISSUER_MAIL_ADDRESS;
					}
					else{
						std::map<string,string*>::iterator it;

						if( ( it = m_issuer_business_address_map.find(s_left) ) != m_issuer_business_address_map.end() ){ 
							string* pstring = it->second;
							*pstring = s_right;
						}
					}
					break;

				case STATE_ISSUER_MAIL_ADDRESS:
					if( s_left.compare("FILER") == 0 ){
						m_state = STATE_FILER;
						this->begin_new_filer();
					}
					else if( s_left.compare("ISSUER") == 0 ){
						m_state = STATE_ISSUER;
					}
					else if( s_left.compare("REPORTING-OWNER") == 0 ){
						m_state = STATE_REPORTING_OWNER; 
					}
					else if( s_left.compare("FILED BY") == 0 ){
						m_state = STATE_FILED_BY; 
					}
					else if( s_left.compare("COMPANY DATA") == 0 ){
						m_state = STATE_ISSUER_COMPANY_DATA;
					}
					else if( s_left.compare("BUSINESS ADDRESS") == 0 ){
						m_state = STATE_ISSUER_BUSINESS_ADDRESS;
					}
					else if( s_left.compare("MAIL ADDRESS") == 0 ){
						m_state = STATE_ISSUER_MAIL_ADDRESS;
					}
					else{
						std::map<string,string*>::iterator it;

						if( ( it = m_issuer_mail_address_map.find(s_left) ) != m_issuer_mail_address_map.end() ){ 
							string* pstring = it->second;
							*pstring = s_right;
						}
					}
					break;

				case STATE_REPORTING_OWNER:
					if( s_left.compare("FILER") == 0 ){
						m_state = STATE_FILER;
						this->begin_new_filer();
					}
					else if( s_left.compare("ISSUER") == 0 ){
						m_state = STATE_ISSUER;
					}
					else if( s_left.compare("REPORTING-OWNER") == 0 ){
						m_state = STATE_REPORTING_OWNER; 
					}
					else if( s_left.compare("FILED BY") == 0 ){
						m_state = STATE_FILED_BY; 
					}
					else if( s_left.compare("OWNER DATA") == 0 ){
						m_state = STATE_REPORTING_OWNER_OWNER_DATA;
					}
					else if( s_left.compare("FILING VALUES") == 0 ){
						m_state = STATE_REPORTING_OWNER_FILING_VALUES;
					}
					else if( s_left.compare("MAIL ADDRESS") == 0 ){
						m_state = STATE_REPORTING_OWNER_MAIL_ADDRESS;
					}
					else if( s_left.compare("BUSINESS ADDRESS") == 0 ){
						m_state = STATE_REPORTING_OWNER_BUSINESS_ADDRESS;
					}
					break;

				case STATE_REPORTING_OWNER_OWNER_DATA:
					if( s_left.compare("FILER") == 0 ){
						m_state = STATE_FILER;
						this->begin_new_filer();
					}
					else if( s_left.compare("ISSUER") == 0 ){
						m_state = STATE_ISSUER;
					}
					else if( s_left.compare("REPORTING-OWNER") == 0 ){
						m_state = STATE_REPORTING_OWNER; 
					}
					else if( s_left.compare("FILED BY") == 0 ){
						m_state = STATE_FILED_BY; 
					}
					else if( s_left.compare("OWNER DATA") == 0 ){
						m_state = STATE_REPORTING_OWNER_OWNER_DATA;
					}
					else if( s_left.compare("FILING VALUES") == 0 ){
						m_state = STATE_REPORTING_OWNER_FILING_VALUES;
					}
					else if( s_left.compare("MAIL ADDRESS") == 0 ){
						m_state = STATE_REPORTING_OWNER_MAIL_ADDRESS;
					}
					else if( s_left.compare("BUSINESS ADDRESS") == 0 ){
						m_state = STATE_REPORTING_OWNER_BUSINESS_ADDRESS;
					}
					else{
						std::map<string,string*>::iterator it;

						if( ( it = m_reporting_owner_owner_data_map.find(s_left) ) != m_reporting_owner_owner_data_map.end() ){ 
							string* pstring = it->second;
							*pstring = s_right;
						}
					}
					break;

				case STATE_REPORTING_OWNER_FILING_VALUES:
					if( s_left.compare("FILER") == 0 ){
						m_state = STATE_FILER;
						this->begin_new_filer();
					}
					else if( s_left.compare("ISSUER") == 0 ){
						m_state = STATE_ISSUER;
					}
					else if( s_left.compare("REPORTING-OWNER") == 0 ){
						m_state = STATE_REPORTING_OWNER; 
					}
					else if( s_left.compare("FILED BY") == 0 ){
						m_state = STATE_FILED_BY; 
					}
					else if( s_left.compare("OWNER DATA") == 0 ){
						m_state = STATE_REPORTING_OWNER_OWNER_DATA;
					}
					else if( s_left.compare("FILING VALUES") == 0 ){
						m_state = STATE_REPORTING_OWNER_FILING_VALUES;
					}
					else if( s_left.compare("MAIL ADDRESS") == 0 ){
						m_state = STATE_REPORTING_OWNER_MAIL_ADDRESS;
					}
					else if( s_left.compare("BUSINESS ADDRESS") == 0 ){
						m_state = STATE_REPORTING_OWNER_BUSINESS_ADDRESS;
					}
					else{
						std::map<string,string*>::iterator it;

						if( ( it = m_reporting_owner_filing_values_map.find(s_left) ) != m_reporting_owner_filing_values_map.end() ){ 
							string* pstring = it->second;
							*pstring = s_right;
						}
					}
					break;

				case STATE_REPORTING_OWNER_MAIL_ADDRESS:
					if( s_left.compare("FILER") == 0 ){
						m_state = STATE_FILER;
						this->begin_new_filer();
					}
					else if( s_left.compare("ISSUER") == 0 ){
						m_state = STATE_ISSUER;
					}
					else if( s_left.compare("REPORTING-OWNER") == 0 ){
						m_state = STATE_REPORTING_OWNER; 
					}
					else if( s_left.compare("FILED BY") == 0 ){
						m_state = STATE_FILED_BY; 
					}
					else if( s_left.compare("OWNER DATA") == 0 ){
						m_state = STATE_REPORTING_OWNER_OWNER_DATA;
					}
					else if( s_left.compare("FILING VALUES") == 0 ){
						m_state = STATE_REPORTING_OWNER_FILING_VALUES;
					}
					else if( s_left.compare("MAIL ADDRESS") == 0 ){
						m_state = STATE_REPORTING_OWNER_MAIL_ADDRESS;
					}
					else if( s_left.compare("BUSINESS ADDRESS") == 0 ){
						m_state = STATE_REPORTING_OWNER_BUSINESS_ADDRESS;
					}
					else{
						std::map<string,string*>::iterator it;

						if( ( it = m_reporting_owner_mail_address_map.find(s_left) ) != m_reporting_owner_mail_address_map.end() ){ 
							string* pstring = it->second;
							*pstring = s_right;
						}
					}
					break;

				case STATE_REPORTING_OWNER_BUSINESS_ADDRESS:
					if( s_left.compare("FILER") == 0 ){
						m_state = STATE_FILER;
						this->begin_new_filer();
					}
					else if( s_left.compare("ISSUER") == 0 ){
						m_state = STATE_ISSUER;
					}
					else if( s_left.compare("REPORTING-OWNER") == 0 ){
						m_state = STATE_REPORTING_OWNER; 
					}
					else if( s_left.compare("FILED BY") == 0 ){
						m_state = STATE_FILED_BY; 
					}
					else if( s_left.compare("OWNER DATA") == 0 ){
						m_state = STATE_REPORTING_OWNER_OWNER_DATA;
					}
					else if( s_left.compare("FILING VALUES") == 0 ){
						m_state = STATE_REPORTING_OWNER_FILING_VALUES;
					}
					else if( s_left.compare("MAIL ADDRESS") == 0 ){
						m_state = STATE_REPORTING_OWNER_MAIL_ADDRESS;
					}
					else if( s_left.compare("BUSINESS ADDRESS") == 0 ){
						m_state = STATE_REPORTING_OWNER_BUSINESS_ADDRESS;
					}
					else{
						std::map<string,string*>::iterator it;

						if( ( it = m_reporting_owner_business_address_map.find(s_left) ) != m_reporting_owner_business_address_map.end() ){ 
							string* pstring = it->second;
							*pstring = s_right;
						}
					}
					break;

				/* -- FILED_BY - BEGIN -- */

				case STATE_FILED_BY:
					if( s_left.compare("FILER") == 0 ){
						m_state = STATE_FILER;
						this->begin_new_filer();
					}
					else if( s_left.compare("ISSUER") == 0 ){
						m_state = STATE_ISSUER;
					}
					else if( s_left.compare("REPORTING-OWNER") == 0 ){
						m_state = STATE_REPORTING_OWNER; 
					}
					else if( s_left.compare("FILED BY") == 0 ){
						m_state = STATE_FILED_BY; 
					}
					else if( s_left.compare("COMPANY DATA") == 0 ){
						m_state = STATE_FILED_BY_COMPANY_DATA;
					}
					else if( s_left.compare("FILING VALUES") == 0 ){
						m_state = STATE_FILED_BY_FILING_VALUES;
					}
					else if( s_left.compare("BUSINESS ADDRESS") == 0 ){
						m_state = STATE_FILED_BY_BUSINESS_ADDRESS;
					}
					else if( s_left.compare("MAIL ADDRESS") == 0 ){
						m_state = STATE_FILED_BY_MAIL_ADDRESS;
					}
					break;

				case STATE_FILED_BY_COMPANY_DATA:
					if( s_left.compare("FILER") == 0 ){
						m_state = STATE_FILER;
						this->begin_new_filer();
					}
					else if( s_left.compare("ISSUER") == 0 ){
						m_state = STATE_ISSUER;
					}
					else if( s_left.compare("REPORTING-OWNER") == 0 ){
						m_state = STATE_REPORTING_OWNER; 
					}
					else if( s_left.compare("FILED BY") == 0 ){
						m_state = STATE_FILED_BY; 
					}
					else if( s_left.compare("COMPANY DATA") == 0 ){
						m_state = STATE_FILED_BY_COMPANY_DATA;
					}
					else if( s_left.compare("FILING VALUES") == 0 ){
						m_state = STATE_FILED_BY_FILING_VALUES;
					}
					else if( s_left.compare("BUSINESS ADDRESS") == 0 ){
						m_state = STATE_FILED_BY_BUSINESS_ADDRESS;
					}
					else if( s_left.compare("MAIL ADDRESS") == 0 ){
						m_state = STATE_FILED_BY_MAIL_ADDRESS;
					}
					else{
						std::map<string,string*>::iterator it;

						if( ( it = m_filed_by_company_data_map.find(s_left) ) != m_filed_by_company_data_map.end() ){ 
							string* pstring = it->second;
							*pstring = s_right;
						}
					}
					break;

				case STATE_FILED_BY_FILING_VALUES:
					if( s_left.compare("FILER") == 0 ){
						m_state = STATE_FILER;
						this->begin_new_filer();
					}
					else if( s_left.compare("ISSUER") == 0 ){
						m_state = STATE_ISSUER;
					}
					else if( s_left.compare("REPORTING-OWNER") == 0 ){
						m_state = STATE_REPORTING_OWNER; 
					}
					else if( s_left.compare("FILED BY") == 0 ){
						m_state = STATE_FILED_BY; 
					}
					else if( s_left.compare("COMPANY DATA") == 0 ){
						m_state = STATE_FILED_BY_COMPANY_DATA;
					}
					else if( s_left.compare("FILING VALUES") == 0 ){
						m_state = STATE_FILED_BY_FILING_VALUES;
					}
					else if( s_left.compare("BUSINESS ADDRESS") == 0 ){
						m_state = STATE_FILED_BY_BUSINESS_ADDRESS;
					}
					else if( s_left.compare("MAIL ADDRESS") == 0 ){
						m_state = STATE_FILED_BY_MAIL_ADDRESS;
					}
					else{
						std::map<string,string*>::iterator it;
						if( ( it = m_filed_by_filing_values_map.find(s_left) ) != m_filed_by_filing_values_map.end() ){ 
							string* pstring = it->second;
							*pstring = s_right;
						}
					}
					break;

				case STATE_FILED_BY_BUSINESS_ADDRESS:
					if( s_left.compare("FILER") == 0 ){
						m_state = STATE_FILER;
						this->begin_new_filer();
					}
					else if( s_left.compare("ISSUER") == 0 ){
						m_state = STATE_ISSUER;
					}
					else if( s_left.compare("REPORTING-OWNER") == 0 ){
						m_state = STATE_REPORTING_OWNER; 
					}
					else if( s_left.compare("FILED BY") == 0 ){
						m_state = STATE_FILED_BY; 
					}
					else if( s_left.compare("COMPANY DATA") == 0 ){
						m_state = STATE_FILED_BY_COMPANY_DATA;
					}
					else if( s_left.compare("FILING VALUES") == 0 ){
						m_state = STATE_FILED_BY_FILING_VALUES;
					}
					else if( s_left.compare("BUSINESS ADDRESS") == 0 ){
						m_state = STATE_FILED_BY_BUSINESS_ADDRESS;
					}
					else if( s_left.compare("MAIL ADDRESS") == 0 ){
						m_state = STATE_FILED_BY_MAIL_ADDRESS;
					}
					else{
						std::map<string,string*>::iterator it;
						if( ( it = m_filed_by_business_address_map.find(s_left) ) != m_filed_by_business_address_map.end() ){ 
							string* pstring = it->second;
							*pstring = s_right;
						}
					}
					break;

				case STATE_FILED_BY_MAIL_ADDRESS:
					if( s_left.compare("FILER") == 0 ){
						m_state = STATE_FILER;
						this->begin_new_filer();
					}
					else if( s_left.compare("ISSUER") == 0 ){
						m_state = STATE_ISSUER;
					}
					else if( s_left.compare("REPORTING-OWNER") == 0 ){
						m_state = STATE_REPORTING_OWNER; 
					}
					else if( s_left.compare("FILED BY") == 0 ){
						m_state = STATE_FILED_BY; 
					}
					else if( s_left.compare("COMPANY DATA") == 0 ){
						m_state = STATE_FILED_BY_COMPANY_DATA;
					}
					else if( s_left.compare("FILING VALUES") == 0 ){
						m_state = STATE_FILED_BY_FILING_VALUES;
					}
					else if( s_left.compare("BUSINESS ADDRESS") == 0 ){
						m_state = STATE_FILED_BY_BUSINESS_ADDRESS;
					}
					else if( s_left.compare("MAIL ADDRESS") == 0 ){
						m_state = STATE_FILED_BY_MAIL_ADDRESS;
					}
					else{
						std::map<string,string*>::iterator it;
						if( ( it = m_filed_by_mail_address_map.find(s_left) ) != m_filed_by_mail_address_map.end() ){ 
							string* pstring = it->second;
							*pstring = s_right;
						}
					}
					break;

				/* -- FILED_BY - END -- */

			}/* switch( m_state ) */

			
			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): line " << m_line_number << ": AFTER: state " << stateTypeToString( m_state ) << "..." << endl;
			}

			return m_state;
		}/* parseLine() */
