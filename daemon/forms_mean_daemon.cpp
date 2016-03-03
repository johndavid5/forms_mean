#include <string>
#include <iostream>
#include <ctime>
#include <time.h>
#include "windows.h" // Sleep(), etc. (NOTE: In Windows it's Sleep(), in UNIX it's sleep()...) 

#include "utils.h" 
#include "logger.h" 

#include "FormsMeanConfig.h"

#include "FormsMeanUtils.h"

using namespace std;

/************ GLOBAL FUNCTION PROTOTYPES - BEGIN ******************/

void in_which_download_window_test(int evening_start_hour, int evening_end_hour);

/************ GLOBAL FUNCTION PROTOTYPES - END ******************/

/********** GLOBAL VARIABLES - BEGIN ******************/

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

JDA::Logger* P_LOGGER = NULL;

/********** GLOBAL VARIABLES - END ******************/

void ErrorHandler(char *s, DWORD err)
{
	const char* sWho = "ErrorHandler";

	ostringstream oss_out;

	oss_out << s << endl;
	oss_out << "Error number: " << err << endl;
	oss_out << "Error string: " << JDA::Utils::strerror(err) << endl;
	oss_out << "Calling ExitProcess(" << err << ")" << endl;

	if( P_LOGGER != NULL ){
		(*P_LOGGER)(JDA::Logger::FATAL) << sWho << "():\n" << oss_out.str() << endl;
	}
	else {
		cerr << "FATAL: " << sWho << "():\n" << oss_out.str() << endl;
	}

	ExitProcess(err);

}/* ErrorHandler() */


DWORD ServiceThread(LPDWORD param)
{
	cout.sync_with_stdio(true); // Get C and C++ I/O to play together nicely...

	string sWho = (string)SERVICE_NAME + "::ServiceThread";

	string sConfigFilePath = JDA::FormsMeanUtils::getConfigFilePath();

	FormsMeanConfig config;
	config.loadConfigFile( sConfigFilePath );

	JDA::Logger logger;
	P_LOGGER = &logger; // Set to global for ErrorHandler...

	FormsMeanUtils::setupLogger( &logger );

	logger(JDA::Logger::INFO) << sWho << "(): " << "\n" 
	<< "*************************************************" << "\n"
	<< "**  Welcome to The Forms Mean Daemon, Pilgrim  **" << "\n"
	<< "*************************************************" << endl;

	logger(JDA::Logger::INFO) << sWho << "(): " << "S_VERSION = " << S_VERSION << "..." << endl;

	logger(JDA::Logger::INFO) << sWho << "(): config = " << config << "..." << endl;

	return 0;

}/* ServiceThread() */


// Initializes the service by starting its thread
BOOL InitService()
{
	const string sWho = "InitService";

	if( P_LOGGER != NULL ){
		(*P_LOGGER)(JDA::Logger::INFO) << sWho << "(): Como esta?..." << endl;
	}
	else{
		cout << "INFO: " << sWho << "(): Como esta?..." << endl;
	}

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

	if( P_LOGGER != NULL ){
		(*P_LOGGER)(JDA::Logger::INFO) << sWho << "(): " << "Welcome back, John...so glad you could make it..." << endl;
	}
	else{
		cout << "INFO: " << sWho << "(): " << "Welcome back, John...so glad you could make it..." << endl;
	}

	pauseService=FALSE;
	ResumeThread(threadHandle);
}

// Pauses the service
VOID PauseService()
{
	const string sWho = "PauseService";

	if( P_LOGGER != NULL ){
		(*P_LOGGER)(JDA::Logger::INFO) << sWho << "(): " << "\"I'll be back, Bennett...!\"" << endl;
	}
	else{
		cout << "INFO: " << sWho << "(): " << "\"I'll be back, Bennett...!\"" << endl;
	}

	pauseService = TRUE;
	SuspendThread(threadHandle);
}

// Stops the service by allowing ServiceMain to
// complete
VOID StopService() 
{
	const string sWho = "StopService";

	if( P_LOGGER != NULL ){
		(*P_LOGGER)(JDA::Logger::INFO) << sWho << "(): " << "\"Let off some steam, Bennett!\"" << endl;
	}
	else{
		cout << "INFO: " << sWho << "(): " << "\"Let off some steam, Bennett!\"" << endl;
	}

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

