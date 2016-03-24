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

    	const static int CIK_IDX = 0;	
		const static int COMPANY_NAME_IDX = 1;	
		const static int FORM_TYPE_IDX = 2;	
		const static int DATE_FILED_IDX = 3;	
		const static int FILE_NAME_IDX = 4;	

	public:

		MyFtpIndexClientCallback(): m_i_iteration_count(0), m_i_byte_count(0), m_i_query_attempt_count(0), m_i_query_succeed_count(0), m_p_logger(NULL) {
			const char* sWho = "MyFtpIndexClientCallback::MyFtpIndexClientCallback";
			(*m_p_logger)(VF::Logger::TRACE) << sWho << "()..." << endl;
		}

		void setPLogger( VF::Logger* p_logger ){ m_p_logger = p_logger; }

		virtual ~MyFtpIndexClientCallback(){
			const char* sWho = "MyFtpIndexClientCallback::~MyFtpIndexClientCallback";
			(*m_p_logger)(VF::Logger::TRACE) << sWho << "()..." << endl;
		}

	private:

		size_t dataReceived( void* buffer, size_t size, size_t nmemb, void *userdata ){

			const char* sWho = "MyFtpIndexClientCallback::dataReceived";
	
			m_i_iteration_count++;
	
			size_t numBytes = size * nmemb;
	
			m_i_byte_count += numBytes;
	
			(*m_p_logger)(VF::Logger::TRACE) << sWho << "(): m_i_iteration_count = " << m_i_iteration_count << "\n" 
				<< "\t" << "size = " << size << "\n" 
				<< "\t" << "nmemb = " << nmemb << "\n" 
				<< "\t" << "numBytes = " << numBytes << "\n" 
				<< "\t" << "m_i_byte_count = " << m_i_byte_count << endl;

			char* cbuf = (char*)buffer;

			// Buffer is not zero-terminated, so explicitly write out the numBytes supplied...
			ostringstream oss_out;
			oss_out.write( cbuf, numBytes );


			(*m_p_logger)(VF::Logger::INFO) << sWho << "(): LINE " << m_i_iteration_count << ": \"" << oss_out.str() << "\"..." << endl;
			(*m_p_logger)(VF::Logger::INFO) << sWho << "(): LINE " << m_i_iteration_count << ": GILLIGAN: Splitting on '|', Skipper..." << endl;

			this->mr_fields.clear();
			VF::Utils::split( oss_out.str(), '|', this->mr_fields, true ); 

			(*m_p_logger)(VF::Logger::INFO) << sWho << "(): LINE " << m_i_iteration_count << ": GILLIGAN: Looks like " << this->mr_fields.size() << " elements in the split, Skipper..." << endl;

			for( size_t i = 0; i < this->mr_fields.size(); i++ ){
				(*m_p_logger)(VF::Logger::INFO) << sWho << "(): LINE " << m_i_iteration_count << ": GILLIGAN: this->mr_fields[" << i << "] = '" << this->mr_fields[i] << "', Skipper..." << endl;
			}

			if( this->mr_fields.size() < 5 ){
				(*m_p_logger)(VF::Logger::DEBUG) << sWho << "(): " << "mr_fields.size() is less than 5, rejecting this line..." << endl;
				return numBytes;		
			}

			for( size_t i = 0; i < this->mr_fields.size(); i++ ){
				this->mr_fields[i] = VF::Utils::trim( this->mr_fields[i] );
			}

   			string cik = this->mr_fields[ CIK_IDX ];

			// NOTE: Not using the company name datum right now...we don't know if it's a subject company, issuer, or reporting person...
			// wait until we peruse the form header later on, pardner...
			string company_name = this->mr_fields[ COMPANY_NAME_IDX ];

			string form_type = this->mr_fields[ FORM_TYPE_IDX ];
			string date_filed = this->mr_fields[ DATE_FILED_IDX ];
			string file_name = this->mr_fields[ FILE_NAME_IDX ];

			string accession_number = WaldoUtils::accessionNumberFromFilePath( file_name );

			(*m_p_logger)(VF::Logger::DEBUG) << sWho << "(): " << "\n" 
   	    	<< " cik = '" << cik << "'" << "\n"
   	    	<< " company_name = '" << company_name << "'" << "\n" 
   	    	<< " form_type = '" << form_type << "'" << "\n"
  	     	<< " date_filed = '" << date_filed << "'" << "\n"
   	    	<< " file_name = '" << file_name << "'" << "\n"
   	    	<< " accession_number = '" << accession_number << "'" << "\n"
   	    	<< " cik_of_filing_agent = '" << cik_of_filing_agent << "'" << endl;

   	    	company_name = WaldoUtils::single_quote_escape( company_name );
			(*m_p_logger)(VF::Logger::DEBUG) << sWho << "(): " << "After escaping, company_name = '" << company_name << "'" << endl;

			ostringstream oss_query;  
			oss_query.str("");
			oss_query << 
				"EXEC daemon_edgar_index_entry_do" << "\n" 
				<< " @cik_bigint = '" << cik << "'," << "\n"
				//<< " @company_name_varchar = '" << company_name << "'," << "\n"
				<< " @form_type_varchar = '" << form_type << "'," << "\n"
				<< " @date_filed_varchar = '" << date_filed << "'," << "\n"
				<< " @file_name_varchar = '" << file_name << "'," << "\n"
				<< " @accession_number_varchar = '" << accession_number << "'," << "\n"
				<< " @cik_of_filing_agent_bigint = '" << cik_of_filing_agent << "'," << "\n"
				<< " @source_modified_varchar = '" << this->source_modified << "'" 
			;

			(*m_p_logger)(VF::Logger::DEBUG) << sWho << "(): SHEMP: Moe, executing oss_query = " << oss_query.str() << ", Moe..." << endl;

			m_i_query_attempt_count++;

			VF::AdoDbClient adoDbClient;

			try {
				(*m_p_logger)(VF::Logger::DEBUG) << sWho << "(): Connecting to '" << db_url << "'..." << endl;
				adoDbClient.connect( db_url, db_user, db_pass );
			} catch(VF::DbClient::DbException& e){ 
				ostringstream oss_out;
				oss_out << "Caught VF::DbClient::DBException while trying to connect to db_url ='" << db_url << "' : \"" << e.what() << "\"";
				(*m_p_logger)(VF::Logger::ERROR) << sWho << "(): " << oss_out.str() << endl;

				return numBytes;
			}

			bool b_query_as_vector = true;

			if( b_query_as_vector ){

				std::vector< map<string, string> > vecInOut;
			
				try {
					// May toss VF::DbClient::DbException...
					adoDbClient.query_as_vector( oss_query.str(), vecInOut ); 	

					//m_i_query_succeed_count++;
				}catch( VF::DbClient::DbException& e ){
				
					(*m_p_logger)(VF::Logger::ERROR) << sWho << "(): Caught VF::DbClient::DbException: " << e.what() << "..." << endl;
	
					return numBytes;
				}

				(*m_p_logger)(VF::Logger::INFO) << sWho << "(): Looks like " << vecInOut.size() << " element(s) in the result set vector, Captain..." << endl;
		
				string s_key;
				string s_value;
				for( size_t i = 0; i < vecInOut.size(); i++ ){
				
					(*m_p_logger)(VF::Logger::INFO) << sWho << "(): vecInOut[" << i << "]:" << "\n";
					
					for(std::map<string,string>::iterator it = vecInOut.at(i).begin(); it != vecInOut.at(i).end(); it++) {
					    s_key = it->first;
					    s_value = it->second;
						(*m_p_logger)(VF::Logger::INFO) << "\t" << s_key << "=" << "'" << s_value << "'" << "\n"; 
					}
					(*m_p_logger)(VF::Logger::INFO) << endl;

				}

				// Stored Proc Should Return a single-row result set like so:
				// [2015-12-16 17:04:20 EST] INFO: WaldoUtils::db_print_query(): vecInOut[0]:
				//	message='daemon_edgar_index_entry_do.sp: @filings_id = 10966'
				//	return_code='1'
				//	return_string='SUCCESS'

				if( vecInOut.size() > 0 ){

					string s_return_string;

					if( vecInOut.at(0).find("return_string") != vecInOut.at(0).end() ){
						s_return_string = vecInOut.at(0).at("return_string");
					}
					else{	
						(*m_p_logger)(VF::Logger::WARN) << sWho << "(): SHEMP: Moe, dhere's no 'return_string' key in dhis vector, Moe..." << endl; 
					}

					(*m_p_logger)(VF::Logger::INFO) << sWho << "(): s_return_string = '" << s_return_string << "'" << endl;

					if( s_return_string.compare("SUCCESS") == 0 ){
						(*m_p_logger)(VF::Logger::INFO) << sWho << "(): SHEMP: Moe, looks successful, so incrementing dhe m_i_query_success_count, Moe..." << endl;
						m_i_query_succeed_count++;
					}
					else{
						(*m_p_logger)(VF::Logger::WARN) << sWho << "(): SHEMP: Sorry, Moe, I didn't find vecInOut.at(0).at('return_string'} equal to \"SUCCESS\", Moe...I'm not gonna increment the m_i_query_succeed_count, Moe..." << endl;
					}
				}

			}/* if(b_query_as_vector) */
			else {
				try {
					int iRowsAffected = adoDbClient.execute( oss_query.str() );
					(*m_p_logger)(VF::Logger::DEBUG) << sWho << "(): " << iRowsAffected << " ROW(S) affected, Moe..." << endl;
					m_i_query_succeed_count++;
				}
				catch( VF::DbClient::DbException& e ){
				
					(*m_p_logger)(VF::Logger::ERROR) << sWho << "(): Caught VF::DbClient::DbException: " << e.what() << "..." << endl;
					return numBytes;
				}
			}


			(*m_p_logger)(VF::Logger::INFO) << sWho << "(): SHEMP: Moe, callin' db_denormalize_filing_agent() wit' " 
			<< " *p_s_accession number_varchar = s_accession_number_from_file_path = \"" << accession_number << "\"..., OK, Moe...?" << endl;

			bool b_return = VF::WaldoUtils::db_denormalize_filing_agent( &(*m_p_logger), "", "", "", &adoDbClient, &accession_number, NULL ); 

			(*m_p_logger)(VF::Logger::INFO) << sWho << "(): SHEMP: Moe, b_return from db_denormalize_filing_agent() = " << VF::Utils::boolToString( b_return ) << ", Moe.." << endl;


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
				(*m_p_logger)(VF::Logger::INFO) << sWho << "(): JOE KOVACS: m_i_query_attempt_count = " << m_i_query_attempt_count << ", is greater than i_max = << " << i_max << ", so returning 0 to attempt to cause an abort, Doc-tor Cy-a-nide..." << endl;
				return 0; // Cause an abort by not returning numBytes...
			}
			else {
				(*m_p_logger)(VF::Logger::DEBUG) << sWho << "(): JOE KOVACS: returning numBytes = " << numBytes << ", Doc-tor Cy-a-nide..." << endl;
				return numBytes; // JOE KOVACS: Important to return numBytes, Doc-tor Cy-a-nide...or else curl will toss an exception and do an abort...
			}

		}/* dataReceived() */

}; /* class MyFtpIndexClientCallback : public JDA::FtpClient::IFtpClientCallback  */


// inspired by: WaldoUtils::load_index_into_db()... 
int Forms::loadFromEdgarIndexUrl( const string& sEdgarIndexUrl )
{
	const char* sWho = "Forms::loadFromEdgarIndexUrl";

	MyFtpIndexClientCallback myFtpIndexClientCallback = MyFtpIndexClientCallback();
	//myFtpIndexClientCallback.setDbConnection( m_p_db_connection );
	myFtpIndexClientCallback.setPLogger( m_p_logger );

	// Check to see if the URL is of form "file:///..." if it is, read the file
	// yourself and feed the lines to myFtpClientCallback.  If it's not,
	// use FtpClient to read the lines from the "ftp://..." or "http://..." URL...

	size_t i_where;
	string s_file_url_prefix = "file:///";

	if( ( i_where = VF::Utils::ifind( ftp_url, s_file_url_prefix ) ) == 0 ){

		(*m_p_logger)(VF::Logger::INFO) << sWho << "(): SHEMP: Looks like a file job, Moe..." << endl;

		string file_path = ftp_url.substr( s_file_url_prefix.length() );	

		(*m_p_logger)(VF::Logger::INFO) << sWho << "(): SHEMP: file_path = '" << file_path << "', Moe..." << endl;

		std::ifstream le_ifs;

		// Turn on exceptions...live life dangerously for once...
		le_ifs.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

		try {

			(*m_p_logger)(VF::Logger::INFO) << sWho << "(): " << "Opening file_path = '" << file_path << "' for reading..." << endl;
			le_ifs.open( file_path.c_str(), std::ifstream::in );

		}catch( std::ifstream::failure e){

			ostringstream oss_error;

			oss_error << "Trouble opening file_path = '" << file_path << "' for reading: \"" << e.what() << "\", ";
			oss_error << "s_error = \"" << VF::Utils::s_error() << "\"...";

			(*m_p_logger)(VF::Logger::ERROR) << sWho << "(): " << oss_error.str() << "\n" 
			<< "SHEMP: I'm tossin' a VF::FtpClient::FtpException and gettin' outta here, Moe...!" << endl;

			throw VF::FtpClient::FtpException( oss_error.str() );
		}

		string le_line;
		int i_count = 0;
		while(!le_ifs.eof() && ! le_ifs.fail() ){

			try {
				std::getline( le_ifs, le_line );
			} catch( std::ifstream::failure e){
				ostringstream oss_error;

				oss_error << "Trouble with getline(), file_path = '" << file_path << "': \"" << e.what() << "\"";

				(*m_p_logger)(VF::Logger::ERROR) << sWho << "(): " << oss_error.str() << "\n"
				<< "SHEMP: Moe...Moe...I think this is just the end o' the file, so I'm gonna eat this exception, OK, Moe...?" << "\n";
				<< "MOE: Ya gotta eat it...it's part of the plot..." << endl;
			}

			i_count++;
			(*m_p_logger)(VF::Logger::INFO) << sWho << "(): " << "line #" << i_count << ": \"" << le_line << "\"..." << endl;
			(*m_p_logger)(VF::Logger::INFO) << sWho << "(): " << "line #" << i_count << ": SHEMP: Callin' myFtpClientCallback.dataReceived(), Moe..." << endl;

			size_t i_return = myFtpClientCallback.dataReceived( (void*)le_line.c_str(), le_line.size(), sizeof(char), NULL ); 

			(*m_p_logger)(VF::Logger::INFO) << sWho << "(): " << "line #" << i_count << ": SHEMP: myFtpClientCallback.dataReceived() retoyned " << i_return << ", Moe..." << endl;

		}/* while(!le_ifs.eof() && ! le_ifs.fail() ) */

		try {
			(*m_p_logger)(VF::Logger::INFO) << sWho << "(): " << "SHEMP: Closin' dhe file_path = '" << file_path << "' for reading, Moe..." << endl;
			le_ifs.close();
		}catch( std::ifstream::failure e){
			(*m_p_logger)(VF::Logger::ERROR) << sWho << "(): " << "SHEMP: Trouble closin' dhe file_path = '" << file_path << "' for reading: Caught std::ifstream::failure exception: \"" << e.what() << "\", sorry, Moe, but don't get excited, Moe, I'll just eat the exception..." << endl;
		}

	}/* if( ( i_where = VF::Utils::ifind( sEdgarFormUrl, s_file_url_prefix ) == 0 ) */
	else {
		(*m_p_logger)(VF::Logger::INFO) << sWho << "(): SHEMP: Looks like an FTP job, Moe..." << endl;

//		// Use Linerator as a "decorator" to grab info line-by-line and feed to pMyFtpIndexClientCallback...
//		VF::FtpClient::LineratorFtpClientCallback myIFtpClientCallback = VF::FtpClient::LineratorFtpClientCallback( &myFtpClientCallback );
//	
//		VF::Logger* pLogger = NULL;
//		string sFilePath = ""; // Use blankey so we don't download to a file...
//		ostream* pDownloadStream = NULL;
//	
//		(*m_p_logger)(VF::Logger::INFO) << sWho << ": Calling ftpClient.grabIt(" << "\n" 
//			<< "\t" << "ftp_url = \"" << ftp_url << "\", sFilePath = \"" << sFilePath << "\", pDownloadStream=" << pDownloadStream << "," << "\n"
//			<< "\t" << "iFtpDebug = " << iFtpDebug << ", bFtpNoProxy = " << VF::Utils::boolToString( bFtpNoProxy) << ", sFtpProxyUserPass = \"" << sFtpProxyUserPass << "\"," << "\n" 
//			<< "\t" << "pLogger = " << pLogger << ", &myIFtpClientCallback = " << &myIFtpClientCallback << "\n"
//			<< ")..." << endl;
//	
//		VF::FtpClient ftpClient;
//	
//		size_t iByteCount = 0;
//	
//		ostringstream oss_info_query;
//		oss_info_query << "SELECT * FROM filings";
//	
//		/* NOTE: FtpClient::grabIt() may throw VF::FtpClient::FtpException */
//		try {
//			iByteCount = ftpClient.grabIt( ftp_url, sFilePath, pDownloadStream, iFtpDebug, bFtpNoProxy, sFtpProxyUserPass, pLogger, &myIFtpClientCallback );
//	
//			cout << "\n" << "***> Received " << VF::Utils::commify( iByteCount ) << " byte" << (iByteCount == 1 ? "" : "s" ) << ", Sancho..." << endl;
//	
//			(*m_p_logger)(VF::Logger::INFO) << sWho << "(): " << "***> myFtpClientCallback.m_i_byte_count = " << myFtpClientCallback.m_i_byte_count << "..." << endl;
//			byte_count_out = myFtpClientCallback.m_i_byte_count;
//	
//			(*m_p_logger)(VF::Logger::INFO) << sWho << "(): " << "***> myFtpClientCallback.m_i_iteration_count = " << myFtpClientCallback.m_i_iteration_count << "..." << endl;
//			iteration_count_out = myFtpClientCallback.m_i_iteration_count;
//
//			(*m_p_logger)(VF::Logger::INFO) << sWho << "(): " << "***> myFtpClientCallback.m_i_query_attempt_count = " << myFtpClientCallback.m_i_query_attempt_count << "..." << endl;
//			query_attempt_count_out = myFtpClientCallback.m_i_query_attempt_count;
//	
//			(*m_p_logger)(VF::Logger::INFO) << sWho << "(): " << "***> myFtpClientCallback.m_i_query_succeed_count = " << myFtpClientCallback.m_i_query_succeed_count << "..." << endl;
//			query_succeed_count_out = myFtpClientCallback.m_i_query_succeed_count;
//	
//			//db_print_query(db_url, db_user, db_pass, oss_info_query.str() );
//	
//		}
//		catch( VF::FtpClient::FtpException& e ){
//			(*m_p_logger)(VF::Logger::ERROR) << sWho << "(): " << "Caught VF::FtpClient::FtpException during FTP download attempt "
//				<< "of \"" << ftp_url << "\": \"" << e.what() << "\".";
//	
//			(*m_p_logger)(VF::Logger::INFO) << sWho << "(): " << "***> myFtpClientCallback.m_i_byte_count = " << myFtpClientCallback.m_i_byte_count << "..." << endl;
//	
//			(*m_p_logger)(VF::Logger::INFO) << sWho << "(): " << "***> myFtpClientCallback.m_i_iteration_count = " << myFtpClientCallback.m_i_iteration_count << "..." << endl;
//			(*m_p_logger)(VF::Logger::INFO) << sWho << "(): " << "***> myFtpClientCallback.m_i_query_attempt_count = " << myFtpClientCallback.m_i_query_attempt_count << "..." << endl;
//	
//			(*m_p_logger)(VF::Logger::INFO) << sWho << "(): " << "***> myFtpClientCallback.m_i_query_succeed_count = " << myFtpClientCallback.m_i_query_succeed_count << "..." << endl;
//	
//			//db_print_query(db_url, db_user, db_pass, oss_info_query.str() );
//	
//			(*m_p_logger)(VF::Logger::ERROR) << sWho << "(): " << "Re-throwin' dhe exception, Moe..." << endl;
//	
//			throw e;
//		}
//	
//		/* Flush out any debug output from FtpClient... */
//		fflush(stdout); 
//		fflush(stderr);
	
	}/* else -- an FTP job */

	(*m_p_logger)(VF::Logger::INFO) << sWho << "(): " << "Returning myFtpClientCallback.m_i_query_succeed_count = " << myFtpClientCallback.m_i_query_succeed_count << ", Moe..." << endl;

	return myFtpClientCallback.m_i_query_succeed_count;

}/* void Forms::loadFromEdgarIndexUrl() */


} /* namespace JDA */
