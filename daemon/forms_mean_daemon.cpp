#include <string>
#include <iostream>
#include <ctime>
#include <time.h>
#include "windows.h" // Sleep(), etc. (NOTE: In Windows it's Sleep(), in UNIX it's sleep()...) 

#include "utils.h" 
#include "logger.h" 

//#include "FormsMeanConfig.h"
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

JDA::Logger G_LOGGER; // Need a global Logger for the global Service methods...

/********** GLOBAL VARIABLES - END ******************/

void ErrorHandler(char *s, DWORD err)
{
	const char* sWho = "ErrorHandler";

	ostringstream oss_out;

	oss_out << s << endl;
	oss_out << "Error number: " << err << endl;
	oss_out << "Error string: " << JDA::Utils::strerror(err) << endl;
	oss_out << "Calling ExitProcess(" << err << ")" << endl;

	G_LOGGER(JDA::Logger::FATAL) << sWho << "():\n" << oss_out.str() << endl;

	ExitProcess(err);

}/* ErrorHandler() */

/* Feed a pointer to OurParams to ServiceThread if launched as a non-daemon... */
struct OurParams {

	string s_manual_index_process_url;
	string s_manual_form_process_url;
	string s_daily_index_backfill_days;
	string s_load_daily_indexes;
	string s_load_next_edgar_filing_header;
	
};

ostream& operator<<(ostream& s, OurParams& ourParams){

	s << "s_manual_index_process_url = \"" << ourParams.s_manual_index_process_url << "\"\n" 
	<< "s_manual_form_process_url = \"" <<  ourParams.s_manual_form_process_url << "\"\n"
	<< "s_daily_index_backfill_days = \"" <<  ourParams.s_daily_index_backfill_days << "\"\n"
	<< "s_load_daily_indexes = \"" <<  ourParams.s_load_daily_indexes << "\"\n"
	<< "s_load_next_edgar_filing_header = \"" <<  ourParams.s_load_next_edgar_filing_header << "\"";

	return s;
}


DWORD ServiceThread(LPDWORD param)
{
	cout.sync_with_stdio(true); // Get C and C++ I/O to play together nicely...

	string sWho = (string)SERVICE_NAME + "::ServiceThread";

	//string sConfigFilePath = JDA::FormsMeanUtils::getConfigFilePath();

	//FormsMeanConfig config;
	//config.loadConfigFile( sConfigFilePath );

	JDA::Logger* p_logger = &G_LOGGER; /* Use global JDA::Logger... */

	//FormsMeanUtils::setupLogger( p_logger );

	(*p_logger)(JDA::Logger::INFO) << sWho << "(): " << "\n" 
	<< "*************************************************" << "\n"
	<< "**  Welcome to The Forms Mean Daemon, Pilgrim  **" << "\n"
	<< "*************************************************" << endl;

	(*p_logger)(JDA::Logger::INFO) << sWho << "(): " << "S_VERSION = " << S_VERSION << "..." << endl;

	if( param != NULL ){
		OurParams* p_our_params = (OurParams*) param;	
		(*p_logger)(JDA::Logger::INFO) << sWho << "(): " << "Got OurParams: " << (*p_our_params) << endl;
	}

	//(*p_logger)(JDA::Logger::INFO) << sWho << "(): config = " << config << "..." << endl;
	
	(*p_logger)(JDA::Logger::INFO) << sWho << "(): " << "Exiting daemon now..." << endl;

	return 0;

}/* ServiceThread() */


// Initializes the service by starting its thread
BOOL InitService()
{
	const string sWho = "InitService";

	G_LOGGER(JDA::Logger::INFO) << sWho << "()..." << endl;

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

	G_LOGGER(JDA::Logger::INFO) << sWho << "()..." << endl;

	pauseService=FALSE;
	ResumeThread(threadHandle);
}

// Pauses the service
VOID PauseService()
{
	const string sWho = "PauseService";

	G_LOGGER(JDA::Logger::INFO) << sWho << "()..." << endl;

	pauseService = TRUE;
	SuspendThread(threadHandle);
}

// Stops the service by allowing ServiceMain to
// complete
VOID StopService() 
{
	const string sWho = "StopService";

	G_LOGGER(JDA::Logger::INFO) << sWho << "()..." << endl;

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

	G_LOGGER(JDA::Logger::INFO) << sWho << "()...\"" << endl;

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

	G_LOGGER(JDA::Logger::INFO) << sWho << "(): return code from SetServiceStatus() = " << success << "..." << endl;

	if (!success) {

		G_LOGGER(JDA::Logger::WARN) << sWho << "(): return code from SetServiceStatus() = " << success << " is false: error = \"" << JDA::Utils::s_error() << "\", but NOT calling StopService()...it seems to cause a crash when I do..." << endl;

		//G_LOGGER(JDA::Logger::ERROR) << sWho << "(): return code from SetServiceStatus() = " << success << " is false: error = \"" << JDA::Utils::s_error() << "\", so calling StopService()..." << endl;
		//StopService();
	}

	return success;
}

// Dispatches events received from the service 
// control manager
VOID ServiceCtrlHandler (DWORD controlCode) 
{
	const string sWho = "ServiceCtrlHandler";

	G_LOGGER(JDA::Logger::INFO) << sWho << "( controlCode = " << controlCode << ")..." << endl;

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

	G_LOGGER(JDA::Logger::INFO) << sWho << "(): begin..." << endl;

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
	G_LOGGER(JDA::Logger::INFO) << sWho << "(): end..." << endl;
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

	OurParams ourParams;

	for(int i=1 ; i < argc; i++ ){
		if( strcmp( argv[i], "--no-daemon" ) == 0 ){
			bNoDaemon = true;
		}
		else if( strcmp( argv[i], "--manual-index-process-url" ) == 0 ){
			bNoDaemon = true;
			if( i+1 < argc ){
				cout << "Setting ourParams.s_manual_index_process_url equal to argv[++i]..." << endl;
				ourParams.s_manual_index_process_url = argv[++i];
			}
		}
		else if( strcmp( argv[i], "--manual-form-process-url" ) == 0 ){
			bNoDaemon = true;
			if( i+1 < argc ){
				cout << "Setting ourParams.s_manual_form_process_url equal to argv[++i]..." << endl;
				ourParams.s_manual_form_process_url = argv[++i];
			}
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

	if( bNoDaemon ){
		cout << "Launching " << SERVICE_NAME << " as a non-daemon process..." << endl;
		DWORD dw_return = ServiceThread( (LPDWORD)&ourParams );
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


