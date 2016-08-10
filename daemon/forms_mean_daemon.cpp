#include <string>
#include <iostream>
#include <ctime>
#include <time.h>

#ifdef _WIN32
	#include "winsock2.h" // Better to pro-actively include it now before it gets included later causing re-definition errors... 
	#include "windows.h" // Sleep(), etc. (NOTE: In Windows it's Sleep(), in UNIX it's sleep()...) 
#endif

#include "utils.h" 
#include "logger.h" 
#include "FtpClient.h" 

#include "FormsMeanCommon.h"
#include "FormsMeanUtils.h"
#include "Forms.h"

using namespace std;

/* Feed a pointer to OurParams to ServiceThread if launched as a non-daemon... */
class OurParams {

	public:
		string s_argv_zero; /* copy argv[0] to this field */
		string s_manual_index_process_url;
		string s_manual_form_process_url;
		bool b_manual_load_next_edgar_form;
		string s_daily_index_backfill_days;
		string s_load_daily_indexes;
		string s_load_next_edgar_filing_header;

		OurParams(): b_manual_load_next_edgar_form(false){}

};/* class OurParams */

ostream& operator<<(ostream& s, OurParams& ourParams){

	s << "s_argv_zero = \"" << ourParams.s_argv_zero << "\"\n" 
	<< "s_manual_index_process_url = \"" << ourParams.s_manual_index_process_url << "\"\n" 
	<< "s_manual_form_process_url = \"" <<  ourParams.s_manual_form_process_url << "\"\n"
	<< "b_manual_load_next_edgar_form = " << boolalpha << ourParams.b_manual_load_next_edgar_form << "\n"
	<< "s_daily_index_backfill_days = \"" <<  ourParams.s_daily_index_backfill_days << "\"\n"
	<< "s_load_daily_indexes = \"" <<  ourParams.s_load_daily_indexes << "\"\n"
	<< "s_load_next_edgar_filing_header = \"" <<  ourParams.s_load_next_edgar_filing_header << "\"";

	return s;
}


/************ GLOBAL FUNCTION PROTOTYPES - BEGIN ******************/

//void getConfig( map<string, string>& configMap );
void getConfig( map<string, string>& configMap, OurParams& our_params );

string getLogFilePath( const string s_log_file_path );

void setupLogger(
	JDA::Logger& kenny_loggins,
	const string& s_debug_level, const string& s_debug_log_file_path,
	const string& s_debug_log_file_on, const string& s_debug_log_file_append,
	const string& s_debug_stdout_on
);

/**
* Keep setting logFilePath with current datestamp.
*
* Whereupon, at the crack of midnight, whence the datestamp changeth,
* JDA::Logger will by black witchcraft and deviltry beginneth a new logfileth.
*/
void resetLogFilePath(
	JDA::Logger& kenny_loggins, const string& s_debug_log_file_path
);


//void in_which_download_window_test(int evening_start_hour, int evening_end_hour);

/************ GLOBAL FUNCTION PROTOTYPES - END ******************/

/********** GLOBAL VARIABLES - BEGIN ******************/

// The name of the service for logging purposes...
const char *SERVICE_NAME = "forms_mean_daemon";

const char* S_VERSION = "0.01"; 


/********** GLOBAL VARIABLES - END ******************/


int ServiceThread(OurParams& our_params)
{
	cout.sync_with_stdio(true); // Get C and C++ I/O to play together nicely...

	string sWho = (string)SERVICE_NAME + "::ServiceThread";

	JDA::Logger le_logger;

	map<string, string> configMap;

	(le_logger)(JDA::Logger::INFO) << sWho << "(): " << "our_params = " << our_params << "..." << endl;

	(le_logger)(JDA::Logger::INFO) << sWho << "(): " << "Calling getConfig()..." << endl;
	getConfig( configMap, our_params );

	// NOTE: ostream& operator<<(ostream& s, const vector<string>& v); defined in utils.h
	(le_logger)(JDA::Logger::INFO) << sWho << "(): After calling getConfig(), configMap = " << configMap << "..." << endl;

	(le_logger)(JDA::Logger::INFO) << sWho << "(): " << "Calling setupLogger()..." << endl;

	setupLogger( le_logger,
		configMap["debug_level"], configMap["debug_log_file_path"],
		configMap["debug_log_file_on"], configMap["debug_log_file_append"],
		configMap["debug_stdout_on"]
	);

	(le_logger)(JDA::Logger::INFO) << sWho << "(): " << "\n" 
	<< "****************************************************" << "\n"
	<< "***  Welcome to The Forms Mean Daemon, Mr. Bond! ***" << "\n"
	<< "****************************************************" << endl;

	(le_logger)(JDA::Logger::INFO) << sWho << "(): " << "S_VERSION = " << S_VERSION << "..." << endl;

	(le_logger)(JDA::Logger::INFO) << sWho << "(): " << "Got our_params =\n" << (our_params) << endl;

	// NOTE: ostream& operator<<(ostream& s, const vector<string>& v); defined in utils.h
	(le_logger)(JDA::Logger::INFO) << sWho << "(): configMap = " << configMap << "..." << endl;

	// i_ftp_debug defaults to 0 if parsing fails...
	int i_ftp_debug = JDA::Utils::stringToInt( configMap["edgar_ftp_debug"], 0 );
	(le_logger)(JDA::Logger::INFO) << sWho << "(): i_ftp_debug = " << i_ftp_debug << "..." << endl;

	// i_download_sleep_time defaults to JDA::FormsMeanCommon::DEFAULT_DOWNLOAD_SLEEP_TIME if parsing fails...
	int i_download_sleep_time = JDA::Utils::stringToInt( configMap["download_sleep_time"], JDA::FormsMeanCommon::DEFAULT_DOWNLOAD_SLEEP_TIME );
	(le_logger)(JDA::Logger::INFO) << sWho << "(): i_download_sleep_time = " << i_download_sleep_time << "..." << endl;

	JDA::Forms forms;
	forms.setPLogger( &le_logger );
	forms.setDbUrl( configMap["db_url"] );
	forms.setDbName( configMap["db_name"] );
	forms.setFtpServer( configMap["edgar_ftp_server"] );
	forms.setFtpDebug( i_ftp_debug );

	if( our_params.s_manual_index_process_url.length() > 0 ){

		(le_logger)(JDA::Logger::INFO) << sWho << "(): " << "our_params.s_manual_index_process_url = \""
			<< our_params.s_manual_index_process_url << "\":\n" 
			<< "\t" << "Running forms.loadFromEdgarIndexUrl( \"" << our_params.s_manual_index_process_url << "\" ), "
			<< "and exiting the daemon..." << endl;

		try {
			forms.loadFromEdgarIndexUrl( our_params.s_manual_index_process_url );
		}
		catch(JDA::FtpClient::FtpException& e) {
			(le_logger)(JDA::Logger::ERROR) << sWho << "(): Caught JDA::FtpClient::FtpException: \"" 
				<< e.what() << "\"" << endl;
		}
		catch(std::exception& e ){
			(le_logger)(JDA::Logger::ERROR) << sWho << "(): Caught std::exception: \"" 
				<< e.what() << "\"" << endl;
		}
		catch(...){
			(le_logger)(JDA::Logger::ERROR) << sWho << "(): Caught unknown exception." << endl;
		}

		(le_logger)(JDA::Logger::INFO) << sWho << "(): " << "Exiting daemon now..." << endl;

		return 0;

	}/* if( our_params.s_manual_index_process_url.length() > 0 ) */
	else if( our_params.s_manual_form_process_url.length() > 0 ){

		(le_logger)(JDA::Logger::INFO) << sWho << "(): " << "our_params.s_manual_form_process_url = \""
			<< our_params.s_manual_form_process_url << "\":\n" 
			<< "\t" << "Running forms.loadFromEdgarFormUrl( \"" << our_params.s_manual_form_process_url << "\" ), "
			<< "and exiting the daemon..." << endl;

		try {
			forms.loadFromEdgarFormUrl( our_params.s_manual_form_process_url );
		}
		catch(JDA::FtpClient::FtpException& e) {
			(le_logger)(JDA::Logger::ERROR) << sWho << "(): Caught JDA::FtpClient::FtpException: \"" 
				<< e.what() << "\"" << endl;
		}
		catch(std::exception& e ){
			(le_logger)(JDA::Logger::ERROR) << sWho << "(): Caught std::exception: \"" 
				<< e.what() << "\"" << endl;
		}
		catch(...){
			(le_logger)(JDA::Logger::ERROR) << sWho << "(): Caught unknown exception." << endl;
		}

		(le_logger)(JDA::Logger::INFO) << sWho << "(): " << "Exiting daemon now..." << endl;

		return 0;

	}/* if( our_params.s_manual_index_process_url.length() > 0 ) */
	else if( our_params.b_manual_load_next_edgar_form == true ){

		(le_logger)(JDA::Logger::INFO) << sWho << "(): " << "our_params.b_manual_load_next_edgar_form is TRUE...\n"
			<< "\t" << "Running forms.loadNextEdgarForm(), and exiting the daemon..." << endl;

		try {
			forms.loadNextEdgarForm();
		}
		catch(JDA::FtpClient::FtpException& e) {
			(le_logger)(JDA::Logger::ERROR) << sWho << "(): Caught JDA::FtpClient::FtpException: \"" 
				<< e.what() << "\"" << endl;
		}
		catch(std::exception& e ){
			(le_logger)(JDA::Logger::ERROR) << sWho << "(): Caught std::exception: \"" 
				<< e.what() << "\"" << endl;
		}
		catch(...){
			(le_logger)(JDA::Logger::ERROR) << sWho << "(): Caught unknown exception." << endl;
		}

		(le_logger)(JDA::Logger::INFO) << sWho << "(): " << "Exiting daemon now..." << endl;

		return 0;

	}/* if( our_params.s_manual_index_process_url.length() > 0 ) */

	/* ...placeholder for daemon loop... */
	while( 1 ){
		(le_logger)(JDA::Logger::INFO) << sWho << "(): " << "main daemon loop: Running forms.loadNextEdgarForm()..." << endl;

		try {
			forms.loadNextEdgarForm();
		}
		catch(JDA::FtpClient::FtpException& e) {
			(le_logger)(JDA::Logger::ERROR) << sWho << "(): Caught JDA::FtpClient::FtpException: \"" 
				<< e.what() << "\"" << endl;
		}
		catch(std::exception& e ){
			(le_logger)(JDA::Logger::ERROR) << sWho << "(): Caught std::exception: \"" 
				<< e.what() << "\"" << endl;
		}
		catch(...){
			(le_logger)(JDA::Logger::ERROR) << sWho << "(): Caught unknown exception." << endl;
		}

		(le_logger)(JDA::Logger::INFO) << sWho << "(): " << "main daemon loop: Sleeping for i_download_sleep_time = " << i_download_sleep_time << " second(s)..." << endl;  
		::Sleep( 1000 * i_download_sleep_time );
	}
	
	(le_logger)(JDA::Logger::INFO) << sWho << "(): " << "Let off some steam, Bennett!" << endl;

	return 0;

}/* ServiceThread() */

int main(int argc, char** argv)
{
	OurParams ourParams;

	cout << "Setting ourParams.s_argv_zero equal to argv[0]..." << endl;
	ourParams.s_argv_zero = argv[0];
	cout << "ourParams.s_argv_zero = \"" << ourParams.s_argv_zero << "\"..." << endl;

	for(int i=1 ; i < argc; i++ ){
		if( strcmp( argv[i], "--manual-index-process-url" ) == 0 ){
			if( i+1 < argc ){
				cout << "Setting ourParams.s_manual_index_process_url equal to argv[++i]..." << endl;
				ourParams.s_manual_index_process_url = argv[++i];
			}
		}
		else if( strcmp( argv[i], "--manual-form-process-url" ) == 0 ){
			if( i+1 < argc ){
				cout << "Setting ourParams.s_manual_form_process_url equal to argv[++i]..." << endl;
				ourParams.s_manual_form_process_url = argv[++i];
			}
		}
		else if( strcmp( argv[i], "--manual-load-next-edgar-form" ) == 0 ){
			cout << "Setting ourParams.b_manual_load_next_edgar_form equal to true..." << endl;
			ourParams.b_manual_load_next_edgar_form = true;
		}
		else if( strcmp( argv[i], "--daily-index-backfill-days" ) == 0 ){
			if( i+1 < argc ){
				cout << "Setting ourParams.s_daily_index_backfill_days equal to argv[++i]..." << endl;
				ourParams.s_daily_index_backfill_days = argv[++i];
			}
		}
		else if( strcmp( argv[i], "--load-daily-indexes" ) == 0 ){
			if( i+1 < argc ){
				cout << "Setting ourParams.s_load_daily_indexes equal to argv[++i]..." << endl;
				ourParams.s_load_daily_indexes = argv[++i];
			}
		}
		else if( strcmp( argv[i], "--load-next-edgar-filing-header" ) == 0 ){
			if( i+1 < argc ){
				cout << "Setting ourParams.s_load_next_edgar_filing_header equal to argv[++i]..." << endl;
				ourParams.s_load_next_edgar_filing_header = argv[++i];
			}
		}
	}

	cout << "Launching " << SERVICE_NAME << "..." << endl;

	int dw_return = ServiceThread( ourParams );

	return dw_return;

}/* main() */


void getConfig( map<string, string>& configMap, OurParams& our_params ) {

	string sWho = "::getConfig";

	cout << sWho << "()..." << endl;

	//string sExecutablePath = JDA::Utils::getExecutablePath(); 
	string sExecutablePath = our_params.s_argv_zero;

	cout << sWho << "(): Calling JDA::Utils::getDefaultConfigFilePath( sExecutablePath = \"" << sExecutablePath << "\" )..." << endl;
	string sConfigFilePath = JDA::Utils::getDefaultConfigFilePath( sExecutablePath );

	/* Set to defaults first, then they can be overridden 
	* by settings in *.ini file, then they are processed
	* in setupLogger()...
	*/
	configMap["debug_level"] = JDA::FormsMeanCommon::DEFAULT_DEBUG_LEVEL;
	configMap["debug_log_file_path"] = JDA::FormsMeanCommon::DEFAULT_DEBUG_LOG_FILE_PATH;
	configMap["debug_log_file_on"] = JDA::Utils::boolToString( JDA::FormsMeanCommon::DEFAULT_DEBUG_LOG_FILE_ON );
	configMap["debug_log_file_append"] = JDA::Utils::boolToString( JDA::FormsMeanCommon::DEFAULT_DEBUG_LOG_FILE_APPEND ); 
	configMap["debug_stdout_on"] = JDA::Utils::boolToString( JDA::FormsMeanCommon::DEFAULT_DEBUG_STDOUT_ON );

	configMap["load_daily_indexes"] = JDA::Utils::boolToString( JDA::FormsMeanCommon::DEFAULT_LOAD_DAILY_INDEXES );
	configMap["load_next_edgar_filing_header"] = JDA::Utils::boolToString( JDA::FormsMeanCommon::DEFAULT_LOAD_NEXT_EDGAR_FILING_HEADER );

	configMap["download_sleep_time"] = JDA::Utils::intToString( JDA::FormsMeanCommon::DEFAULT_DOWNLOAD_SLEEP_TIME );

	//JDA::Utils::debug = 1;
	try {
		cout << sWho << "(): Calling JDA::Utils::read_config_file( sConfigFilePath = \"" << sConfigFilePath << "\", configMap )..." << endl;
		JDA::Utils::read_config_file( sConfigFilePath, configMap );

	}
	catch(JDA::Utils::Exception& e) {

		cerr << sWho << "(): Caught exception during JDA::Utils::read_config_file( \"" << sConfigFilePath 
			<< "\" ): \"" << e.what() << "\", using defaults..." << endl;
	
	}
	catch( ... ) {

		cerr << sWho << "(): Caught unknown exception during JDA::Utils::read_config_file( \"" << sConfigFilePath 
			<< "\" ) using defaults..." << endl;
	}

	cout << sWho << "(): Got it..." << endl;

}/* void getConfig() */

string getLogFilePath( const string s_log_file_path ){

	const char* sWho = "getLogFilePath";

	// For ubuntu, the NYC conversion gets stuck in an endless loop during is_dst()...nth_wday_in_month()...
	cout << sWho << "(): Calling sNycYYYYMMDD = JDA::Utils::get_nyc_datestamp()..." << endl;
	string sNycYYYYMMDD = JDA::Utils::get_nyc_datestamp();
	//string sNycYYYYMMDD = JDA::Utils::get_local_datestamp();

	string sLogFilePath = "";

	//if( s_log_file_path.length() == 0 ){
	//	string sSuffix = "-" + sNycYYYYMMDD + ".log";
	//	string sExecutablePath = JDA::Utils::getExecutablePath(); 
	//	sLogFilePath = JDA::Utils::getDefaultLogFilePath( sExecutablePath, sSuffix ); 
	//}
	//else {
		sLogFilePath = s_log_file_path;
		string sToken = "YYYY-MM-DD";
		size_t where = sLogFilePath.find( sToken );
		if( where != std::string::npos ){
			sLogFilePath.replace( where, sToken.length(), sNycYYYYMMDD );
		}
	//}

	return sLogFilePath;

}/* getLogFilePath() */

void setupLogger(
	JDA::Logger& kenny_loggins,
	const string& s_debug_level, const string& s_debug_log_file_path,
	const string& s_debug_log_file_on, const string& s_debug_log_file_append,
	const string& s_debug_stdout_on
){

	const char* sWho = "::setupLogger";

	kenny_loggins.setDebugLevel( JDA::FormsMeanCommon::DEFAULT_DEBUG_LEVEL );

	JDA::Logger::DebugLevelType debugLevel = JDA::Logger::LevelNameToDebugLevel( s_debug_level );

	if( debugLevel != JDA::Logger::NOTSET ){
		kenny_loggins(JDA::Logger::INFO) << sWho << "(): Setting debugLevel to " << JDA::Logger::getLevelName( debugLevel ) << "..." << endl;
		kenny_loggins.setDebugLevel( debugLevel );
	}


	bool bLogFileOn = JDA::Utils::stringToBool( s_debug_log_file_on );

	kenny_loggins(JDA::Logger::INFO) << sWho << "(): Setting logFileOn to " << JDA::Utils::boolToString( bLogFileOn ) << "..." << endl;
	kenny_loggins.setLogFileOn( bLogFileOn );
	
	bool bLogFileAppend = JDA::Utils::stringToBool( s_debug_log_file_append );

	kenny_loggins(JDA::Logger::INFO) << sWho << "(): Setting logFileAppend to " << JDA::Utils::boolToString( bLogFileAppend ) << "..." << endl;
	kenny_loggins.setLogFileAppend( bLogFileAppend );

	bool bLogStdoutOn = JDA::Utils::stringToBool( s_debug_stdout_on );

	kenny_loggins(JDA::Logger::INFO) << sWho << "(): Setting logStdoutOn to " << JDA::Utils::boolToString( bLogStdoutOn ) << "..." << endl;
	kenny_loggins.setStdoutOn( bLogStdoutOn );

	string sLogFilePath = getLogFilePath( s_debug_log_file_path );

	kenny_loggins(JDA::Logger::INFO) << sWho << "(): Setting logFilePath to '" << sLogFilePath << "'..." << endl;
	kenny_loggins.setLogFilePath( sLogFilePath );

	//kenny_loggins(JDA::Logger::INFO) << sWho << "(): Done with " << sWho << "()..." << endl;

}/* setupLogger() */


void resetLogFilePath(
	JDA::Logger& kenny_loggins, const string& s_debug_log_file_path
){

	string sWho = (string)SERVICE_NAME + "::resetLogFilePath";

	string sLogFilePath = getLogFilePath( s_debug_log_file_path );

	kenny_loggins(JDA::Logger::INFO) << sWho << "(): Setting logFilePath to '" << sLogFilePath << "'..." << endl;
	kenny_loggins.setLogFilePath( sLogFilePath );

}/* resetLogFilePath() */


//void in_which_download_window_test(JDA::Logger* p_logger, int evening_start_hour, int evening_end_hour){
//
//	const char* sWho = "::in_which_download_window_test";
//
//	for( int day_of_week = 0; day_of_week <= 6; day_of_week++ ){
//		for( int hour = 0; hour <= 23; hour++ ){
//
//			JDA::FormsMeanUtils::DownloadWindowType downloadWindowType = in_which_download_window( evening_start_hour, evening_end_hour, hour, day_of_week );
//
//			(*p_logger)(JDA::Logger::INFO) << sWho << "(): " 
//				<< "in_which_download_window( " << "hour = " << hour << ", day_of_week = " << day_of_week << ") = "  
//				<< downloadWindowType << " = " << downloadWindowTypeToString( downloadWindowType ) 
//				<< "..." << endl;
//		}
//	}
//
//}/* in_which_download_window_test() */


