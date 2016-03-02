#include <string>
#include <iostream>
#include <ctime>
#include <time.h>
#include "windows.h" // Sleep(), etc. (NOTE: In Windows it's Sleep(), in UNIX it's sleep()...) 

#include "utils.h" 
#include "logger.h" 

//#include "FtpClient.h"

//#include "FormsMeanCommon.h"
//#include "FormsMeanUtils.h"

using namespace std;

/************ GLOBAL FUNCTION PROTOTYPES - BEGIN ******************/
void getConfig();

string getLogFilePath();

void setupLogger( JDA::Logger& kenny_loggins );

enum DownloadWindowType { DAY, EVENING, WEEKEND }; 

static const char* downloadWindowTypeEnumStrings[] = { "DAY", "EVENING", "WEEKEND"};

static string downloadWindowTypeToString( DownloadWindowType downloadWindowType ){
	const char* sWho = "::downloadWindowTypeToString";
	// int iDownloadWindowType = (int)downloadWindowType;
	// JDA::Logger::log(JDA::Logger::TRACE) << sWho << "(): SHEMP: Moe, iDownloadWindowType = " << iDownloadWindowType << "..." << endl;
	// JDA::Logger::log(JDA::Logger::TRACE) << sWho << "(): SHEMP: Moe, Returning downloadWindowTypeEnumStrings[ " << iDownloadWindowType << "] = \"" << downloadWindowTypeEnumStrings[iDownloadWindowType] << "\"..." << endl;
	return downloadWindowTypeEnumStrings[ downloadWindowType ];
}

DownloadWindowType in_which_download_window(
	int evening_start_hour,
	int evening_end_hour,
	int hour = -1,
	int day_of_week = -1
);

void in_which_download_window_test(int evening_start_hour, int evening_end_hour);

/**
* Keep setting logFilePath with current datestamp.
*
* Whereupon, at the crack of midnight, whence the datestamp changeth,
* JDA::Logger will by black witchcraft and deviltry begineth a new logfileth.
*/
void resetLogFilePath();

/************ GLOBAL FUNCTION PROTOTYPES - END ******************/

/********** GLOBAL VARIABLES - BEGIN ******************/
map<string, string> ConfigMap;

// The name of the service for logging purposes...
const char *SERVICE_NAME = "forms_mean_daemon";

const char* S_VERSION = "0.01"; 

// Event used to hold ServiceMain from completing
HANDLE terminateEvent = NULL;

// Handle used to communicate status info with
// the SCM. Created by RegisterServiceCtrlHandler
SERVICE_STATUS_HANDLE serviceStatusHandle;

// Flags holding current state of service
BOOL pauseService = FALSE;
BOOL runningService = FALSE;

// Thread for the actual work
HANDLE threadHandle = 0;

string GB_MANUAL_INDEX_PROCESS_URL = ""; // Set via command-line argument in main() for manual loading of an edgar index...
string GB_MANUAL_FORM_PROCESS_URL = ""; // Set via command-line argument in main() for manual loading of an edgar form...

string GB_CMD_LINE_DAILY_INDEX_BACKFILL_DAYS = ""; // Set via command-line argument in main() to override both DEFAULT value and value from Config file.
string GB_CMD_LINE_LOAD_DAILY_INDEXES = ""; // Set via command-line argument in main() to override both DEFAULT value and value from Config file.
string GB_CMD_LINE_LOAD_NEXT_EDGAR_FILING_HEADER = ""; // Set via command-line argument in main() to override both DEFAULT value and value from Config file.

/********** GLOBAL VARIABLES - END ******************/

void ErrorHandler(char *s, DWORD err)
{
	const char* sWho = "ErrorHandler";

	ostringstream oss_out;

	oss_out << s << endl;
	oss_out << "Error number: " << err << endl;
	oss_out << "Error string: " << JDA::Utils::strerror(err) << endl;
	oss_out << "Calling ExitProcess(" << err << ")" << endl;
	JDA::Logger::log(JDA::Logger::FATAL) << sWho << "():\n" << oss_out.str() << endl;

	ExitProcess(err);
}

DWORD ServiceThread(LPDWORD param)
{
	cout.sync_with_stdio(true); // Get C and C++ I/O to play together nicely...

	string sWho = (string)SERVICE_NAME + "::ServiceThread";

	//JDA::Logger::log.debug = 1;

	getConfig();
	setupLogger( JDA::Logger::log );

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "\n" 
	<< "*************************************************" << "\n"
	<< "**  Welcome to The Forms Mean Daemon, Pilgrim  **" << "\n"
	<< "*************************************************" << endl;

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "S_VERSION = " << S_VERSION << "..." << endl;

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): ConfigMap = " << ConfigMap << "..." << endl;

	int iEdgarFtpDebug = 0;
	try {
		iEdgarFtpDebug = JDA::Utils::stringToInt( ConfigMap["edgar_ftp_debug"] );
	}catch( JDA::Utils::Exception& e ){ 
		JDA::Logger::log(JDA::Logger::WARN) << sWho << "(): " << "Trouble parsing 'edgar_ftp_debug' = \""
			<< ConfigMap["edgar_ftp_debug"] << "\" into an integer [\"" << e.what() << "\"],\n" 
			<< "so using default value of 0 (zero)..." << endl;
	}

	string sEdgarFtpServer = ConfigMap["edgar_ftp_server"]; 

	string sEdgarDailyIndexPath = ConfigMap["edgar_daily_index_path"]; 

	string sEdgarDailyIndexFileName = ConfigMap["edgar_daily_index_file_name"]; 

	string sEnvFtpProxySet = ConfigMap["env_ftp_proxy_set"];
	string sEnvHttpProxySet = ConfigMap["env_http_proxy_set"];
	string sEnvHttpsProxySet = ConfigMap["env_https_proxy_set"];

	string sEdgarFtpProxyUserPass = ConfigMap["edgar_ftp_proxy_user_pass"];

	bool bEdgarFtpNoProxy = JDA::Utils::stringToBool( ConfigMap["edgar_ftp_no_proxy"] );

	string sDbUrl = ConfigMap["db_url"];
	string sDbUser = ConfigMap["db_user"];
	string sDbPass = ConfigMap["db_pass"];

	int iDayDownloadSleepTime = JDA::FormsMeanCommon::DEF_DAY_DOWNLOAD_SLEEP_TIME;
	try {
		iDayDownloadSleepTime = JDA::Utils::stringToInt( ConfigMap["day_download_sleep_time"] );
	}catch( JDA::Utils::Exception& e ){ 
		JDA::Logger::log(JDA::Logger::WARN) << sWho << "(): " << "Trouble parsing 'day_download_sleep_time' = \""
			<< ConfigMap["day_download_sleep_time"] << "\" into an integer [\"" << e.what() << "\"],\n" 
			<< "so using default value..." << endl;
	}

	int iEveningDownloadSleepTime = JDA::FormsMeanCommon::DEF_EVENING_DOWNLOAD_SLEEP_TIME;
	try {
		iEveningDownloadSleepTime = JDA::Utils::stringToInt( ConfigMap["evening_download_sleep_time"] );
	}catch( JDA::Utils::Exception& e ){ 
		JDA::Logger::log(JDA::Logger::WARN) << sWho << "(): " << "Trouble parsing 'evening_download_sleep_time' = \""
			<< ConfigMap["evening_download_sleep_time"] << "\" into an integer [\"" << e.what() << "\"],\n" 
			<< "so using default value..." << endl;
	}

	int iWeekendDownloadSleepTime = JDA::FormsMeanCommon::DEF_WEEKEND_DOWNLOAD_SLEEP_TIME;
	try {
		iWeekendDownloadSleepTime = JDA::Utils::stringToInt( ConfigMap["weekend_download_sleep_time"] );
	}catch( JDA::Utils::Exception& e ){ 
		JDA::Logger::log(JDA::Logger::WARN) << sWho << "(): " << "Trouble parsing 'weekend_download_sleep_time' = \""
			<< ConfigMap["weekend_download_sleep_time"] << "\" into an integer [\"" << e.what() << "\"],\n" 
			<< "so using default value..." << endl;
	}

	int iEveningDownloadStartHour = JDA::FormsMeanCommon::DEF_EVENING_DOWNLOAD_START_HOUR;
	try {
		iEveningDownloadStartHour = JDA::Utils::stringToInt( ConfigMap["evening_download_start_hour"] );
	}catch( JDA::Utils::Exception& e ){ 
		JDA::Logger::log(JDA::Logger::WARN) << sWho << "(): " << "Trouble parsing 'evening_download_start_hour' = \""
			<< ConfigMap["evening_download_start_hour"] << "\" into an integer [\"" << e.what() << "\"],\n" 
			<< "so using default value..." << endl;
	}

	int iEveningDownloadEndHour = JDA::FormsMeanCommon::DEF_EVENING_DOWNLOAD_END_HOUR;
	try {
		iEveningDownloadEndHour = JDA::Utils::stringToInt( ConfigMap["evening_download_end_hour"] );
	}catch( JDA::Utils::Exception& e ){ 
		JDA::Logger::log(JDA::Logger::WARN) << sWho << "(): " << "Trouble parsing 'evening_download_end_hour' = \""
			<< ConfigMap["evening_download_end_hour"] << "\" into an integer [\"" << e.what() << "\"],\n" 
			<< "so using default value..." << endl;
	}

	//////////////////////////////////////////////////////////

	if( GB_CMD_LINE_DAILY_INDEX_BACKFILL_DAYS.length() > 0 ){
		JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "Overriding ConfigMap[\"daily_index_backfill_days\"] with GB_CMD_LINE_DAILY_INDEX_BACKFILL_DAYS = \"" << GB_CMD_LINE_DAILY_INDEX_BACKFILL_DAYS << "\"..." << endl;
		ConfigMap["daily_index_backfill_days"] = GB_CMD_LINE_DAILY_INDEX_BACKFILL_DAYS;
	}

	string sDailyIndexBackfillDays = ConfigMap["daily_index_backfill_days"];
	int iDailyIndexBackfillDays = -1;
	if( ! JDA::Utils::stringToInt( sDailyIndexBackfillDays, &iDailyIndexBackfillDays ) ){
		JDA::Logger::log(JDA::Logger::WARN) << sWho << "(): " << "Could not parse ConfigMap[\"daily_index_backfill_days\"] = \"" << sDailyIndexBackfillDays << "\" as an integer, defaulting to JDA::FormsMeanCommon::DEFAULT_DAILY_INDEX_BACKFILL_DAYS = " << JDA::FormsMeanCommon::DEFAULT_DAILY_INDEX_BACKFILL_DAYS << "..." << endl;
		iDailyIndexBackfillDays = JDA::FormsMeanCommon::DEFAULT_DAILY_INDEX_BACKFILL_DAYS;
	}

	string sDailyIndexRecheckInterval = ConfigMap["daily_index_recheck_interval"];
	int iDailyIndexRecheckInterval = -1;
	if( ! JDA::Utils::stringToInt( sDailyIndexRecheckInterval, &iDailyIndexRecheckInterval ) ){
		JDA::Logger::log(JDA::Logger::WARN) << sWho << "(): " << "Could not parse ConfigMap[\"daily_index_recheck_interval\"] = \"" << sDailyIndexRecheckInterval << "\" as an integer, defaulting to JDA::FormsMeanCommon::DEFAULT_DAILY_INDEX_RECHECK_INTERVAL = " << JDA::FormsMeanCommon::DEFAULT_DAILY_INDEX_RECHECK_INTERVAL << "..." << endl;
		iDailyIndexRecheckInterval = JDA::FormsMeanCommon::DEFAULT_DAILY_INDEX_RECHECK_INTERVAL;
	}

	string sDailyIndexRetryInterval = ConfigMap["daily_index_retry_interval"];
	int iDailyIndexRetryInterval = -1;
	if( ! JDA::Utils::stringToInt( sDailyIndexRetryInterval, &iDailyIndexRetryInterval ) ){
		JDA::Logger::log(JDA::Logger::WARN) << sWho << "(): " << "Could not parse ConfigMap[\"daily_index_retry_interval\"] = \"" << sDailyIndexRetryInterval << "\" as an integer, defaulting to JDA::FormsMeanCommon::DEFAULT_DAILY_INDEX_RETRY_INTERVAL = " << JDA::FormsMeanCommon::DEFAULT_DAILY_INDEX_RETRY_INTERVAL << "..." << endl;
		iDailyIndexRetryInterval = JDA::FormsMeanCommon::DEFAULT_DAILY_INDEX_RETRY_INTERVAL;
	}

	string sDailyIndexMaxAttempts = ConfigMap["daily_index_max_attempts"];
	int iDailyIndexMaxAttempts = -1;
	if( ! JDA::Utils::stringToInt( sDailyIndexMaxAttempts, &iDailyIndexMaxAttempts ) ){
		JDA::Logger::log(JDA::Logger::WARN) << sWho << "(): " << "Could not parse ConfigMap[\"daily_index_max_attempts\"] = \"" << sDailyIndexMaxAttempts << "\" as an integer, defaulting to JDA::FormsMeanCommon::DEFAULT_DAILY_INDEX_MAX_ATTEMPTS = " << JDA::FormsMeanCommon::DEFAULT_DAILY_INDEX_MAX_ATTEMPTS << "..." << endl;
		iDailyIndexMaxAttempts = JDA::FormsMeanCommon::DEFAULT_DAILY_INDEX_MAX_ATTEMPTS;
	}

	if( GB_CMD_LINE_LOAD_DAILY_INDEXES.length() > 0 ){
		JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "Overriding ConfigMap[\"load_daily_indexes\"] with GB_CMD_LINE_LOAD_DAILY_INDEXES = \"" << GB_CMD_LINE_LOAD_DAILY_INDEXES << "\"..." << endl;
		ConfigMap["load_daily_indexes"] = GB_CMD_LINE_LOAD_DAILY_INDEXES;
	}
	bool bLoadDailyIndexes = JDA::Utils::stringToBool( ConfigMap["load_daily_indexes"] );

	if( GB_CMD_LINE_LOAD_NEXT_EDGAR_FILING_HEADER.length() > 0 ){
		JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "Overriding ConfigMap[\"load_next_edgar_filing_header\"] with GB_CMD_LINE_LOAD_NEXT_EDGAR_FILING_HEADER = \"" << GB_CMD_LINE_LOAD_NEXT_EDGAR_FILING_HEADER << "\"..." << endl;
		ConfigMap["load_next_edgar_filing_header"] = GB_CMD_LINE_LOAD_NEXT_EDGAR_FILING_HEADER;
	}
	bool bLoadNextEdgarFilingHeader = JDA::Utils::stringToBool( ConfigMap["load_next_edgar_filing_header"] );

	/////////////////// LOG IT! ////////////
	
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "BENNETT: Welcome back, John...so glad you could make it...!" << endl;
	
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "GB_MANUAL_INDEX_PROCESS_URL = \"" << GB_MANUAL_INDEX_PROCESS_URL << "\"..." << endl;
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "GB_MANUAL_FORM_PROCESS_URL = \"" << GB_MANUAL_FORM_PROCESS_URL << "\"..." << endl;

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "GB_CMD_LINE_DAILY_INDEX_BACKFILL_DAYS = \"" << GB_CMD_LINE_DAILY_INDEX_BACKFILL_DAYS << "\"..." << endl;
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "GB_CMD_LINE_LOAD_DAILY_INDEXES = \"" << GB_CMD_LINE_LOAD_DAILY_INDEXES << "\"..." << endl;
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "GB_CMD_LINE_LOAD_NEXT_EDGAR_FILING_HEADER = \"" << GB_CMD_LINE_LOAD_NEXT_EDGAR_FILING_HEADER << "\"..." << endl;
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "iEdgarFtpDebug = " << iEdgarFtpDebug << "..." << endl;

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "sEdgarFtpServer = '" << sEdgarFtpServer << "'..." << endl;
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "sEdgarDailyIndexPath = '" << sEdgarDailyIndexPath << "'..." << endl;
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "sEdgarDailyIndexFileName = '" << sEdgarDailyIndexFileName << "'..." << endl;

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): SHEMP: Note dhat curl uses ENV{'ftp_proxy'}, ENV{'http_proxy'}, and ENV{'https_proxy'}, Moe, so printing them now for your information..." << endl;
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): SHEMP: Moe, FYI, ENV{'ftp_proxy'} = \"" << JDA::Utils::getenv("ftp_proxy") << "\"..." << endl;  
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): SHEMP: Moe, FYI, ENV{'http_proxy'} = \"" << JDA::Utils::getenv("http_proxy") << "\"..." << endl;  
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): SHEMP: Moe, FYI, ENV{'https_proxy'} = \"" << JDA::Utils::getenv("https_proxy") << "\"..." << "\n" << endl;

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "sEnvFtpProxySet = '" << sEnvFtpProxySet << "'..." << endl;
	if( sEnvFtpProxySet.length() > 0 ){
		string s_key = "ftp_proxy";
		JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): SHEMP: Moe, sEnvFtpProxySet has length > 0, so callin' setenv(\"" << s_key << "\", \"" << sEnvFtpProxySet << "\") now, OK, Moe...?" << endl;
		bool b_return_code = JDA::Utils::setenv( s_key, sEnvFtpProxySet ); 
		JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): SHEMP: Moe, retoyn code from setenv() = " << JDA::Utils::boolToString( b_return_code ) << "..." << endl;
		JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): SHEMP: Moe, now ENV{'" << s_key << "'} = \"" << JDA::Utils::getenv(s_key) << "\"..." << endl;  
	}

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "sEnvHttpProxySet = '" << sEnvHttpProxySet << "'..." << endl;
	if( sEnvHttpProxySet.length() > 0 ){
		string s_key = "http_proxy";
		JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): SHEMP: Moe, sEnvHttpProxySet has length > 0, so callin' setenv(\"" << s_key << "\", \"" << sEnvHttpProxySet << "\") now, OK, Moe...?" << endl;
		bool b_return_code = JDA::Utils::setenv( s_key, sEnvHttpProxySet ); 
		JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): SHEMP: Moe, retoyn code from setenv() = " << JDA::Utils::boolToString( b_return_code ) << "..." << endl;
		JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): SHEMP: Moe, now ENV{'" << s_key << "'} = \"" << JDA::Utils::getenv(s_key) << "\"..." << endl;  
	}

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "sEnvHttpsProxySet = '" << sEnvHttpsProxySet << "'..." << endl;
	if( sEnvHttpsProxySet.length() > 0 ){
		string s_key = "https_proxy";
		JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): SHEMP: Moe, sEnvHttpsProxySet has length > 0, so callin' setenv(\"" << s_key << "\", \"" << sEnvHttpsProxySet << "\") now, OK, Moe...?" << endl;
		bool b_return_code = JDA::Utils::setenv( s_key, sEnvHttpsProxySet ); 
		JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): SHEMP: Moe, retoyn code from setenv() = " << JDA::Utils::boolToString( b_return_code ) << "..." << endl;
		JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): SHEMP: Moe, now ENV{'" << s_key << "'} = \"" << JDA::Utils::getenv(s_key) << "\"..." << endl;  
	}

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "bEdgarFtpNoProxy = '" << JDA::Utils::boolToString( bEdgarFtpNoProxy ) << "'..." << endl;

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "sEdgarFtpProxyUserPass = '" << sEdgarFtpProxyUserPass << "'..." << endl;
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "bEdgarFtpNoProxy = '" << JDA::Utils::boolToString( bEdgarFtpNoProxy ) << "'..." << endl;

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "sDbUrl = '" << sDbUrl << "'..." << endl;
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "sDbUser = '" << sDbUser << "'..." << endl;
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "sDbPass = '" << sDbPass << "'..." << endl;

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "iDayDownloadSleepTime = " << iDayDownloadSleepTime << "..." << endl;
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "iEveningDownloadSleepTime = " << iEveningDownloadSleepTime << "..." << endl;
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "iWeekendDownloadSleepTime = " << iWeekendDownloadSleepTime << "..." << endl;

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "iEveningDownloadStartHour = " << iEveningDownloadStartHour << "..." << endl;
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "iEveningDownloadEndHour = " << iEveningDownloadEndHour << "..." << "\n" << endl;

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "iDailyIndexBackfillDays = " << iDailyIndexBackfillDays << "..." << endl;
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "iDailyIndexRecheckInterval = " << iDailyIndexRecheckInterval << "..." << endl;
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "iDailyIndexRetryInterval = " << iDailyIndexRetryInterval << "..." << endl;
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "iDailyIndexMaxAttempts = " << iDailyIndexMaxAttempts << "..." << "\n" << endl;

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "bLoadDailyIndexes = " << bLoadDailyIndexes << "..." << endl;
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "bLoadNextEdgarFilingHeader = " << bLoadNextEdgarFilingHeader << "..." << endl;
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "bDownfillNextAgentDbDatum = " << bDownfillNextAgentDbDatum << "..." << "\n" << endl;

	// One time index load and exit...
	if( GB_MANUAL_INDEX_PROCESS_URL.length() > 0 ){ 

		JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "GB_MANUAL_INDEX_PROCESS_URL = \"" << GB_MANUAL_INDEX_PROCESS_URL << "\" has been supplied on the command line, will call load load_edgar_index_and_log_it(), then exit the application..." << endl;

		JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "Calling load_edgar_index_and_log_it()..." << endl;

		int i_return_code = JDA::FormsMeanUtils::load_edgar_index_and_log_it(
					&(JDA::Logger::log),
					sDbUrl, sDbUser, sDbPass,
					GB_MANUAL_INDEX_PROCESS_URL,
					iEdgarFtpDebug, bEdgarFtpNoProxy, sEdgarFtpProxyUserPass
		);


		
		JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "Exiting process now with exit_code = i_return_code = " << i_return_code << "..." << endl;

		return i_return_code;

	}/* if( GB_MANUAL_INDEX_PROCESS_URL.length() > 0 ) */

	if( GB_MANUAL_FORM_PROCESS_URL.length() > 0 ){ 

		JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "GB_MANUAL_FORM_PROCESS_URL = \"" << GB_MANUAL_FORM_PROCESS_URL << "\" has been supplied on the command line, will call load load_edgar_form_and_log_it(), then exit..." << endl;

		JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "Calling load_edgar_form_and_log_it()..." << endl;

		int i_return_code = JDA::FormsMeanUtils::load_edgar_form_and_log_it(
					sDbUrl, sDbUser, sDbPass,
					GB_MANUAL_FORM_PROCESS_URL,
					iEdgarFtpDebug, bEdgarFtpNoProxy, sEdgarFtpProxyUserPass
		);
		
		JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "Exiting process now with exit_code = i_return_code = " << i_return_code << "..." << endl;

		return i_return_code;

	}/* if( GB_MANUAL_FORM_PROCESS_URL.length() > 0 ) */

	in_which_download_window_test( iEveningDownloadStartHour, iEveningDownloadEndHour );
	

	try {

		int iDailyIndexReturn = -1;
		int iFilingHeaderReturn = -1;

		time_t time_t_last_load_yesterweekdays_daily_index_call = 0; 

		time_t time_t_now = time(NULL);
		time_t time_t_elapsed_time = 0;

		JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "SCOTTY: Entering the main EDGAR loop, Captain..." << endl;

		// The Main EDGAR Loop...
		while( true ){

			JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "SCOTTY: Captain, what about load_yesterweekdays_daily_edgar_index()...?" << endl;

			if( bLoadDailyIndexes ){

				time_t_now = time(NULL);

				time_t_elapsed_time = time_t_now - time_t_last_load_yesterweekdays_daily_index_call;

				if( time_t_elapsed_time >= iDailyIndexRecheckInterval ){
					JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "SCOTTY: Captain, bLoadDailyIndexes is TRUE, and time_t_elapsed time = " << JDA::Utils::commify( time_t_elapsed_time ) << ", is greater than or equal to iDailyIndexRecheckInterval = " << iDailyIndexRecheckInterval << ", so calling load_yesterweekdays_daily_edgar_index()..." << endl;

					iDailyIndexReturn = JDA::FormsMeanUtils::load_yesterweekdays_daily_edgar_index(
							&JDA::Logger::log,
							sDbUrl, sDbUser, sDbPass,
							sEdgarFtpServer, sEdgarDailyIndexPath, sEdgarDailyIndexFileName,
							iEdgarFtpDebug, bEdgarFtpNoProxy, sEdgarFtpProxyUserPass, 
							iDailyIndexBackfillDays, iDailyIndexRetryInterval, iDailyIndexMaxAttempts
					);

					time_t_last_load_yesterweekdays_daily_index_call = time(NULL);
				}/* if( time_t_elapsed_time >= iDailyIndexRecheckInterval ) */
				else{
					JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "SCOTTY: Captain, bLoadDailyIndexes is TRUE, but time_t_elapsed time = " << JDA::Utils::commify( time_t_elapsed_time ) << ", is less than iDailyIndexRecheckInterval = " << iDailyIndexRecheckInterval << ", so NOT calling load_yesterweekdays_daily_edgar_index(), and setting iDailyIndexReturn equal to 0 to indicate that EDGAR has not been hit.." << endl;
					iDailyIndexReturn = 0;
				}
			}
			else {
				JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "SCOTTY: Captain, bLoadDailyIndexes is FALSE, so NOT calling load_yesterweekdays_daily_edgar_index(), and setting iDailyIndexReturn to 0 to indicate that EDGAR has NOT been hit..." << endl;
				iDailyIndexReturn = 0;
			}
			JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "SCOTTY: iDailyIndexReturn = " << iDailyIndexReturn << "..." << endl;

			if( iDailyIndexReturn == 0 ){

				JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "SCOTTY: Looks like an EDGAR index was NOT downloaded, Captain, so we can hit up EDGAR for a Filing Header..." << endl;

				if( bLoadNextEdgarFilingHeader ){
					JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "SCOTTY: Calling load_next_filing_header()..." << endl;
					iFilingHeaderReturn = JDA::FormsMeanUtils::load_next_edgar_filing_header(
							sDbUrl, sDbUser, sDbPass,
							sEdgarFtpServer, 
							iEdgarFtpDebug, bEdgarFtpNoProxy, sEdgarFtpProxyUserPass 
					);
				}
				else {
					JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "SCOTTY: Actually, bLoadNextEdgarFilingHeader is FALSE, so NOT calling load_next_edgar_filing_header(), and setting iFilingHeaderReturn to 0 to indicate that EDGAR has not been hit..." << endl;
					iFilingHeaderReturn = 0;
				}

				JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "SCOTTY: Captain, iFilingHeaderReturn = " << iFilingHeaderReturn << "..." << endl;


			}/* if( iDailyIndexReturn == 0 ) */
			else if( iDailyIndexReturn > 0 ){
				JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "SCOTTY: Looks like a daily index was downloaded, Captain, so we won't hit EDGAR with a filing header..." << endl;
			}
			else if( iDailyIndexReturn < 0 ){
				JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "SCOTTY: Looks like a daily index download was ATTEMPTED, but not quite successfully, Captain, so we won't hit EDGAR with a filing header..." << endl;
			}

			int iSleepTime = iDayDownloadSleepTime;

			DownloadWindowType downloadWindowType = in_which_download_window( iEveningDownloadStartHour, iEveningDownloadEndHour );

			JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): SPOCK: based on the Enterprise chronometer, downloadWindowType = " << downloadWindowType <<  " = " << downloadWindowTypeToString( downloadWindowType ) << ", Captain..." << endl;

			if( downloadWindowType == DAY ){
				JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): SPOCK: We appear to be in the \"day\" download window, Captain..." << endl;
				iSleepTime = iDayDownloadSleepTime;
			}
			else if( downloadWindowType == EVENING ){
				JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): SPOCK: We appear to be in the \"evening\" download window, Captain..." << endl;
				iSleepTime = iEveningDownloadSleepTime;
			}
			else if( downloadWindowType == WEEKEND ){
				JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): SPOCK: We appear to be in the \"weekend\" download window, Captain..." << endl;
				iSleepTime = iWeekendDownloadSleepTime;
			}

			JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): SPOCK: Going into suspended animation for " << iSleepTime << " second" << (iSleepTime!=1?"s":"") << ", Captain..." << endl;

			::Sleep( iSleepTime * 1000 );		

			JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): SPOCK: Back from suspended animation, Captain..." << endl;

			// If we crossed past midnight, we'll start a new log file for the new day...
			resetLogFilePath();

		}/* while( true ) -- The Main EDGAR Loop */

	} catch( ... ) {

		JDA::Logger::log(JDA::Logger::FATAL) << sWho << "(): " << "SCOTTY: Caught an unknown exception, Captain, exiting the loop..." << endl;

	}

	JDA::Logger::log(JDA::Logger::WARN) << sWho << "(): " << "SCOTTY: Abnormal exit from loop, Captain..." << endl;

	return 0;

}/* DWORD ServiceThread(LPDWORD param) */


// Initializes the service by starting its thread
BOOL InitService()
{
	const string sWho = "InitService";

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): Como esta?..." << endl;

	DWORD id;

	// Start the service's thread
	threadHandle = CreateThread(0, 0,
		(LPTHREAD_START_ROUTINE) ServiceThread,
		0, 0, &id);

	if (threadHandle==0){
		return FALSE;
	}
	else
	{
		runningService = TRUE;
		return TRUE;
	}
}

// Resumes a paused service
VOID ResumeService()
{
	const string sWho = "ResumeService";

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " << "Welcome back, John...so glad you could make it..." << endl;

	pauseService=FALSE;
	ResumeThread(threadHandle);
}

// Pauses the service
VOID PauseService()
{
	const string sWho = "PauseService";

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): \"I'll be back, Bennett...!\"" << endl;

	pauseService = TRUE;
	SuspendThread(threadHandle);
}

// Stops the service by allowing ServiceMain to
// complete
VOID StopService() 
{
	const string sWho = "StopService";

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): \"Let off some steam, Bennett!\"" << endl;

	runningService=FALSE;
	// Set the event that is holding ServiceMain
	// so that ServiceMain can return
	SetEvent(terminateEvent);
}

// This function consolidates the activities of 
// updating the service status with
// SetServiceStatus
BOOL SendStatusToSCM (DWORD dwCurrentState,
	DWORD dwWin32ExitCode, 
	DWORD dwServiceSpecificExitCode,
	DWORD dwCheckPoint,
	DWORD dwWaitHint)
{
	const string sWho = "SendStatusToSCM";

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): \"You're a funny guy, Sully, I like you...that's why I'm going to kill you last...\"" << endl;

	BOOL success;
	SERVICE_STATUS serviceStatus;

	// Fill in all of the SERVICE_STATUS fields
	serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	serviceStatus.dwCurrentState = dwCurrentState;

	// If in the process of something, then accept
	// no control events, else accept anything
	if (dwCurrentState == SERVICE_START_PENDING)
		serviceStatus.dwControlsAccepted = 0;
	else
		serviceStatus.dwControlsAccepted = 
			SERVICE_ACCEPT_STOP |
			SERVICE_ACCEPT_PAUSE_CONTINUE |
			SERVICE_ACCEPT_SHUTDOWN;

	// if a specific exit code is defines, set up
	// the win32 exit code properly
	if (dwServiceSpecificExitCode == 0)
		serviceStatus.dwWin32ExitCode =
			dwWin32ExitCode;
	else
		serviceStatus.dwWin32ExitCode = 
			ERROR_SERVICE_SPECIFIC_ERROR;

	serviceStatus.dwServiceSpecificExitCode =
		dwServiceSpecificExitCode;

	serviceStatus.dwCheckPoint = dwCheckPoint;
	serviceStatus.dwWaitHint = dwWaitHint;

	// Pass the status record to the SCM
	success = SetServiceStatus (serviceStatusHandle, &serviceStatus);

	//JDA::Logger::debugPrintf(1, "[%s]: %s(): return code from SetServiceStatus() = %d...\n", JDA::Utils::get_timestamp().c_str(), sWho.c_str(), success );
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): return code from SetServiceStatus() = " << success << "..." << endl;

	if (!success) {

		JDA::Logger::log(JDA::Logger::WARN) << sWho << "(): return code from SetServiceStatus() = " << success << " is false: error = \"" << JDA::Utils::s_error() << "\", but NOT calling StopService()..." << endl;

		//JDA::Logger::log(JDA::Logger::ERROR) << sWho << "(): return code from SetServiceStatus() = " << success << " is false: error = \"" << JDA::Utils::s_error() << "\", so calling StopService()..." << endl;
		//StopService();
	}

	return success;
}

// Dispatches events received from the service 
// control manager
VOID ServiceCtrlHandler (DWORD controlCode) 
{
	const string sWho = "ServiceCtrlHandler";

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "( controlCode = " << controlCode << "): \"Do me a favor, don't disturb my friend...he's dead tired.\"" << endl;

	DWORD  currentState = 0;
	BOOL success;

	switch(controlCode)
	{
		// There is no START option because
		// ServiceMain gets called on a start

		// Stop the service
		case SERVICE_CONTROL_STOP:
			currentState = SERVICE_STOP_PENDING;
			// Tell the SCM what's happening
			success = SendStatusToSCM(
				SERVICE_STOP_PENDING,
				NO_ERROR, 0, 1, 5000);
			// Not much to do if not successful

			// Stop the service
			StopService();
			return;

		// Pause the service
		case SERVICE_CONTROL_PAUSE:
			if (runningService && !pauseService)
			{
				// Tell the SCM what's happening
				success = SendStatusToSCM(
					SERVICE_PAUSE_PENDING,
					NO_ERROR, 0, 1, 1000);
				PauseService();
				currentState = SERVICE_PAUSED;
			}
			break;

		// Resume from a pause
		case SERVICE_CONTROL_CONTINUE:
			if (runningService && pauseService)
			{
				// Tell the SCM what's happening
				success = SendStatusToSCM(
					SERVICE_CONTINUE_PENDING,
					NO_ERROR, 0, 1, 1000);
					ResumeService();
					currentState = SERVICE_RUNNING;
			}
			break;

		// Update current status
		case SERVICE_CONTROL_INTERROGATE:
			// it will fall to bottom and send status
			break;

		// Do nothing in a shutdown. Could do cleanup
		// here but it must be very quick.
		case SERVICE_CONTROL_SHUTDOWN:
			// Do nothing on shutdown
			return;

		default:
 			break;
	}
	SendStatusToSCM(currentState, NO_ERROR,
		0, 0, 0);
}

// Handle an error from ServiceMain by cleaning up
// and telling SCM that the service didn't start.
VOID terminate(DWORD error)
{
	const string sWho = "terminate";

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): \"Typically, the subject being copied is terminated...\"" << endl;

	// if terminateEvent has been created, close it.
	if (terminateEvent)
		CloseHandle(terminateEvent);

	// Send a message to the scm to tell about
	// stopage
	if (serviceStatusHandle)
		SendStatusToSCM(SERVICE_STOPPED, error,
			0, 0, 0);

	// If the thread has started kill it off
	if (threadHandle)
		CloseHandle(threadHandle);

	// Do not need to close serviceStatusHandle
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): \"Terminated!\"" << endl;
}

// ServiceMain is called when the SCM wants to
// start the service. When it returns, the service
// has stopped. It therefore waits on an event
// just before the end of the function, and
// that event gets set when it is time to stop. 
// It also returns on any error because the
// service cannot start if there is an eror.
VOID ServiceMain(DWORD argc, LPTSTR *argv) 
{
	BOOL success;

	// immediately call Registration function
	serviceStatusHandle =
		RegisterServiceCtrlHandler(
			(LPSTR)SERVICE_NAME,
			(LPHANDLER_FUNCTION) ServiceCtrlHandler);
	if (!serviceStatusHandle)
	{
		terminate(GetLastError());
		return;
	}

	// Notify SCM of progress
	success = SendStatusToSCM(
		SERVICE_START_PENDING,
		NO_ERROR, 0, 1, 5000);
	if (!success)
	{
		terminate(GetLastError()); 
		return;
	}

	// create the termination event
	terminateEvent = CreateEvent (0, TRUE, FALSE,
		0);
	if (!terminateEvent)
	{
		terminate(GetLastError());
		return;
	}

	// Notify SCM of progress
	success = SendStatusToSCM(
		SERVICE_START_PENDING,
		NO_ERROR, 0, 2, 1000);
	if (!success)
	{
		terminate(GetLastError()); 
		return;
	}

	// Check for startup params
	//if (argc == 2)
	//{
	//	int temp = atoi(argv[1]);
	//	if (temp < 1000){
	//		// Thwart wise guy who feed in BeepDelay = 0 milliseconds...
	//		BeepDelay = 2000;
	//	}
	//	else{
	//		BeepDelay = temp;
	//	}
	//}

	// Notify SCM of progress
	success = SendStatusToSCM(
		SERVICE_START_PENDING,
		NO_ERROR, 0, 3, 5000);
	if (!success)
	{
		terminate(GetLastError()); 
		return;
	}

	// Start the service itself
	success = InitService();
	if (!success)
	{
		terminate(GetLastError());
		return;
	}

	// The service is now running. 
	// Notify SCM of progress
	success = SendStatusToSCM(
		SERVICE_RUNNING,
		NO_ERROR, 0, 0, 0);
	if (!success)
	{
		terminate(GetLastError()); 
		return;
	}

	// Wait for stop signal, and then terminate
	WaitForSingleObject (terminateEvent, INFINITE);

	terminate(0);
}


//VOID main(VOID)
int main(int argc, char** argv)
{
	bool bNoDaemon = false;

	for(int i=1 ; i < argc; i++ ){
		if( strcmp( argv[i], "--no-daemon" ) == 0 ){
			bNoDaemon = true;
		}
		else if( strcmp( argv[i], "--manual-index-process-url" ) == 0 ){
			bNoDaemon = true;
			if( i+1 < argc ){
				cout << "Setting GB_MANUAL_INDEX_PROCESS_URL equal to argv[++i]..." << endl;
				GB_MANUAL_INDEX_PROCESS_URL = argv[++i];
				cout << "GB_MANUAL_INDEX_PROCESS_URL = \"" <<  GB_MANUAL_INDEX_PROCESS_URL << "\"..." << endl;
			}
		}
		else if( strcmp( argv[i], "--manual-form-process-url" ) == 0 ){
			bNoDaemon = true;
			if( i+1 < argc ){
				cout << "Setting GB_MANUAL_FORM_PROCESS_URL equal to argv[++i]..." << endl;
				GB_MANUAL_FORM_PROCESS_URL = argv[++i];
				cout << "GB_MANUAL_FORM_PROCESS_URL = \"" <<  GB_MANUAL_FORM_PROCESS_URL << "\"..." << endl;
			}
		}
		else if( strcmp( argv[i], "--daily-index-backfill-days" ) == 0 ){
			if( i+1 < argc ){
				cout << "Setting GB_CMD_LINE_DAILY_INDEX_BACKFILL_DAYS equal to argv[++i]..." << endl;
				GB_CMD_LINE_DAILY_INDEX_BACKFILL_DAYS = argv[++i];
				cout << "GB_CMD_LINE_DAILY_INDEX_BACKFILL_DAYS = \"" <<  GB_CMD_LINE_DAILY_INDEX_BACKFILL_DAYS << "\"..." << endl;
			}
		}
		else if( strcmp( argv[i], "--load-daily-indexes" ) == 0 ){
			if( i+1 < argc ){
				cout << "Setting GB_CMD_LINE_LOAD_DAILY_INDEXES equal to argv[++i]..." << endl;
				GB_CMD_LINE_LOAD_DAILY_INDEXES = argv[++i];
				cout << "GB_CMD_LINE_LOAD_DAILY_INDEXES = \"" <<  GB_CMD_LINE_LOAD_DAILY_INDEXES << "\"..." << endl;
			}
		}
		else if( strcmp( argv[i], "--load-next-edgar-filing-header" ) == 0 ){
			if( i+1 < argc ){
				cout << "Setting GB_CMD_LINE_LOAD_NEXT_EDGAR_FILING_HEADER equal to argv[++i]..." << endl;
				GB_CMD_LINE_LOAD_NEXT_EDGAR_FILING_HEADER = argv[++i];
				cout << "GB_CMD_LINE_LOAD_NEXT_EDGAR_FILING_HEADER = \"" <<  GB_CMD_LINE_LOAD_NEXT_EDGAR_FILING_HEADER << "\"..." << endl;
			}
		}
	}

	if( bNoDaemon ){
		cout << "Launching " << SERVICE_NAME << " as a non-daemon process..." << endl;
		DWORD dw_return = ServiceThread( 0 );
		return dw_return;
	}

	SERVICE_TABLE_ENTRY serviceTable[] = 
	{ 
	{ (LPSTR)SERVICE_NAME,
		(LPSERVICE_MAIN_FUNCTION) ServiceMain},
		{ NULL, NULL }
	};
	BOOL success;

	// Register with the SCM
	success = StartServiceCtrlDispatcher(serviceTable);

	if (!success){
		ErrorHandler("In StartServiceCtrlDispatcher", GetLastError());
	}

}/* main() */


DownloadWindowType in_which_download_window(
	int evening_start_hour,
	int evening_end_hour,
	int hour,
	int day_of_week
){
	if( hour == -1 || day_of_week == -1 ){ 

		time_t epoch_seconds; 
		time( &epoch_seconds );

		struct tm t_struct;
		JDA::Utils::nyctime( &t_struct, epoch_seconds );

		//timestamp = time();

		if( hour == -1 ){
			//hour = date('G', $timestamp);
			hour = t_struct.tm_hour; /* hours since midnight: 0-23 */
		}

		if( day_of_week == -1 ){
			//day_of_week = date('w', $timestamp);
			day_of_week = t_struct.tm_wday; /* days since Sunday: 0-6 */
		}
	}

	// day_of_week = 0 : sunday
	// day_of_week = 1 : monday
	// day_of_week = 2 : tuesday
	// day_of_week = 3 : wednesday 
	// day_of_week = 4 : thursday
	// day_of_week = 5 : friday
	// day_of_week = 6 : saturday

	// On weekends, we're always in the weekend download zone, Baby...
	if(
		6 == day_of_week /* saturday */
			||
		0 == day_of_week /* sunday */
	){
		return WEEKEND;
	}

	if(
		hour >= evening_start_hour
			||
		hour < evening_end_hour
	){
		return EVENING;
	}
	else
	{
		return DAY;
	}

}/* bool in_which_download_window() */

void in_which_download_window_test(int evening_start_hour, int evening_end_hour){

	const char* sWho = "::in_which_download_window_test";

	for( int day_of_week = 0; day_of_week <= 6; day_of_week++ ){
		for( int hour = 0; hour <= 23; hour++ ){

			DownloadWindowType downloadWindowType = in_which_download_window( evening_start_hour, evening_end_hour, hour, day_of_week );

			JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): " 
				<< "in_which_download_window( " << "hour = " << hour << ", day_of_week = " << day_of_week << ") = "  
				<< downloadWindowType << " = " << downloadWindowTypeToString( downloadWindowType ) 
				<< "..." << endl;
		}
	}

}/* in_which_download_window_test() */


void getConfig() {

	string sWho = "::getConfig";

	string sExecutablePath = JDA::Utils::getExecutablePath(); 
	string sConfigFilePath = JDA::Utils::getDefaultConfigFilePath( sExecutablePath );

	/* Set to defaults first, then they can be overridden 
	* by settings in *.ini file, then they are processed
	* in setupLogger()...
	*/
	ConfigMap["debug_level"] = JDA::FormsMeanCommon::DEFAULT_DEBUG_LEVEL;
	ConfigMap["debug_log_file_path"] = JDA::FormsMeanCommon::DEFAULT_DEBUG_LOG_FILE_PATH;
	ConfigMap["debug_log_file_on"] = JDA::Utils::boolToString( JDA::FormsMeanCommon::DEFAULT_DEBUG_LOG_FILE_ON );
	ConfigMap["debug_log_file_append"] = JDA::Utils::boolToString( JDA::FormsMeanCommon::DEFAULT_DEBUG_LOG_FILE_APPEND ); 
	ConfigMap["debug_stdout_on"] = JDA::Utils::boolToString( JDA::FormsMeanCommon::DEFAULT_DEBUG_STDOUT_ON );

	ConfigMap["load_daily_indexes"] = JDA::Utils::boolToString( JDA::FormsMeanCommon::DEFAULT_LOAD_DAILY_INDEXES );
	ConfigMap["load_next_edgar_filing_header"] = JDA::Utils::boolToString( JDA::FormsMeanCommon::DEFAULT_LOAD_NEXT_EDGAR_FILING_HEADER );

	cout << sWho << "(): sExecutablePath = '" << sExecutablePath << "'..." << endl;
	cout << sWho << "(): sConfigFilePath = '" << sExecutablePath << "'..." << endl;
	cout << sWho << "(): SHEMP: Moe, reading config file now, Moe..." << endl;

	try {

		JDA::Utils::read_config_file( sConfigFilePath, ConfigMap );

	}
	catch(JDA::Utils::Exception& e) {

		cerr << sWho << "(): Cannot read config file '" << sConfigFilePath 
			<< "': \"" << e.what() << "\", using defaults..." << endl;
	
	}

	cout << sWho << "(): After reading config file, ConfigMap = " << ConfigMap << "..." << endl;
	
}/* void getConfig() */

/**
* If Config["debug_log_file_path"] is not supplied...
* 
* If executable is found, at 
*  c:/vf/bin/joe.exe
* returns... 
*  c:/vf/bin/joe-<YYYY-MM-DD>.log
* e.g.,...
*  c:/vf/bin/joe-2014-05-09.log
*
* Otherwise, returns Config["debug_log_file_path"]
* with "YYYY-MM-DD" replaced by the current
* datestamp.
* e.g., if Config["debug_log_file_path"] = "c:/vf/logs/joe-YYYY-MM-DD.log",
*  returns something like
*    "c:/vf/logs/joe-2014-05-09.log",
*/
string getLogFilePath(){

	string sNycYYYYMMDD = JDA::Utils::get_nyc_datestamp();
	string sLogFilePath = "";

	if( ConfigMap["debug_log_file_path"].length() == 0 ){
		string sSuffix = "-" + sNycYYYYMMDD + ".log";
		string sExecutablePath = JDA::Utils::getExecutablePath(); 
		sLogFilePath = JDA::Utils::getDefaultLogFilePath( sExecutablePath, sSuffix ); 
	}
	else {
		sLogFilePath = ConfigMap["debug_log_file_path"];
		string sToken = "YYYY-MM-DD";
		size_t where = sLogFilePath.find( sToken );
		if( where != std::string::npos ){
			sLogFilePath.replace( where, sToken.length(), sNycYYYYMMDD );
		}
	}

	return sLogFilePath;
}/* getLogFilePath() */

string getLogFilePath( const string s_log_file_path ){

	string sNycYYYYMMDD = JDA::Utils::get_nyc_datestamp();
	string sLogFilePath = "";

	if( s_log_file_path.length() == 0 ){
		string sSuffix = "-" + sNycYYYYMMDD + ".log";
		string sExecutablePath = JDA::Utils::getExecutablePath(); 
		sLogFilePath = JDA::Utils::getDefaultLogFilePath( sExecutablePath, sSuffix ); 
	}
	else {
		sLogFilePath = s_log_file_path;
		string sToken = "YYYY-MM-DD";
		size_t where = sLogFilePath.find( sToken );
		if( where != std::string::npos ){
			sLogFilePath.replace( where, sToken.length(), sNycYYYYMMDD );
		}
	}

	return sLogFilePath;

}/* getLogFilePath() */

void setupLogger( JDA::Logger& kenny_loggins ){

	const char* sWho = "::setupLogger";

	//JDA::Logger::log.setDebugLevel( JDA::FormsMeanCommon::DEFAULT_DEBUG_LEVEL );
	kenny_loggins.setDebugLevel( JDA::FormsMeanCommon::DEFAULT_DEBUG_LEVEL );

	JDA::Logger::DebugLevelType debugLevel = JDA::Logger::LevelNameToDebugLevel( ConfigMap["debug_level"] );

	if( debugLevel != JDA::Logger::NOTSET ){
		cout << sWho << "(): Setting debugLevel to " << JDA::Logger::getLevelName( debugLevel ) << "..." << endl;
		kenny_loggins.setDebugLevel( debugLevel );
	}

	string sLogFilePath = getLogFilePath();
	cout << sWho << "(): Setting logFilePath to '" << sLogFilePath << "'..." << endl;
	kenny_loggins.setLogFilePath( sLogFilePath );

	bool bLogFileOn = JDA::Utils::stringToBool( ConfigMap["debug_log_file_on"] );
	cout << sWho << "(): Setting logFileOn to " << JDA::Utils::boolToString( bLogFileOn ) << "..." << endl;
	kenny_loggins.setLogFileOn( bLogFileOn );
	
	bool bLogFileAppend = JDA::Utils::stringToBool( ConfigMap["debug_log_file_append"] );
	cout << sWho << "(): Setting logFileAppend to " << JDA::Utils::boolToString( bLogFileAppend ) << "..." << endl;
	kenny_loggins.setLogFileAppend( bLogFileAppend );

	bool bLogStdoutOn = JDA::Utils::stringToBool( ConfigMap["debug_stdout_on"] );
	cout << sWho << "(): Setting logStdoutOn to " << JDA::Utils::boolToString( bLogStdoutOn ) << "..." << endl;
	kenny_loggins.setStdoutOn( bLogStdoutOn );

	cout << sWho << "(): Done with " << sWho << "()..." << endl;

}/* setupLogger() */

void setupLogger(
	JDA::Logger& kenny_loggins,
	const string& s_debug_level, const string& s_debug_log_file_path,
	const string& s_debug_log_file_on, const string& s_debug_log_file_append, const string& s_debug_stdout_on
){

	const char* sWho = "::setupLogger";

	//JDA::Logger::log.setDebugLevel( JDA::FormsMeanCommon::DEFAULT_DEBUG_LEVEL );
	kenny_loggins.setDebugLevel( JDA::FormsMeanCommon::DEFAULT_DEBUG_LEVEL );

	JDA::Logger::DebugLevelType debugLevel = JDA::Logger::LevelNameToDebugLevel( ConfigMap["debug_level"] );

	if( debugLevel != JDA::Logger::NOTSET ){
		cout << sWho << "(): Setting debugLevel to " << JDA::Logger::getLevelName( debugLevel ) << "..." << endl;
		kenny_loggins.setDebugLevel( debugLevel );
	}

	string sLogFilePath = getLogFilePath( s_debug_log_file_path );
	cout << sWho << "(): Setting logFilePath to '" << sLogFilePath << "'..." << endl;
	kenny_loggins.setLogFilePath( sLogFilePath );

	bool bLogFileOn = JDA::Utils::stringToBool( s_debug_log_file_on );
	cout << sWho << "(): Setting logFileOn to " << JDA::Utils::boolToString( bLogFileOn ) << "..." << endl;
	kenny_loggins.setLogFileOn( bLogFileOn );
	
	bool bLogFileAppend = JDA::Utils::stringToBool( s_debug_log_file_append );
	cout << sWho << "(): Setting logFileAppend to " << JDA::Utils::boolToString( bLogFileAppend ) << "..." << endl;
	kenny_loggins.setLogFileAppend( bLogFileAppend );

	bool bLogStdoutOn = JDA::Utils::stringToBool( s_debug_stdout_on );
	cout << sWho << "(): Setting logStdoutOn to " << JDA::Utils::boolToString( bLogStdoutOn ) << "..." << endl;
	kenny_loggins.setStdoutOn( bLogStdoutOn );

	cout << sWho << "(): Done with " << sWho << "()..." << endl;

}/* setupLogger() */


void resetLogFilePath(){

	string sWho = (string)SERVICE_NAME + "::resetLogFilePath";

	string sLogFilePath = getLogFilePath();
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): Setting logFilePath to '" << sLogFilePath << "'..." << endl;
	JDA::Logger::log.setLogFilePath( sLogFilePath );
}/* resetLogFilePath() */

void resetLogFilePath(
	JDA::Logger& kenny_loggins, const string& s_debug_log_file_path
){

	string sWho = (string)SERVICE_NAME + "::resetLogFilePath";

	string sLogFilePath = getLogFilePath( s_debug_log_file_path );
	kenny_loggins(JDA::Logger::INFO) << sWho << "(): Setting logFilePath to '" << sLogFilePath << "'..." << endl;
	kenny_loggins.setLogFilePath( sLogFilePath );

}/* resetLogFilePath() */

