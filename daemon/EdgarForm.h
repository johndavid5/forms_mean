#ifndef _EDGAR_FORM_H_
#define _EDGAR_FORM_H_

#include "logger.h"
#include <string>
#include <memory> // shared_ptr<>, etc.
//using namespace std;
//using namespace std::tr1; // shared_ptr -- gcc

/** Parse the <SEC-HEADER>...</SEC-HEADER> of an Edgar Form... 
*
* EXAMPLES:
* ------------------------------------------------------------------
* URL: ftp://ftp.sec.gov/edgar/data/1000621/9999999997-15-015521.txt
* ------------------------------------------------------------------
* <SEC-DOCUMENT>9999999997-15-015521.txt : 20151210
* <SEC-HEADER>9999999997-15-015521.hdr.sgml : 20151210
* <ACCEPTANCE-DATETIME>20151210121600
* ACCESSION NUMBER:		9999999997-15-015521
* CONFORMED SUBMISSION TYPE:	REVOKED
* PUBLIC DOCUMENT COUNT:		1
* FILED AS OF DATE:		20061108
* DATE AS OF CHANGE:		20151210
* <ACTION-DATE>20061108
* <RECEIVED-DATE>20151210
* 
* FILER:
* 
* 	COMPANY DATA:	
* 		COMPANY CONFORMED NAME:			GEOGRAPHICS INC
* 		CENTRAL INDEX KEY:			0001000621
* 		STANDARD INDUSTRIAL CLASSIFICATION:	WHOLESALE-PAPER AND PAPER PRODUCTS [5110]
* 		IRS NUMBER:				870305614
* 		STATE OF INCORPORATION:			WI
* 		FISCAL YEAR END:			0331
* 
* 	FILING VALUES:
* 		FORM TYPE:		REVOKED
* 		SEC ACT:		1934 Act
* 		SEC FILE NUMBER:	000-26756
* 		FILM NUMBER:		151280314
* 
* 	BUSINESS ADDRESS:	
* 		STREET 1:		1555 ODELL RD
* 		STREET 2:		P O BOX 1750
* 		CITY:			BLAINE
* 		STATE:			WA
* 		ZIP:			98230
* 		BUSINESS PHONE:		3603326711
* 
* 	MAIL ADDRESS:	
* 		STREET 1:		1555 ODELL RD
* 		CITY:			BLAINE
* 		STATE:			WA
* 		ZIP:			98230
* </SEC-HEADER>
* 
*
* ------------------------------------------------------------------
* URL:ftp://ftp.sec.gov/edgar/data/1001250/0001001250-15-000126.txt
* ------------------------------------------------------------------
* <SEC-DOCUMENT>0001001250-15-000126.txt : 20151210
* <SEC-HEADER>0001001250-15-000126.hdr.sgml : 20151210
* <ACCEPTANCE-DATETIME>20151210162006
* ACCESSION NUMBER:		0001001250-15-000126
* CONFORMED SUBMISSION TYPE:	4
* PUBLIC DOCUMENT COUNT:		1
* CONFORMED PERIOD OF REPORT:	20151130
* FILED AS OF DATE:		20151210
* DATE AS OF CHANGE:		20151210
* 
* ISSUER:		
* 
* 	COMPANY DATA:	
* 		COMPANY CONFORMED NAME:			ESTEE LAUDER COMPANIES INC
* 		CENTRAL INDEX KEY:			0001001250
* 		STANDARD INDUSTRIAL CLASSIFICATION:	PERFUMES, COSMETICS & OTHER TOILET PREPARATIONS [2844]
* 		IRS NUMBER:				112408943
* 		STATE OF INCORPORATION:			DE
* 		FISCAL YEAR END:			0630
* 
* 	BUSINESS ADDRESS:	
* 		STREET 1:		767 FIFTH AVE
* 		CITY:			NEW YORK
* 		STATE:			NY
* 		ZIP:			10153
* 		BUSINESS PHONE:		2125724200
* 
* 	MAIL ADDRESS:	
* 		STREET 1:		767 FIFTH AVE
* 		CITY:			NEW YORK
* 		STATE:			NY
* 		ZIP:			10153
* 
* REPORTING-OWNER:	
* 
* 	OWNER DATA:	
* 		COMPANY CONFORMED NAME:			MOSS SARA E
* 		CENTRAL INDEX KEY:			0001204538
* 
* 	FILING VALUES:
* 		FORM TYPE:		4
* 		SEC ACT:		1934 Act
* 		SEC FILE NUMBER:	001-14064
* 		FILM NUMBER:		151281069
* </SEC-HEADER>
*
* ------------------------------------------------------------------
* URL: ftp://ftp.sec.gov/edgar/data/1493761/0000899243-15-002897.txt
* ------------------------------------------------------------------
* <SEC-DOCUMENT>0000899243-15-002897.txt : 20150811
* <SEC-HEADER>0000899243-15-002897.hdr.sgml : 20150811
* <ACCEPTANCE-DATETIME>20150811215444
* ACCESSION NUMBER:		0000899243-15-002897
* CONFORMED SUBMISSION TYPE:	4
* PUBLIC DOCUMENT COUNT:		1
* CONFORMED PERIOD OF REPORT:	20150811
* FILED AS OF DATE:		20150811
* DATE AS OF CHANGE:		20150811
* 
* ISSUER:		
* 
* 	COMPANY DATA:	
* 		COMPANY CONFORMED NAME:			Turtle Beach Corp
* 		CENTRAL INDEX KEY:			0001493761
* 		STANDARD INDUSTRIAL CLASSIFICATION:	COMMUNICATIONS EQUIPMENT, NEC [3669]
* 		IRS NUMBER:				272767540
* 		STATE OF INCORPORATION:			NV
* 		FISCAL YEAR END:			1231
* 
* 	BUSINESS ADDRESS:	
* 		STREET 1:		100 SUMMIT LAKE DRIVE
* 		STREET 2:		SUITE 100
* 		CITY:			VALHALLA
* 		STATE:			NY
* 		ZIP:			10595
* 		BUSINESS PHONE:		914-345-2255
* 
* 	MAIL ADDRESS:	
* 		STREET 1:		100 SUMMIT LAKE DRIVE
* 		STREET 2:		SUITE 100
* 		CITY:			VALHALLA
* 		STATE:			NY
* 		ZIP:			10595
* 
* 	FORMER COMPANY:	
* 		FORMER CONFORMED NAME:	Parametric Sound Corp
* 		DATE OF NAME CHANGE:	20100609
* 
* REPORTING-OWNER:	
* 
* 	OWNER DATA:	
* 		COMPANY CONFORMED NAME:			Stark Juergen M.
* 		CENTRAL INDEX KEY:			0001595670
* 
* 	FILING VALUES:
* 		FORM TYPE:		4
* 		SEC ACT:		1934 Act
* 		SEC FILE NUMBER:	001-35465
* 		FILM NUMBER:		151045407
* 
* 	MAIL ADDRESS:	
* 		STREET 1:		100 SUMMIT LAKE DRIVE, SUITE 100
* 		CITY:			VALHALLA
* 		STATE:			NY
* 		ZIP:			10594
* </SEC-HEADER>
*	http://www.sec.gov/Archives/edgar/data/1403385/0000950157-15-000937-index.htm 
*/

namespace JDA{ 

	class EdgarForm {

	public:

		
		enum StateType {

			STATE_START,

			STATE_IN_SEC_HEADER,
			STATE_DONE_WITH_SEC_HEADER,

			STATE_FILER,
				STATE_FILER_COMPANY_DATA,
				STATE_FILER_FILING_VALUES,
				STATE_FILER_BUSINESS_ADDRESS,
				STATE_FILER_MAIL_ADDRESS,

			STATE_ISSUER,
				STATE_ISSUER_COMPANY_DATA,
				STATE_ISSUER_BUSINESS_ADDRESS,
				STATE_ISSUER_MAIL_ADDRESS,

			STATE_REPORTING_OWNER,
				STATE_REPORTING_OWNER_OWNER_DATA,
				STATE_REPORTING_OWNER_FILING_VALUES,
				STATE_REPORTING_OWNER_MAIL_ADDRESS,
				STATE_REPORTING_OWNER_BUSINESS_ADDRESS, /* Do you think...? */

			STATE_FILED_BY, /* A lot like FILER, so we'll re-use the FILER struct. */
				STATE_FILED_BY_COMPANY_DATA,
				STATE_FILED_BY_FILING_VALUES,
				STATE_FILED_BY_BUSINESS_ADDRESS,
				STATE_FILED_BY_MAIL_ADDRESS,
		};

		static const char* stateTypeEnumStrings[];

		static string stateTypeToString( StateType stateType ){
			return stateTypeEnumStrings[ stateType ];
		}

		/* --------------------------------------------------------------- */

		struct OWNER_DATA {

			string company_conformed_name;
			string central_index_key;

			string toString( int iIndentLevel = 0, string sIndentPrefix = "\t" ){

				string sPrefix = JDA::Utils::stringMultiply( sIndentPrefix, iIndentLevel );

				ostringstream oss_out;
				oss_out << sIndentPrefix << "company_conformed_name = \"" << company_conformed_name << "\"\n";
				oss_out << sIndentPrefix << "central_index_key = \"" << central_index_key << "\"";
				//oss_out << sIndentPrefix << "xxx = \"" << xxx << "\"\n";
				return oss_out.str();	
			}
		}; /* OWNER_DATA */

		struct COMPANY_DATA {

			string company_conformed_name;
			string central_index_key;
			string standard_industrial_classification;
			string irs_number;
			string state_of_incorporation;
			string fiscal_year_end;

			string toString( int iIndentLevel = 0, string sIndentPrefix = "\t" ){

				string sPrefix = JDA::Utils::stringMultiply( sIndentPrefix, iIndentLevel );

				ostringstream oss_out;
				oss_out << sIndentPrefix << "company_conformed_name = \"" << company_conformed_name << "\"\n";
				oss_out << sIndentPrefix << "central_index_key = \"" << central_index_key << "\"\n";
				oss_out << sIndentPrefix << "standard_industrial_classification = \"" << standard_industrial_classification << "\"\n";
				oss_out << sIndentPrefix << "irs_number = \"" << irs_number << "\"\n";
				oss_out << sIndentPrefix << "state_of_incorporation = \"" << state_of_incorporation << "\"\n";
				oss_out << sIndentPrefix << "fiscal_year_end = \"" << fiscal_year_end << "\"";
				//oss_out << sIndentPrefix << "xxx = \"" << xxx << "\"\n";
				return oss_out.str();	
			}
		}; /* COMPANY_DATA */

		struct FILING_VALUES {

			string form_type;
			string sec_act;
			string sec_file_number;
			string film_number;

			string toString( int iIndentLevel = 0, string sIndentPrefix = "\t" ){

				string sPrefix = JDA::Utils::stringMultiply( sIndentPrefix, iIndentLevel );

				ostringstream oss_out;
				oss_out << sPrefix << "form_type = \"" << form_type << "\"\n";
				oss_out << sPrefix << "sec_act = \"" << sec_act << "\"\n";
				oss_out << sPrefix << "sec_file_number = \"" << sec_file_number << "\"\n";
				oss_out << sPrefix << "film_number = \"" << film_number << "\"";
				return oss_out.str();	
			}
		}; /* FILING_VALUES */

		struct BUSINESS_ADDRESS {

			string street_1;
			string street_2;
			string city;
			string state;
			string zip;
			string business_phone;

			string toString( int iIndentLevel = 0, string sIndentPrefix = "\t" ){

				string sPrefix = JDA::Utils::stringMultiply( sIndentPrefix, iIndentLevel );

				ostringstream oss_out;
				oss_out << sPrefix << "street_1 = \"" << street_1 << "\"\n";
				oss_out << sPrefix << "street_2 = \"" << street_2 << "\"\n";
				oss_out << sPrefix << "city = \"" << city << "\"\n";
				oss_out << sPrefix << "state = \"" << state << "\"\n";
				oss_out << sPrefix << "zip = \"" << zip << "\"\n";
				oss_out << sPrefix << "business_phone = \"" << business_phone << "\"";
				return oss_out.str();	
			}
		}; /* BUSINESS_ADDRESS */

		struct MAIL_ADDRESS {

			string street_1;
			string street_2;
			string city;
			string state;
			string zip;

			string toString( int iIndentLevel = 0, string sIndentPrefix = "\t" ){

				string sPrefix = JDA::Utils::stringMultiply( sIndentPrefix, iIndentLevel );

				ostringstream oss_out;
				oss_out << sPrefix << "street_1 = \"" << street_1 << "\"\n";
				oss_out << sPrefix << "street_2 = \"" << street_2 << "\"\n";
				oss_out << sPrefix << "city = \"" << city << "\"\n";
				oss_out << sPrefix << "state = \"" << state << "\"\n";
				oss_out << sPrefix << "zip = \"" << zip << "\"";
				return oss_out.str();	
			}
		}; /* MAIL_ADDRESS */

		/* --------------------------------------------------------------- */

		struct FILER {
			COMPANY_DATA company_data;	
			FILING_VALUES filing_values;
			BUSINESS_ADDRESS business_address;
			MAIL_ADDRESS mail_address;
			
			string toString( int iIndentLevel = 0, string sIndentPrefix = "\t" ){

				string sPrefix = JDA::Utils::stringMultiply( sIndentPrefix, iIndentLevel );

				ostringstream oss_out;
				oss_out << sPrefix << "company_data:\n";
				oss_out << company_data.toString( iIndentLevel+1 ) << "\n";
				oss_out << sPrefix << "filing_values:\n";
				oss_out << filing_values.toString( iIndentLevel+1 ) << "\n";
				oss_out << sPrefix << "business_address:\n";
				oss_out << business_address.toString( iIndentLevel+1 ) << "\n";
				oss_out << sPrefix << "mail_address:\n";
				oss_out << mail_address.toString( iIndentLevel+1 ) << "\n";
				return oss_out.str();	
			}
		}; /* FILER */

		struct ISSUER {

			COMPANY_DATA company_data;	
			BUSINESS_ADDRESS business_address;	
			MAIL_ADDRESS mail_address;	

			string toString( int iIndentLevel = 0, string sIndentPrefix = "\t" ){

				string sPrefix = JDA::Utils::stringMultiply( sIndentPrefix, iIndentLevel );

				ostringstream oss_out;
				oss_out << sPrefix << "company_data:\n";
				oss_out << company_data.toString( iIndentLevel+1 ) << "\n";
				oss_out << sPrefix << "business_address:\n";
				oss_out << business_address.toString( iIndentLevel+1 ) << "\n";
				oss_out << sPrefix << "mail_address:\n";
				oss_out << mail_address.toString( iIndentLevel+1 ) << "\n";
				return oss_out.str();	
			}

		}; /* ISSUER */

		struct REPORTING_OWNER {

			OWNER_DATA owner_data;	
			FILING_VALUES filing_values;
			MAIL_ADDRESS mail_address;	
			BUSINESS_ADDRESS business_address; /* Do you think...? */

			string toString( int iIndentLevel = 0, string sIndentPrefix = "\t" ){

				string sPrefix = JDA::Utils::stringMultiply( sIndentPrefix, iIndentLevel );

				ostringstream oss_out;
				oss_out << sPrefix << "owner_data:\n";
				oss_out << owner_data.toString( iIndentLevel+1 ) << "\n";
				oss_out << sPrefix << "filing_values:\n";
				oss_out << filing_values.toString( iIndentLevel+1 ) << "\n";
				oss_out << sPrefix << "mail_address:\n";
				oss_out << mail_address.toString( iIndentLevel+1 ) << "\n";
				oss_out << sPrefix << "business_address:\n";
				oss_out << business_address.toString( iIndentLevel+1 ) << "\n";
				return oss_out.str();	
			}

		}; /* REPORTING_OWNER */


		/* --------------------------------------------------------------- */

		string m_s_accession_number;

		shared_ptr<FILER> m_new_filer;
		vector<shared_ptr<FILER>> m_filers;

		map<string,string*> m_filer_company_data_map;
		map<string,string*> m_filer_filing_values_map;
		map<string,string*> m_filer_business_address_map;
		map<string,string*> m_filer_mail_address_map;

		ISSUER m_issuer;

		map<string,string*> m_issuer_company_data_map;
		map<string,string*> m_issuer_business_address_map;
		map<string,string*> m_issuer_mail_address_map;

		REPORTING_OWNER m_reporting_owner;

		map<string,string*> m_reporting_owner_owner_data_map;
		map<string,string*> m_reporting_owner_filing_values_map;
		map<string,string*> m_reporting_owner_mail_address_map;
		map<string,string*> m_reporting_owner_business_address_map;

		FILER m_filed_by;

		map<string,string*> m_filed_by_company_data_map;
		map<string,string*> m_filed_by_filing_values_map;
		map<string,string*> m_filed_by_business_address_map;
		map<string,string*> m_filed_by_mail_address_map;

		/* --------------------------------------------------------------- */

		EdgarForm(); /* constructor */

		StateType parseLine( const string& le_line );			

		/**
		* For example, 
		*    extract_sic_code( "WHOLESALE-PAPER AND PAPER PRODUCTS [5110]" ) = "5110"
		*    extract_sic_code( "WHOLESALE-PAPER AND PAPER PRODUCTS [WXYZ]" ) = "XYZ"
		*    extract_sic_code( "WHOLESALE-PAPER AND PAPER PRODUCTS" ) = ""
		*/
		static string extract_sic_code( const string& standard_industrial_classification ); 

		static string filers_to_string( vector<shared_ptr<FILER>>& filers );

		protected: 

			static bool splitIt( const string& le_input, string* le_left, string* le_right, const string& le_splitteo = ":" );

			void begin_new_filer();
			void save_new_filer();

			void map_filer( FILER* p_filer );

			StateType m_state;
			size_t m_line_number;
				
	}; /* class EdgarForm */

} /* namespace JDA */

#endif /* #ifndef _EDGAR_FORM_H_ */
