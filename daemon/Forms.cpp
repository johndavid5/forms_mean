#ifdef _WIN32
	// Gotta include <winsock2.h> before 
	// <windows.h>...avoids winsock already
	// defined error when including mongoc.h 
	#include <winsock2.h>
#endif

#include "Forms.h"
#include "EdgarForm.h"
#include "FormsMeanUtils.h"
#include "FtpClient.h"

namespace JDA { 

/** Used to process lines in an EDGAR index... */
class MyFtpIndexClientCallback : public JDA::FtpClient::IFtpClientCallback 
{
	protected:

		size_t m_i_iteration_count; 
		size_t m_i_byte_count; 

		size_t m_i_query_attempt_count;
		size_t m_i_query_succeed_count;

		std::vector<std::string> mr_fields; // Use to split the line...

		JDA::Logger* m_p_logger;
		JDA::Forms* m_p_forms;
		string m_s_url; // For informational db-logging purposes only, this callback does not directly use the url... 

    	const static int CIK_IDX = 0;	
		const static int COMPANY_NAME_IDX = 1;	
		const static int FORM_TYPE_IDX = 2;	
		const static int DATE_FILED_IDX = 3;	
		const static int FILE_NAME_IDX = 4;	

	public:

		MyFtpIndexClientCallback(): m_i_iteration_count(0), m_i_byte_count(0),
			m_i_query_attempt_count(0), m_i_query_succeed_count(0),
			m_p_logger(NULL), m_p_forms(NULL), m_s_url("")
		{
			const char* sWho = "MyFtpIndexClientCallback::MyFtpIndexClientCallback";

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "()..." << endl;
			}
		}

		void setPLogger( JDA::Logger* p_logger ){ m_p_logger = p_logger; }
		void setPForms( JDA::Forms* p_forms ){ m_p_forms = p_forms; }
		void setSUrl( const string& s_url ){ m_s_url = s_url; }

		int getIterationCount(){ return m_i_iteration_count; };
		int getByteCount(){ return m_i_byte_count; };
		int getQueryAttemptCount(){ return m_i_query_attempt_count; };
		int getQuerySucceedCount(){ return m_i_query_succeed_count; };

		virtual ~MyFtpIndexClientCallback(){
			const char* sWho = "MyFtpIndexClientCallback::~MyFtpIndexClientCallback";
			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "()..." << endl;
			}
		}

		size_t dataReceived( void* buffer, size_t size, size_t nmemb, void *userdata ){

			const char* sWho = "MyFtpIndexClientCallback::dataReceived";
	
			m_i_iteration_count++;
	
			size_t numBytes = size * nmemb;
	
			m_i_byte_count += numBytes;
	
			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): m_i_iteration_count = " << m_i_iteration_count << "\n" 
					<< "\t" << "size = " << size << "\n" 
					<< "\t" << "nmemb = " << nmemb << "\n" 
					<< "\t" << "numBytes = " << numBytes << "\n" 
					<< "\t" << "m_i_byte_count = " << m_i_byte_count << endl;
			}

			char* cbuf = (char*)buffer;

			// Buffer is not zero-terminated, so explicitly write out the numBytes supplied...
			ostringstream oss_out;
			oss_out.write( cbuf, numBytes );


			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): LINE " << m_i_iteration_count << ": \"" << oss_out.str() << "\"..." << endl;
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): LINE " << m_i_iteration_count << ": GILLIGAN: Splitting on '|', Skipper..." << endl;
			}

			this->mr_fields.clear();
			JDA::Utils::split( oss_out.str(), '|', this->mr_fields, true ); 


			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): LINE " << m_i_iteration_count << ": GILLIGAN: Looks like " << this->mr_fields.size() << " elements in the split, Skipper..." << endl;
			}

			for( size_t i = 0; i < this->mr_fields.size(); i++ ){
				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): LINE " << m_i_iteration_count << ": GILLIGAN: this->mr_fields[" << i << "] = '" << this->mr_fields[i] << "', Skipper..." << endl;
				}
			}

			if( this->mr_fields.size() < 5 ){
				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): " << "mr_fields.size() is less than 5, rejecting this line..." << endl;
				}
				return numBytes;		
			}

			for( size_t i = 0; i < this->mr_fields.size(); i++ ){
				this->mr_fields[i] = JDA::Utils::trim( this->mr_fields[i] );
			}

   			string cik = this->mr_fields[ CIK_IDX ];

			// NOTE: Not using the company name datum right now...
			// ...we don't know if it's a subject company, issuer, or reporting person...
			// wait until we peruse the form header later on, pardner...
			string company_name = this->mr_fields[ COMPANY_NAME_IDX ];

			string form_type = this->mr_fields[ FORM_TYPE_IDX ];
			string date_filed = JDA::FormsMeanUtils::isoDateFromYyyyMmDd( this->mr_fields[ DATE_FILED_IDX ] );
			string file_name = this->mr_fields[ FILE_NAME_IDX ];

			string accession_number = FormsMeanUtils::accessionNumberFromFilePath( file_name );

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): " << "\n" 
   		    	<< " cik = '" << cik << "'" << "\n"
	   	    	<< " company_name = '" << company_name << "'" << "\n" 
	   	    	<< " form_type = '" << form_type << "'" << "\n"
	  	    	<< " date_filed = '" << date_filed << "'" << "\n"
	   	    	<< " file_name = '" << file_name << "'" << "\n"
	   	    	<< " accession_number = '" << accession_number << "'" << "..." << endl;
			}

			m_i_query_attempt_count++;

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): SHEMP: Moe, callin' Forms::insertIndexEntry()..." << endl;
			}

			try {
				m_p_forms->insertIndexEntry( cik, form_type, date_filed, file_name, accession_number, this->m_s_url );

				m_i_query_succeed_count++;

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): SHEMP: Good news, Moe...Forms::insertIndexEntry() appears to be successful...m_i_query_succeed_count = " << m_i_query_succeed_count << "..." << endl;
				}
			}
			catch( JDA::MongoDbClient::Exception& e ){
				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): SHEMP: Sorry, Moe, caught JDA::Utils::Exception during Forms::insertIndexEntry(): \"" << e.what() << "\"..." << endl;
				}
			}
			catch( ... ){
				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): SHEMP: Sorry, Moe, caught unknown exception during Forms::insertIndexEntry()..." << endl;
				}
			}



			//cout << "------------- <ossout-data" << m_i_iteration_count << "> --------------\n"; 
			//cout << oss_out.str();
			//cout << "------------- </ossout-data-" << m_i_iteration_count << "> --------------\n"; 

			// A hack to cause a premature abort after i_max iterations...
			// since the return value from dataReceived() should
			// be the number of bytes or else curl will toss
			// an exception and do an abort...
			int i_max = -1;
			//int i_max = 10;
			//int i_max = 100;
			//int i_max = 1000;

			if( i_max > 0 && m_i_query_attempt_count > (size_t)i_max ){
				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): JOE KOVACS: m_i_query_attempt_count = " << m_i_query_attempt_count << ", is greater than i_max = << " << i_max << ", so returning 0 to attempt to cause an abort, Doc-tor Cy-a-nide..." << endl;
				}
				return 0; // Cause an abort by not returning numBytes...
			}
			else {
				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::DEBUG) << sWho << "(): JOE KOVACS: returning numBytes = " << numBytes << ", Doc-tor Cy-a-nide..." << endl;
				}
				return numBytes; // JOE KOVACS: Important to return numBytes, Doc-tor Cy-a-nide...or else curl will toss an exception and do an abort...
			}

		}/* dataReceived() */

}; /* class MyFtpIndexClientCallback : public JDA::FtpClient::IFtpClientCallback  */

// example:
// Forms::insertIndexEntry(
//	cik = "1000275", form_type = "424B2",
//	date_filed = "20141031", file_name = "edgar/data/1000275/0001214659-14-007292.txt",
//	accession_number = "0001214659-14-007292",	index_url = "file:///../edgar/daily-index/master.20141031.sample.idx"
//  )
int Forms::insertIndexEntry(
	const string& cik, const string& form_type, const string& date_filed, const string& file_name,
	const string& accession_number, const string& index_url
){  

	const char* sWho = "Forms::insertIndexEntry";

	if( m_p_logger ){
		(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): SHEMP: Moe, we gots...\n" 
			<< "\t" << "cik = " << "\"" << cik << "\"," << "\t" << "form_type = " << "\"" << form_type << "\"," << "\n"
			<< "\t" << "date_filed = " << "\"" << date_filed << "\"," << "\t" << "file_name = " << "\"" << file_name << "\"," << "\n"
			<< "\t" << "accession_number = " << "\"" << accession_number << "\"," << "\t" << "index_url = " << "\"" << index_url << "\"..." << endl;
	}

	//e.g.,
	// db.forms.insert( { "cik": 1000180, "form_type": "4", "date_filed": "2014-10-31", 
	//	"file_name": "edgar/data/1000180/0001242648-14-000081.txt",
	//	"accession_number": "0001242648-14-000081",
	//	"index_url": "file:///../edgar/daily-index/master.20141031.sample.idx"
	// });

	ostringstream oss_json;
	oss_json << "{\n" 
	<< "\"cik\": " << cik << ", \"form_type\": \"" << form_type << "\", \"date_filed\": \"" << date_filed << "\",\n" 
	<< "\"file_name\": \"" << file_name << "\",\n"
	<< "\"accession_number\": \"" << accession_number << "\",\n"
	<< "\"index_url\": \"" << index_url << "\"\n"
	<< "}"
	;

	if( m_p_logger ){
		(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): SHEMP: " << "oss_json = \"" << oss_json.str() << "\"..." << endl; 
	}

	string s_collection_name = "forms";

	if( m_p_logger ){
		(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "Calling mongoDbClient.insert( \"" << this->getDbName() << "\", \"" << s_collection_name << "\", \"" << oss_json.str() << "\")..." << endl;
	}

	int i_ret_code = 0;

	// NOTE: This call may toss a JDA::MongoDbClient::Exception
	i_ret_code = mongoDbClient.insert( this->getDbName(), s_collection_name, oss_json.str() );					
			
	return i_ret_code;

}/* insertIndexEntry() */


	/**
	* NOTE: We should be chaining this guy to the JDA::FtpClient::LineratorFtpClientCallback as a "decorator" so that we get
	* the data line-by-line, which is precisely the way JDA::EdgarForm expects it...
	*/
	class MyFtpClientFormeratorCallback : public JDA::FtpClient::IFtpClientCallback 
	{
		public:

		JDA::Logger* m_p_logger;

		size_t m_i_iteration_count; 
		size_t m_i_byte_count; 
		bool m_b_intentional_abort;

		JDA::EdgarForm* m_p_the_formerator;

		void setPLogger( JDA::Logger* p_logger ){ m_p_logger = p_logger; }

		MyFtpClientFormeratorCallback( JDA::EdgarForm* p_the_formerator ): m_p_logger(NULL), m_i_iteration_count(0), m_i_byte_count(0), m_b_intentional_abort(false), m_p_the_formerator(p_the_formerator ) {

			const char* sWho = "MyFtpClientFormeratorCallback::MyFtpClientFormeratorCallback<constructor>";

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "()..." << endl;
			}
			
		}

		virtual ~MyFtpClientFormeratorCallback(){
			const char* sWho = "MyFtpClientFormeratorCallback::~MyFtpClientFormeratorCallback<destructor>";

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "()..." << endl;
			}
			
		}

		size_t dataReceived( void* buffer, size_t size, size_t nmemb, void *userdata ){

			const char* sWho = "MyFtpClientFormeratorCallback::dataReceived";
	
			m_i_iteration_count++;
	
			size_t numBytes = size * nmemb;
	
			m_i_byte_count += numBytes;
	
			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): m_i_iteration_count = " << m_i_iteration_count << "\n" 
				<< "\t" << "size = " << size << "\n" 
				<< "\t" << "nmemb = " << nmemb << "\n" 
				<< "\t" << "numBytes = " << numBytes << "\n" 
				<< "\t" << "m_i_byte_count = " << m_i_byte_count << endl;
			}

			// Note that `buffer` is NOT zero-terminated, so cast it to a char* then
			// use ostringstream.write() with specific number of bytes...
			char* cbuf = (char*)buffer;

			ostringstream oss_line;
			oss_line.write( cbuf, numBytes );

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): LINE " << m_i_iteration_count << ": \"" << oss_line.str() << "\"..." << endl;
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): LINE " << m_i_iteration_count << ": GILLIGAN: Feeding this line to m_p_the_formerator, Skipper..." << endl;
			}

			JDA::EdgarForm::StateType stateTypeReturn = m_p_the_formerator->parseLine( oss_line.str() );

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::TRACE) << sWho << "(): LINE " << m_i_iteration_count << ": GILLIGAN:  stateTypeReturn = " << stateTypeReturn << " = " << JDA::EdgarForm::stateTypeToString(stateTypeReturn) << ", Skipper..." << endl; 
			}

			if( stateTypeReturn == JDA::EdgarForm::STATE_DONE_WITH_SEC_HEADER ){

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): JOE KOVACS: Looks like we're done with the SEC-HEADER, so returning 0 to attempt to cause an abort, Doc-tor Cy-a-nide..." << endl;
				}

				m_b_intentional_abort = true;
				return 0; // Cause an abort by not returning numBytes...
			}
			else {

				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): JOE KOVACS: returning numBytes = " << numBytes << ", Doc-tor Cy-a-nide..." << endl;
				}
				return numBytes; // JOE KOVACS: Important to return numBytes, Doc-tor Cy-a-nide...otherwise curl will abort...
			}

		}/* dataReceived() */

	}; /* class MyFtpClientFormeratorCallback : public JDA::FtpClient::IFtpClientCallback  */


// inspired by: FormsMeanUtils::load_index_into_db()... 
int Forms::loadFromEdgarIndexUrl( const string& sEdgarIndexUrl )
{
	const char* sWho = "Forms::loadFromEdgarIndexUrl";

	if( m_p_logger ){
		(*m_p_logger)(JDA::Logger::INFO) << sWho << "( sEdgarIndexUrl = \"" << sEdgarIndexUrl << "\" ): SHEMP: Here goes, Moe..." << endl;
	}

	// Utilized in both read-from-file case and FtpClient case...
	MyFtpIndexClientCallback myFtpIndexClientCallback = MyFtpIndexClientCallback();

	myFtpIndexClientCallback.setPLogger( m_p_logger );
	myFtpIndexClientCallback.setPForms( this );
	myFtpIndexClientCallback.setSUrl( sEdgarIndexUrl ); // For informational purposes...to log to DB...otherwise callback doesn't need to know...

	// Check to see if the URL is of form "file:///..." if it is, read the file
	// yourself and feed the lines to myFtpClientCallback.  If it's not,
	// use FtpClient to read the lines from the "ftp://..." or "http://..." URL...

	size_t i_where;
	string s_file_url_prefix = "file:///";

	if( m_p_logger ){
		(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): SHEMP: Let's check sEdgarIndexUrl = \"" << sEdgarIndexUrl << "\" to see if it looks like a file job, Moe..." << endl;
	}

	if( ( i_where = JDA::Utils::ifind( sEdgarIndexUrl, s_file_url_prefix ) ) == 0 ){

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): SHEMP: Looks like a file job, Moe..." << endl;
		}

		string file_path = sEdgarIndexUrl.substr( s_file_url_prefix.length() );	

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): SHEMP: file_path = '" << file_path << "', Moe..." << endl;
		}

		std::ifstream le_ifs;

		// Turn on exceptions...live life dangerously for once...
		le_ifs.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

		try {
			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "Opening file_path = '" << file_path << "' for reading..." << endl;
			}

			le_ifs.open( file_path.c_str(), std::ifstream::in );

		}catch( std::ifstream::failure e){

			ostringstream oss_error;

			oss_error << "Trouble opening file_path = '" << file_path << "' for reading: \"" << e.what() << "\", ";
			oss_error << "s_error = \"" << JDA::Utils::s_error() << "\"...";

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): " << oss_error.str() << "\n" 
				<< "SHEMP: I'm tossin' a JDA::FtpClient::FtpException and gettin' outta here, Moe...!" << endl;
			}

			throw JDA::FtpClient::FtpException( oss_error.str() );
		}

		string le_line;
		int i_count = 0;
		while(!le_ifs.eof() && ! le_ifs.fail() ){

			try {
				std::getline( le_ifs, le_line );
			} catch( std::ifstream::failure e){
				ostringstream oss_error;

				oss_error << "Trouble with getline(), file_path = '" << file_path << "': \"" << e.what() << "\"";

				if( m_p_logger ){			
					(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): " << oss_error.str() << "\n"
					<< "SHEMP: Moe...Moe...I think this is just the end o' the file, so I'm gonna eat this exception, OK, Moe...?" << "\n"
					<< "MOE: Ya gotta eat it...it's part of the plot..." << endl;
				}

			}

			i_count++;

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "line #" << i_count << ": \"" << le_line << "\"..." << endl;
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "line #" << i_count << ": SHEMP: Callin' myFtpIndexClientCallback.dataReceived(), Moe..." << endl;
			}

			size_t i_return = myFtpIndexClientCallback.dataReceived( (void*)le_line.c_str(), le_line.size(), sizeof(char), NULL ); 

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "line #" << i_count << ": SHEMP: myFtpIndexClientCallback.dataReceived() retoyned " << i_return << ", Moe..." << endl;
			}

		}/* while(!le_ifs.eof() && ! le_ifs.fail() ) */

		try {
			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "SHEMP: Closin' dhe file_path = '" << file_path << "' for reading, Moe..." << endl;
			}

			le_ifs.close();
		}catch( std::ifstream::failure e){
			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): " << "SHEMP: Trouble closin' dhe file_path = '" << file_path << "' for reading: Caught std::ifstream::failure exception: \"" << e.what() << "\", sorry, Moe, but don't get excited, Moe, I'll just eat the exception..." << endl;
			}
		}

	}/* if( ( i_where = JDA::Utils::ifind( sEdgarFormUrl, s_file_url_prefix ) == 0 ) */
	else {
		if( m_p_logger ){
		 	(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): SHEMP: Looks like an FTP job, Moe..." << endl;
		}

		// Use Linerator as a "decorator" to grab info line-by-line and feed to pMyFtpIndexClientCallback...
		// Requires copy constructor, causes error...
		//JDA::FtpClient::LineratorFtpClientCallback myIFtpClientCallback = JDA::FtpClient::LineratorFtpClientCallback( &myFtpIndexClientCallback );
		JDA::FtpClient::LineratorFtpClientCallback myIFtpClientCallback( &myFtpIndexClientCallback );
	
		string sFilePath = ""; // Use blankey so we don't download to a file...
		ostream* pDownloadStream = NULL; // Use NULL so we don't squirt it to a stream...
	
		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << ": Calling ftpClient.grabIt(" << "\n" 
				<< "\t" << "sEdgarIndexUrl = \"" << sEdgarIndexUrl << "\", sFilePath = \"" << sFilePath << "\", pDownloadStream=" << pDownloadStream << "," << "\n"
				<< "\t" << "iFtpDebug = " << this->getFtpDebug() << ", bFtpNoProxy = " << std::boolalpha << this->getFtpNoProxy() << ", sFtpProxyUserPass = \"" << this->getFtpProxyUserPass() << "\"," << "\n" 
				<< "\t" << "m_p_logger = " << m_p_logger << ", &myIFtpClientCallback = " << &myIFtpClientCallback << "\n"
				<< ")..." << endl;
		}
	
		JDA::FtpClient ftpClient;
	
		size_t iByteCount = 0;
	
		ostringstream oss_info_query;
		oss_info_query << "SELECT * FROM filings";
  
  		/* NOTE: FtpClient::grabIt() may throw JDA::FtpClient::FtpException */
		try {
			iByteCount = ftpClient.grabIt( sEdgarIndexUrl, sFilePath, pDownloadStream,
							this->getFtpDebug(), this->getFtpNoProxy(), this->getFtpProxyUserPass(),
							m_p_logger, &myIFtpClientCallback );
	
			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "():" <<
						"\n" << "***> Received " << JDA::Utils::commify( iByteCount ) << " byte" << (iByteCount == 1 ? "" : "s" ) << ", Sancho..." << endl;
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "***> myFtpIndexClientCallback.getByteCount() = " << myFtpIndexClientCallback.getByteCount() << "..." << endl;
			}

			//byte_count_out = myIFtpClientCallback.m_i_byte_count;
	
			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "***> myFtpIndexClientCallback.getIterationCount() = " << myFtpIndexClientCallback.getIterationCount() << "..." << endl;
			}

			//iteration_count_out = myFtpClientCallback.m_i_iteration_count;

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "***> myFtpIndexClientCallback.getQueryAttemptCount() = " << myFtpIndexClientCallback.getQueryAttemptCount() << "..." << endl;
			}

			//query_attempt_count_out = myFtpClientCallback.m_i_query_attempt_count;
	
			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "***> myFtpIndexClientCallback.getQuerySucceedCount() = " << myFtpIndexClientCallback.getQuerySucceedCount() << "..." << endl;
			}

			//query_succeed_count_out = myFtpClientCallback.m_i_query_succeed_count;
	
		}
		catch( JDA::FtpClient::FtpException& e ){
			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): " << "SHEMP: Sorry, Moe, caught JDA::FtpClient::FtpException during FTP download attempt "
					<< "of \"" << sEdgarIndexUrl << "\": \"" << e.what() << "\".";
			}

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "():" <<
						"\n" << "***> Received " << JDA::Utils::commify( iByteCount ) << " byte" << (iByteCount == 1 ? "" : "s" ) << ", Sancho..." << endl;
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "***> myFtpIndexClientCallback.getByteCount() = " << myFtpIndexClientCallback.getByteCount() << "..." << endl;
			}

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "***> myFtpIndexClientCallback.getIterationCount() = " << myFtpIndexClientCallback.getIterationCount() << "..." << endl;
			}

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "***> myFtpIndexClientCallback.getQueryAttemptCount() = " << myFtpIndexClientCallback.getQueryAttemptCount() << "..." << endl;
			}

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "***> myFtpIndexClientCallback.getQuerySucceedCount() = " << myFtpIndexClientCallback.getQuerySucceedCount() << "..." << endl;
			}

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): " << "SHEMP: Re-throwin' dhe exception, Moe..." << endl;
			}
			
	
			throw e;
		}/* catch */
	
		/* Flush out any debug output from FtpClient... */
		fflush(stdout); 
		fflush(stderr);

	
	}/* else -- an FTP job */

	if( m_p_logger ){
		(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "SHEMP: Moe, I got news for ya, Moe...\n"
		<< "\t" << "myFtpIndexClientCallback.getByteCount = " << myFtpIndexClientCallback.getByteCount() << "..." << "\n"
		<< "\t" << "myFtpIndexClientCallback.getIterationCount = " << myFtpIndexClientCallback.getIterationCount() << "..." << "\n"
		<< "\t" << "myFtpIndexClientCallback.getQueryAttemptCount = " << myFtpIndexClientCallback.getQueryAttemptCount() << "..." << "\n"
		<< "\t" << "myFtpIndexClientCallback.getQuerySucceedCount = " << myFtpIndexClientCallback.getQuerySucceedCount() << "..." << endl;

		(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "Returning myFtpIndexClientCallback.getQuerySucceedCount = " << myFtpIndexClientCallback.getQuerySucceedCount() << ", Moe..." << endl;
	}

	return myFtpIndexClientCallback.getQuerySucceedCount();

}/* void Forms::loadFromEdgarIndexUrl() */

/* Inspired by FormsMeanUtils::load_edgar_form_and_log_it() */
int Forms::loadFromEdgarFormUrl( const string& sEdgarFormUrl ){

	const char* sWho = "Forms::loadFromEdgarFormUrl";

	if( m_p_logger ){
		(*m_p_logger)(JDA::Logger::INFO) << sWho << "( sEdgarFormUrl = \"" << sEdgarFormUrl << "\" ):..." << endl;
	}

	string s_accession_number_from_file_path = FormsMeanUtils::accessionNumberFromFilePath( sEdgarFormUrl );  	
	if( m_p_logger ){
		(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): SHEMP: s_accession_number_from_file_path = '" << s_accession_number_from_file_path << "'..." << endl;
	}

	string s_cik_from_url = FormsMeanUtils::cikFromUrl( sEdgarFormUrl );  	
	if( m_p_logger ){
		(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): SHEMP: s_cik_from_url = '" << s_cik_from_url << "'..." << endl;
	}

	JDA::EdgarForm le_formerator;
	
	size_t i_where;
	string s_file_url_prefix = "file:///";

	// Check to see if it looks like a file:/// URL...
	if( ( i_where = JDA::Utils::ifind( sEdgarFormUrl, s_file_url_prefix ) ) == 0 ){

		if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): SHEMP: Looks like a file, Moe..." << endl;
		}
		string file_path = sEdgarFormUrl.substr( s_file_url_prefix.length() );	

		if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): SHEMP: file_path = '" << file_path << "', Moe..." << endl;
		}
		std::ifstream le_ifs;

		// Turn on exceptions...
		le_ifs.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

		try {
			if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "Opening file_path = '" << file_path << "' for reading..." << endl;
			}			le_ifs.open( file_path.c_str(), std::ifstream::in );
		}catch( std::ifstream::failure e){
			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): " << "Trouble opening file_path = '" << file_path << "' for reading: \"" << e.what() << "\"" << endl;
				(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): " << "s_error = \"" << JDA::Utils::s_error() << "\"..." << endl;
				(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): " << "SHEMP: I'm gettin' outta here, Moe...!" << endl;
			}
			return -1;
		}

		string le_line;
		int i_count = 0;
		while(!le_ifs.eof() && ! le_ifs.fail() ){

			try {
				std::getline( le_ifs, le_line );
			} catch( std::ifstream::failure e){
				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): " << "Trouble with getline(): \"" << e.what() << "\"" << endl;
					(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): " << "SHEMP: I'm gettin' outta here, Moe...!" << endl;
				}
				return -2;
			}

			i_count++;
			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "line #" << i_count << ": \"" << le_line << "\"..." << endl;
			}
			JDA::EdgarForm::StateType stateTypeReturn = le_formerator.parseLine( le_line );

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "line #" << i_count << ": \"" << "stateTypeReturn = " << stateTypeReturn << " = " << JDA::EdgarForm::stateTypeToString(stateTypeReturn) << "..." << endl;
			}
			if( stateTypeReturn == JDA::EdgarForm::STATE_DONE_WITH_SEC_HEADER ){
				if( m_p_logger ){
					(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): "<< "line #" << i_count << ": \"" << "SHEMP: Okey-Dokey, Moe...looks like we're all done with the SEC header, may as well bail out o' the loop now, Moe..." << endl;
				}
				break;
			}

		}/* while(!le_ifs.eof() && ! le_ifs.fail() ) */

		try {
			if( m_p_logger ){
						(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "SHEMP: Closin' dhe file_path = '" << file_path << "' for reading, Moe..." << endl;
			}
			le_ifs.close();
		}catch( std::ifstream::failure e){
			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): " << "SHEMP: Trouble closin' dhe file_path = '" << file_path << "' for reading: \"" << e.what() << "\", sorry, Moe..." << endl;
			}
		}

	}/* if( ( i_where = JDA::Utils::ifind( sEdgarFormUrl, s_file_url_prefix ) == 0 ) */
	else {
		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "SHEMP: Looks like it's gonna be an FTP job, Moe..." << endl;
		}
		JDA::FtpClient ftpClient;

		//MyFtpClientFormeratorCallback myFtpClientFormeratorCallback = MyFtpClientFormeratorCallback( &le_formerator );
		MyFtpClientFormeratorCallback myFtpClientFormeratorCallback( &le_formerator );
		myFtpClientFormeratorCallback.setPLogger( m_p_logger );

		//JDA::FtpClient::LineratorFtpClientCallback myLineratorFtpClientCallback = JDA::FtpClient::LineratorFtpClientCallback( &myFtpClientFormeratorCallback ); 
		JDA::FtpClient::LineratorFtpClientCallback myLineratorFtpClientCallback( &myFtpClientFormeratorCallback ); 

		JDA::Logger* pLogger = NULL;
		string sFilePath = ""; // Use blankey so we don't download to a file...
		ostream* pDownloadStream = NULL; // Use null so it doesn't get squirted to a file stream...

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << ": Calling ftpClient.grabIt(" << "\n" 
			<< "\t" << "sEdgarFormUrl = \"" << sEdgarFormUrl << "\", sFilePath = \"" << sFilePath << "\", pDownloadStream=" << pDownloadStream << "," << "\n"
			<< "\t" << "this->getFtpDebug() = " << this->getFtpDebug() << ", this->getFtpNoProxy() = " << std::boolalpha << this->getFtpNoProxy() << ", this->getFtpProxyUserPass() = \"" << this->getFtpProxyUserPass() << "\"," << "\n" 
			<< "\t" << "pLogger = " << pLogger << ", &myLineratorFtpClientCallback = " << &(myLineratorFtpClientCallback) << "\n"
			<< ")..." << endl;
		}
		size_t iByteCount = 0;
		bool b_ftp_exception_caught = false;
		string s_ftp_exception = "";

		/* NOTE: FtpClient::grabIt() may throw JDA::FtpClient::FtpException */
		try {
			iByteCount = ftpClient.grabIt( sEdgarFormUrl, sFilePath, pDownloadStream,
							this->getFtpDebug(), this->getFtpNoProxy(), this->getFtpProxyUserPass(),
							pLogger, &(myLineratorFtpClientCallback)
			);

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::INFO) << "\n" << "***> DON QUIJOTE: Received " << JDA::Utils::commify( iByteCount ) << " byte" << (iByteCount == 1 ? "" : "s" ) << ", Sancho..." << endl;
			}
		}
		catch( JDA::FtpClient::FtpException& e ){
			ostringstream oss_out;
			oss_out << "Caught JDA::FtpClient::FtpException during FTP download attempt "
				<< "of \"" << sEdgarFormUrl << "\": \"" << e.what() << "\"." << endl;

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): " << oss_out.str() << endl;
			}

			b_ftp_exception_caught = true;
			s_ftp_exception = oss_out.str();
		}

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "***> myFtpClientFormeratorCallback.m_i_byte_count = " << myFtpClientFormeratorCallback.m_i_byte_count << "..." << endl;

			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "***> myFtpClientFormeratorCallback.m_i_iteration_count = " << myFtpClientFormeratorCallback.m_i_iteration_count << "..." << endl;

			(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "***> myFtpClientFormeratorCallback.m_b_intentional_abort = " << myFtpClientFormeratorCallback.m_b_intentional_abort << "..." << endl;
		}

		if( b_ftp_exception_caught && myFtpClientFormeratorCallback.m_b_intentional_abort == false ){

			if( m_p_logger ){
				(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): SHEMP: Sorry, Moe, caught an FTP Exception ['" << s_ftp_exception << "'] and myFtpClientFormeratorCallback.m_b_intentional_abort == false, so loggin' error to dhe database and bailin' out, Moe..." << endl;
			}

			//s_ftp_exception = FormsMeanUtils::single_quote_escape( s_ftp_exception );

			//FormsMeanUtils::db_filings_edgar_header_load_log(
			//	db_url, db_user, db_pass, 
			//	s_accession_number_from_file_path, false, &s_ftp_exception, "FormsMeanDaemon"
			//);

			return -1;
		}
	}

	if( m_p_logger ){
		(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): SHEMP: Moe, le_formerator.m_s_accession_number = '" << le_formerator.m_s_accession_number << "'...\n" << endl;
		(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): SHEMP: Moe, le_formerator.toString = '" << le_formerator.toString() << "'..." << endl;
	}

	ostringstream oss_json_query;
	ostringstream oss_json_update;

	oss_json_query << "{ \"accession_number\": \"" << le_formerator.m_s_accession_number << "\" }";

	time_t seconds_since_unix_epoch = time(NULL); 
	int64_t milliseconds_since_unix_epoch = JDA::MongoDbClient::milliseconds_since_unix_epoch();

	if( m_p_logger ){
		(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): SHEMP: Moe, seconds_since_unix_epoch = " << seconds_since_unix_epoch << " = " 
			<< "\"" << JDA::Utils::get_utc_timestamp( seconds_since_unix_epoch ) << "\" = \""
			<< JDA::Utils::get_local_timestamp( seconds_since_unix_epoch ) << "\"..." << endl;
		(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): SHEMP: Moe, milliseconds_since_unix_epoch = " << milliseconds_since_unix_epoch << "..." << endl;
	}


	oss_json_update 
	<< "{\n"
	<< " \"$push\": { \"form_processing_attempts\" :  { \"when\": { \"$date\": " << milliseconds_since_unix_epoch << " }, \"success\": true } },\n";

  	oss_json_update
	<< " \"$set\": {\n" 
	;

	oss_json_update
	<< "	\"filers\": [\n" 
	;

	for( size_t i = 0; i < le_formerator.m_filers.size(); i++ ){ 
		oss_json_update
		<< "		{\n" 
		<< "			\"company_data\":{\n"
		<< "				\"company_conformed_name\": \"" << FormsMeanUtils::double_quote_escape( le_formerator.m_filers[i]->company_data.company_conformed_name ) << "\",\n"
		<< "				\"central_index_key\": \"" << le_formerator.m_filers[i]->company_data.central_index_key << "\",\n"
		<< "				\"standard_industrial_classification\": \"" << le_formerator.m_filers[i]->company_data.standard_industrial_classification << "\",\n"
		<< "				\"irs_number\": \"" << le_formerator.m_filers[i]->company_data.irs_number << "\",\n"
		<< "				\"state_of_incorporation\": \"" << le_formerator.m_filers[i]->company_data.state_of_incorporation << "\",\n"
		<< "				\"fiscal_year_end\": \"" << le_formerator.m_filers[i]->company_data.fiscal_year_end << "\"\n"
		<< "			},\n"
		<< "			\"filing_values\":{\n"
		<< "				\"form_type\": \"" << FormsMeanUtils::double_quote_escape( le_formerator.m_filers[i]->filing_values.form_type ) << "\",\n"
		<< "				\"sec_act\": \"" << le_formerator.m_filers[i]->filing_values.sec_act << "\",\n"
		<< "				\"sec_file_number\": \"" << le_formerator.m_filers[i]->filing_values.sec_file_number << "\",\n"
		<< "				\"film_number\": \"" << le_formerator.m_filers[i]->filing_values.film_number << "\"\n"
		<< "			},\n"
		<< "			\"business_address\":{\n"
		<< "				\"street_1\": \"" << le_formerator.m_filers[i]->business_address.street_1 << "\",\n"
		<< "				\"street_2\": \"" << le_formerator.m_filers[i]->business_address.street_2 << "\",\n"
		<< "				\"city\": \"" << le_formerator.m_filers[i]->business_address.city << "\",\n"
		<< "				\"state\": \"" << le_formerator.m_filers[i]->business_address.state << "\",\n"
		<< "				\"zip\": \"" << le_formerator.m_filers[i]->business_address.zip << "\",\n"
		<< "				\"business_phone\": \"" << le_formerator.m_filers[i]->business_address.business_phone << "\"\n"
		<< "			},\n"
		<< "			\"mail_address\":{\n"
		<< "				\"street_1\": \"" << le_formerator.m_filers[i]->mail_address.street_1 << "\",\n"
		<< "				\"street_2\": \"" << le_formerator.m_filers[i]->mail_address.street_2 << "\",\n"
		<< "				\"city\": \"" << le_formerator.m_filers[i]->mail_address.city << "\",\n"
		<< "				\"state\": \"" << le_formerator.m_filers[i]->mail_address.state << "\",\n"
		<< "				\"zip\": \"" << le_formerator.m_filers[i]->mail_address.zip << "\"\n"
		<< "			}\n"
		<< "		}";

		if( i < le_formerator.m_filers.size()-1 ){
			oss_json_update << ",";
		}
		oss_json_update << "\n";
	}

	oss_json_update
	<< "	],\n" 
	;

	oss_json_update
	<< "	\"issuer\": {\n" 
	<< "		\"company_data\":{\n"
	<< "			\"company_conformed_name\": \"" << FormsMeanUtils::double_quote_escape( le_formerator.m_issuer.company_data.company_conformed_name ) << "\",\n"
	<< "			\"central_index_key\": \"" << le_formerator.m_issuer.company_data.central_index_key << "\",\n"
	<< "			\"standard_industrial_classification\": \"" << le_formerator.m_issuer.company_data.standard_industrial_classification << "\",\n"
	<< "			\"irs_number\": \"" << le_formerator.m_issuer.company_data.irs_number << "\",\n"
	<< "			\"state_of_incorporation\": \"" << le_formerator.m_issuer.company_data.state_of_incorporation << "\",\n"
	<< "			\"fiscal_year_end\": \"" << le_formerator.m_issuer.company_data.fiscal_year_end << "\"\n"
	<< "		},\n"
	<< "		\"business_address\":{\n"
	<< "			\"street_1\": \"" << le_formerator.m_issuer.business_address.street_1 << "\",\n"
	<< "			\"street_2\": \"" << le_formerator.m_issuer.business_address.street_2 << "\",\n"
	<< "			\"city\": \"" << le_formerator.m_issuer.business_address.city << "\",\n"
	<< "			\"state\": \"" << le_formerator.m_issuer.business_address.state << "\",\n"
	<< "			\"zip\": \"" << le_formerator.m_issuer.business_address.zip << "\",\n"
	<< "			\"business_phone\": \"" << le_formerator.m_issuer.business_address.business_phone << "\"\n"
	<< "		},\n"
	<< "		\"mail_address\":{\n"
	<< "			\"street_1\": \"" << le_formerator.m_issuer.mail_address.street_1 << "\",\n"
	<< "			\"street_2\": \"" << le_formerator.m_issuer.mail_address.street_2 << "\",\n"
	<< "			\"city\": \"" << le_formerator.m_issuer.mail_address.city << "\",\n"
	<< "			\"state\": \"" << le_formerator.m_issuer.mail_address.state << "\",\n"
	<< "			\"zip\": \"" << le_formerator.m_issuer.mail_address.zip << "\"\n"
	<< "		}\n"
	<< "	},\n";

	oss_json_update
	<< "	\"reporting_owner\": {\n" 
	<< "		\"owner_data\":{\n"
	<< "			\"company_conformed_name\": \"" << FormsMeanUtils::double_quote_escape( le_formerator.m_reporting_owner.owner_data.company_conformed_name ) << "\",\n"
	<< "			\"central_index_key\": \"" << le_formerator.m_reporting_owner.owner_data.central_index_key << "\"\n"
	<< "		},\n"
	<< "		\"filing_values\":{\n"
	<< "			\"form_type\": \"" << FormsMeanUtils::double_quote_escape( le_formerator.m_reporting_owner.filing_values.form_type ) << "\",\n"
	<< "			\"sec_act\": \"" << le_formerator.m_reporting_owner.filing_values.sec_act << "\",\n"
	<< "			\"sec_file_number\": \"" << le_formerator.m_reporting_owner.filing_values.sec_file_number << "\",\n"
	<< "			\"film_number\": \"" << le_formerator.m_reporting_owner.filing_values.film_number << "\"\n"
	<< "		},\n"
	<< "		\"business_address\":{\n"
	<< "			\"street_1\": \"" << le_formerator.m_reporting_owner.business_address.street_1 << "\",\n"
	<< "			\"street_2\": \"" << le_formerator.m_reporting_owner.business_address.street_2 << "\",\n"
	<< "			\"city\": \"" << le_formerator.m_reporting_owner.business_address.city << "\",\n"
	<< "			\"state\": \"" << le_formerator.m_reporting_owner.business_address.state << "\",\n"
	<< "			\"zip\": \"" << le_formerator.m_reporting_owner.business_address.zip << "\",\n"
	<< "			\"business_phone\": \"" << le_formerator.m_reporting_owner.business_address.business_phone << "\"\n"
	<< "		},\n"
	<< "		\"mail_address\":{\n"
	<< "			\"street_1\": \"" << le_formerator.m_reporting_owner.mail_address.street_1 << "\",\n"
	<< "			\"street_2\": \"" << le_formerator.m_reporting_owner.mail_address.street_2 << "\",\n"
	<< "			\"city\": \"" << le_formerator.m_reporting_owner.mail_address.city << "\",\n"
	<< "			\"state\": \"" << le_formerator.m_reporting_owner.mail_address.state << "\",\n"
	<< "			\"zip\": \"" << le_formerator.m_reporting_owner.mail_address.zip << "\"\n"
	<< "		}\n"
	<< "	}\n";

	oss_json_update 
	<< " }\n"
	<< "}"
	;

	string s_collection_name = "forms";

	if( m_p_logger ){
		(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): " << "Calling mongoDbClient.update( \"" << this->getDbName() << "\", \"" << s_collection_name << "\", \"" << oss_json_query.str() << "\", \"" << oss_json_update.str() << "\" )..." << endl;
	}

	int i_ret_code = 0;

	// NOTE: This call may toss a JDA::MongoDbClient::Exception
	try {
		i_ret_code = mongoDbClient.update( this->getDbName(), s_collection_name, oss_json_query.str(), oss_json_update.str() );					

		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): " << "SHEMP: i_ret_code = " << i_ret_code << endl;
		}
	}catch( JDA::MongoDbClient::Exception e ){
		if( m_p_logger ){
			(*m_p_logger)(JDA::Logger::ERROR) << sWho << "(): " << "SHEMP: Trouble with mongoDbClient.insert: \"" << e.what() << "\", sorry, Moe..." << endl;
		}
	}

	//if( m_p_logger ){
	//	(*m_p_logger)(JDA::Logger::INFO) << sWho << "(): SHEMP: Moe, loggin' success to dhe database and returnin', Moe..." << endl;
	//}
	//FormsMeanUtils::db_filings_edgar_header_load_log(
	//		db_url, db_user, db_pass, 
	//		s_accession_number_from_file_path, true, NULL, "FormsMeanDaemon"
	//);

	return 1;

}/* Forms::loadFromEdgarFormUrl() */


} /* namespace JDA */
