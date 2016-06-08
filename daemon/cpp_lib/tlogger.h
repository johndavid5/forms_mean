#ifndef _T_LOGGER_H_
#define _T_LOGGER_H_

#ifdef WIN32
	#include <io.h>
#endif

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

#ifdef _VC_DEBUG_
	#include "windows.h"
	// For Visual C++ Debug Window -- This is a macro only defined if _DEBUG_ is defined...
	//OutputDebugString( TEXT( le_oss.str().c_str() ) );
#endif

#include "utils.h"

/** 
* NOTE: ERROR and DEBUG are defined as macro's in Windows and are clashing
* with JDA::Logger::DebugLevelType = ERROR and DEBUG, so undef 'em...
*/
#undef ERROR 
#undef DEBUG

#ifdef _T_LOGGER_WIDE_ 
  //#pragma message("tlogger.h: _T_LOGGER_WIDE_ is set...")
#endif

#ifdef _T_LOGGER_NARROW_ 
  //#pragma message("tlogger.h: _T_LOGGER_NARROW_ is set...")
#endif

#if ! defined _T_LOGGER_NARROW_  && ! defined _T_LOGGER_WIDE_ 
  #error You must '#define' either _T_LOGGER_NARROW_ or _T_LOGGER_WIDE_
#endif

#if defined _T_LOGGER_NARROW_  &&  defined _T_LOGGER_WIDE_ 
  #error You must '#define' either _T_LOGGER_NARROW_ or _T_LOGGER_WIDE_ ... not BOTH!
#endif

#ifdef _T_LOGGER_NARROW_
  //#pragma message("tlogger.h: Using narrow shtuff...")
  #define CHAR char
  #define _TT(x) x
  #define STRING string
  #define OSTRINGSTREAM ostringstream
  #define SPRINTF_S sprintf_s
  #define FPUTS fputs
  #define _VSCPRINTF _vscprintf
  #define VSPRINTF_S vsprintf_s
  #define OSTREAM ostream
  #define FOPEN fopen 
  //#define FOPEN_S fopen_s // Files opened with fopen_s are not shareable...

  #ifdef WIN32
	  #define FSOPEN _fsopen // Use _fsopen to specify shareability of file...
  #endif

  #define COUT cout
  #define FPRINTF fprintf
#elif defined _T_LOGGER_WIDE_
  #pragma message("tlogger.h: Using wide shtuff...")
  #define CHAR wchar_t
  #define _TT(x) L ##x
  #define STRING wstring
  #define OSTRINGSTREAM wostringstream
  #define SPRINTF_S swprintf_s
  #define FPUTS fputws
  #define _VSCPRINTF _vscwprintf
  #define VSPRINTF_S vswprintf_s
  #define OSTREAM wostream
  //#define FOPEN _wfopen 
  //#define FOPEN_S _wfopen_s // Files opened with _wfopen_s are not shareable...
  #define FSOPEN _wfsopen // Use _wfsopen to specify shareability of file...
  #define COUT wcout
  #define FPRINTF fwprintf
#endif


namespace JDA
{

/**
* A simple logger that uses both "static" methods and "stream" type non-static methods...
*
* Feel free to use C++ style streams as the underlying stream type, thus making it possible
* to output non-null-char-terminated strings...
*/
class Logger {

		public:

		enum DebugLevelType { 
			EMERG = -100,
			FATAL = 0,
			ALERT = 100,
			CRIT = 200, 
			ERROR = 300, ERR = 300,
			WARN = 400,
			NOTICE = 500,
			INFO = 600, 
			DEBUG = 700, DBG = 700,
			VDEBUG = 800, TRACE = 800, // Verbose Debug
			NOTSET = 900 
		}; // Corresponding to syslog(3)

		int debug; /* Turn on internal debugging... */

	/** Interface if you wish to subscribe
	* to logger events...
	*/
	class ILogWriter
	{
		public:
			/* You get to implement logIt() yourself in a subclass... */
			virtual void logIt(const STRING& message) const = 0;
	
			/* Virtual destructor to avoid any future memory leaks... */
			virtual ~ILogWriter(){}
	};

	protected:
		// For the "streams" interface...
		OSTRINGSTREAM oss;

		//string sTimestamp;
		DebugLevelType debugLevel;  
		DebugLevelType messageLevel;  

	protected:

		bool stdoutOn;

		bool logFileOn;

		STRING logFilePath;

		bool logFileAppend;

		FILE* logFileHandle;

		/* To keep track of lazy initialization... */
		/* bool logFileSetup; */
		STRING logFilePathOpened; 

		vector<ILogWriter*> logWriters; /* Keep track of subscribers... */

	public:

		/* static JDA::Logger& log; */
								/* Static member initialized to a log object...so you can do C++ streams stuff 
								* as
								*   JDA::Logger::log(JDA::Logger::INFO) << "Here is my info" << endl;
								* without having to initialize your own logger object...
								*   JDA::Logger myLog;
								*   myLog(JDA::Logger::INFO) << "Here is my info" << endl;
								* I'm not so sure it's such a great idea to use this, though...
								*/

	public:

		Logger() : messageLevel( INFO ), debugLevel( INFO ), stdoutOn( true ),
				logFileOn( false ), logFilePath( _TT("") ), logFileAppend( false ),
				logFileHandle( NULL ), logFilePathOpened( _TT("") ), debug( 0 )
		{}

		~Logger(){
			this->logFileClose();
		}

		void inline setDebugLevel( DebugLevelType debugLevel) {
			this->debugLevel = debugLevel;
		}

		DebugLevelType inline getDebugLevel( ) {
			return this->debugLevel;
		}

		STRING getDebugLevelName()
		{
			return getLevelName( this->debugLevel );
		}

	static STRING& getTimestamp( STRING& s_out, time_t epoch_time = -1 ){

		string sStamp = JDA::Utils::get_nyc_timestamp( epoch_time );

		#ifdef _T_LOGGER_NARROW_
			s_out = sStamp;
		#elif defined _T_LOGGER_WIDE_
			s_out = JDA::Utils:narrowToWideS( sStamp ); 
		#endif

		return s_out;
	}

	//static STRING& Logger::getTimestamp( STRING& s_out, time_t epoch_time = -1 ){
	//
	//	if( epoch_time == -1 ) {
	//		time( &epoch_time );
	//	}
	//
	//	struct tm newtime;
	//	localtime_s( &newtime, &epoch_time );
	//
	//	const int buf_sz = 256;
	//	CHAR buffy[buf_sz];
	//	
	//	SPRINTF_S(buffy, buf_sz, _TT("%04d-%02d-%02d %02d:%02d:%02d"), 
	//   	    1900 + newtime.tm_year,
	//		newtime.tm_mon + 1,
	//		newtime.tm_mday,
	//		newtime.tm_hour,
	//		newtime.tm_min,
	//		newtime.tm_sec
	//	);
	//
	//    s_out = buffy;
	//	return s_out;
	//
	//}/* getTimestamp() */
	
	void inline setStdoutOn( bool stdoutOn ) {
		this->stdoutOn = stdoutOn;
	}
	
	bool inline getStdoutOn( ) {
		return this->stdoutOn; 
	}
	
	void inline setLogFilePath ( const STRING& logFilePath) {
		this->logFilePath = logFilePath;
	}
	
	STRING inline getLogFilePath ( ) {
		return this->logFilePath;
	}
	
	void inline setLogFileOn ( bool logFileOn ){
		this->logFileOn = logFileOn;
	}
	
	bool inline getLogFileOn ( ) {
		return this->logFileOn;
	}
	
	void inline setLogFileAppend( bool logFileAppend ) {
		this->logFileAppend = logFileAppend; 
	}
	
	bool inline getLogFileAppend() {
		return this->logFileAppend;
	}
	
	void inline addLogWriter( JDA::Logger::ILogWriter* pLogWriter) {
		this->logWriters.push_back( pLogWriter ); 	
	}
	
	bool inline willPrint (DebugLevelType messageLevel ){
	
		if( messageLevel <= this->debugLevel ) {
			return true;
		}
		else {
			return false;
		}
	}

	/**
	* Performs "lazy initialization" of logFileHandle...meaning it
	* will only initialize the logFileHandle once (plus every 
	* time it detects a change in this->logFilePath )...
	* so you can call it every time you're going to write to the logFileHandle...
	* ...or, set force=true to force a re-init...
	*/
	void fileHandleSetup( bool force = false ) {
	
		STRING sWho=_TT("Logger::fileHandleSetup");
	
		// REFERENCE: http://msdn.microsoft.com/en-us/library/z0kc8e3z.aspx
		// Compile with /W3
	
		if( 
			this->logFileOn 
			&& ( force == true || this->logFilePathOpened.compare(this->logFilePath) != 0 )
		) {

			logFileClose(); // Close this->logFileHandle if it is open...
	
			STRING mode_flags;
			STRING mode_gerund;
	
			if ( this->logFileAppend ) {
				mode_flags = _TT("a");
				mode_gerund = _TT("appending");
			} else { 
				mode_flags = _TT("w");
				mode_gerund = _TT("writing");
			}
	
			#if defined(FSOPEN)
				// Use FSOPEN to allow sharing of file...
				if( debug ) {
					COUT << sWho << _TT("(): Opening log file '") << this->logFilePath << _TT("' for ") << mode_gerund << _TT(" with shared access...") << endl;
				}

				// Using _SH_DENYNO to allow both read and write access to the logfile...
				this->logFileHandle = FSOPEN( this->logFilePath.c_str(), mode_flags.c_str(), _SH_DENYNO );
		
				if( this->logFileHandle == NULL ) { 
					const int err_buf_sz = 256;
					char err_buf[err_buf_sz];
					strerror_s(err_buf, err_buf_sz, errno);
	
					FPRINTF(stderr, _TT("Trouble opening log file '%s' for shared %s: %d: '%s'\n"),
						this->logFilePath.c_str(), mode_gerund.c_str(), errno,  err_buf );
	
					this->logFileOn = false;
				}
				else {
					if( debug ){
						COUT << sWho << _TT("(): File open of log file '") << this->logFilePath << _TT("' for ") << mode_gerund << _TT(" with shared access successful, logFileHandle = ") << this->logFileHandle << endl;
					}
					this->logFilePathOpened = this->logFilePath; // This will let us know we've opened it...
				}
			#elif defined(FOPEN)
				if( debug ) {
					COUT << sWho << _TT("(): Opening log file '") << this->logFilePath << _TT("' for ") << mode_gerund << _TT("...") << endl;
				}

				this->logFileHandle = FOPEN( this->logFilePath.c_str(), mode_flags.c_str() );
		
				if( this->logFileHandle == NULL ) { 

					//const int err_buf_sz = 256;
					//char err_buf[err_buf_sz];
					//strerror_s(err_buf, err_buf_sz, errno);
	
					//FPRINTF(stderr, _TT("Trouble opening log file '%s' for %s: %d: '%s'\n"),
					//	this->logFilePath.c_str(), mode_gerund.c_str(), errno,  err_buf );

					FPRINTF(stderr, _TT("Trouble opening log file '%s' for %s: %d: '%s'\n"),
						this->logFilePath.c_str(), mode_gerund.c_str(), errno,  JDA::Utils::strerror(errno).c_str());
	
					this->logFileOn = false;
				}
				else {
					if( debug ) {
						COUT << sWho << _TT("(): File open of log file '") << this->logFilePath << _TT("' for ") << mode_gerund << _TT(" successful, logFileHandle = ") << this->logFileHandle << endl;
					}
					this->logFilePathOpened = this->logFilePath; // This will let us know we've opened it...
				}
			#elif defined(FOPEN_S)
				if( debug ) {
					COUT << sWho << _TT("(): Opening log file '") << this->logFilePath << _TT("' for ") << mode_gerund << _TT("...") << endl;
				}

				errno_t iReturn = FOPEN_S(&(this->logFileHandle), this->logFilePath.c_str(), mode_flags.c_str());
		
				if ( iReturn != 0 ) {
					const int err_buf_sz = 256;
					char err_buf[err_buf_sz];
					strerror_s(err_buf, err_buf_sz, errno);
	
					FPRINTF(stderr, _TT("Trouble opening log file '%s' for %s: %d: '%s'\n"),
						this->logFilePath.c_str(), mode_gerund.c_str(), errno,  err_buf );
	
					this->logFileOn = false;
				}
				else {
					if( debug ) {
						COUT << sWho << _TT("(): File open of log file '") << this->logFilePath << _TT("' for ") << mode_gerund << _TT(" successful, logFileHandle = ") << this->logFileHandle << endl;
					}
					this->logFilePathOpened = this->logFilePath; // This will let us know we've opened it...
				}
			#endif
	
			//this->logFileSetup = 1;
		}
	
	}/* debugFileHandleSetup() */
	
	void logFileClose() {

		std::string sWho="Logger::logFileClose";

		//if( this->logFileOn &&  this->logFileSetup && this->logFileHandle ) {
		if( this->logFileHandle ) {

			if( debug ) {
				COUT << sWho << _TT("(): Closing log fileHandle ") << this->logFileHandle << _TT(" for '") << this->logFilePathOpened << _TT("'...") << endl;
			}

			fclose( this->logFileHandle );

			this->logFileHandle = NULL;
			this->logFilePathOpened = "";
		}
	}

protected:
	/*
	* The bytes stop here.  This is the base-level printing method...all roads should lead to this method...
	*/
	void printIt(  DebugLevelType messageDebugLevel, const STRING& message  ){

		std::string sWho="JDA::Logger::printIt";
	
		//fprintf(stdout, "%s(): messageDebugLevel=%d, message='%s'...\n", sWho.c_str(), messageDebugLevel, message.c_str());
		//fflush(stdout);
	
		fileHandleSetup(); // lazy initialization

		STRING sTimestamp;
		Logger::getTimestamp( sTimestamp );

		OSTRINGSTREAM le_oss;

		le_oss << "[" << sTimestamp << "] " << getLevelName( messageDebugLevel ) << ": " << message;

		if( this->stdoutOn ) {
			//fprintf( stdout, "%s", message.c_str() );
			//fputs( le_oss.str().c_str(), stdout );
			//fputws( le_oss.str().c_str(), stdout );
			FPUTS( le_oss.str().c_str(), stdout );
			fflush( stdout );
		}
	
		if( this->logFileOn && this->logFileHandle ) {
			//fprintf( GBDebugLogFileHandle,  message.c_str() );
			//fputs( le_oss.str().c_str(), this->logFileHandle );
			//fputws( le_oss.str().c_str(), this->logFileHandle );
			FPUTS( le_oss.str().c_str(), this->logFileHandle );
			fflush( this->logFileHandle );
		}
	
		#ifdef _VC_DEBUG_
		// For Visual C++ Debug Window -- This is a macro only defined if _DEBUG_ is defined...
		OutputDebugString( TEXT( le_oss.str().c_str() ) );
		#endif
	
		for(size_t i=0; i < this->logWriters.size(); i++ ){
			this->logWriters[i]->logIt( le_oss.str() );	
		}

	}/* printIt() */

public:

	void print( DebugLevelType messageDebugLevel, const STRING& message  ){
	
		std::string sWho="JDA::Logger::print";
	
		//fprintf(stdout, "%s(): messageDebugLevel=%d, message='%s'...\n", sWho.c_str(), messageDebugLevel, message.c_str());
		//fflush(stdout);
		//debugFileHandleSetup(); // lazy initialization
	
		if( willPrint( messageDebugLevel ) ) {

			printIt( messageDebugLevel, message ); 
	
		}/* if( willPrint( messageDebugLevel ) )  */
	
	}/* static void print( int messageDebugLevel, const string& message  ) */
	

	#ifdef WIN32
	void printf( DebugLevelType messageDebugLevel, const CHAR* format, ... ){
	
		// REFERENCE: http://msdn.microsoft.com/en-us/library/28d5ce15.aspx
	
		if( willPrint( messageDebugLevel ) ) {
	
			va_list args;
			va_start( args, format );
	
			// _vscprintf() will figure out how big a buffer we need,
			// but it doesn't count terminating '\0', so add 1 to len...
			//int len = _vscprintf( format, args ) + 1;
			//int len = _vscwprintf( format, args ) + 1;
			int len = _VSCPRINTF( format, args ) + 1;

			// Allocate a temporary buffer for the vsprintf()...
			//char* buffer = (char*) malloc( len * sizeof(char) );
			//wchar_t* buffer = (wchar_t*) malloc( len * sizeof(wchar_t) );
			CHAR* buffer = (CHAR*) malloc( len * sizeof(CHAR) );
	
			//vsprintf( buffer, format, args ); // C4996
			//vsprintf_s( buffer, len, format, args ); // C4996
			//vswprintf_s( buffer, len, format, args ); // C4996
			VSPRINTF_S( buffer, len, format, args ); // C4996
	
			printIt( messageDebugLevel, buffer );

			// Free the temporary buffer... 
			free ( buffer );
		}
	}/* printf() */
	#endif

	template <typename T> Logger& operator<<(T a) {
	    oss << a;
	    return *this;
	}

	#if defined(_T_LOGGER_WIDE_)
  		#pragma message("Including 	Logger& operator<<(string& a) shtuff...")
	/* Explicitly convert to wide if a narrow string is passed in...
	* this should be safer than converting a wide string to narrow...
	*/
	Logger& operator<<(string& a) {

		const wchar_t* swWho = L"Logger& operator<<(string& a)";
		const char* sWho = "Logger& operator<<(string& a)";

		if( debug ){
			cout << sWho << "(): a='" << a << "'..." << endl;
		}
		wstring aw = JDA::Utils::narrowToWideS(a);

		if( debug ){
			wcout << swWho << L"(): aw='" << aw << L"'..." << endl;
		}

	    this->oss << aw;

	    return *this;
	}
	#endif

    Logger& operator<<( std::OSTREAM&(*f)(std::OSTREAM&) )
    {
		STRING sWho = _TT("operator<<");

		oss << f;

		/* If we have a std::endl, dump oss to the log and re-set oss... */
        if( f == ::std::endl ) {

			this->flush();

			//if( debug ) {
            //	COUT << sWho << _TT("(): messageLevel=") << this->messageLevel << _TT(", debugLevel=") << this->debugLevel << "..." << std::endl;
   	        //	COUT << sWho << _TT("(): oss.str() = '") << oss.str() << _TT("'...") << endl;
			//}

			//if( willPrint( this->messageLevel ) ) {
			//
			//	if( debug ) {
            //		COUT << sWho << _TT("(): it's a go...!") << endl;
			//	}
			//
			//	printIt( this->messageLevel, oss.str() );
			//
			//}
			//else {
			//	if( debug ) {
            //		COUT << sWho << _TT("(): it's NOT a go...!") << endl;
			//	}
			//}

            //oss.str(_TT("")); // Re-set oss back to empty string so we can start appending to it again...

			//this->messageLevel = INFO; // Re-set back to default message level...

        }/* if( f == std::endl ) */

        return *this;

    }/* Logger& operator<<( std::OSTREAM&(*f)(std::OSTREAM&) ) */

	Logger& flush(){

			STRING sWho = _TT("Logger::flush");

			if( debug ) {
            	COUT << sWho << _TT("(): messageLevel=") << this->messageLevel << _TT(", debugLevel=") << this->debugLevel << "..." << std::endl;
   	        	COUT << sWho << _TT("(): oss.str() = '") << oss.str() << _TT("'...") << endl;
			}

			if( willPrint( this->messageLevel ) ) {

				if( debug ) {
            		COUT << sWho << _TT("(): it's a go...!") << endl;
				}

				printIt( this->messageLevel, oss.str() );

			}
			else {
				if( debug ) {
            		COUT << sWho << _TT("(): it's NOT a go...!") << endl;
				}
			}

            oss.str(_TT("")); // Re-set oss back to empty string so we can start appending to it again...

			this->messageLevel = INFO; // Re-set back to default message level...

			return *this;
	}

	Logger& operator()(const DebugLevelType& MessageLevel)
	{
		//std::cout << "(Level " << Level << ")...";
		this->messageLevel = MessageLevel;
		return *this;
	}

	static STRING getLevelNamesAsCommaList(){
		OSTRINGSTREAM oss_out;
		oss_out << Logger::getLevelName(EMERG)
			<< "," << Logger::getLevelName(FATAL)
			<< "," << Logger::getLevelName(ALERT)
			<< "," << Logger::getLevelName(CRIT)
			<< "," << Logger::getLevelName(ERROR)
			<< "," << Logger::getLevelName(WARN)
			<< "," << Logger::getLevelName(NOTICE)
			<< "," << Logger::getLevelName(INFO)
			<< "," << Logger::getLevelName(DEBUG)
			<< "," << Logger::getLevelName(TRACE) << "(or " << Logger::getLevelName(VDEBUG) << ")";

		return oss_out.str();
	}
	
	//static std::string Logger::getLevelName(DebugLevelType Level) {
	static STRING getLevelName(DebugLevelType Level) {
	
		switch (Level)
		{
			case Logger::EMERG:
				return STRING(_TT("EMERG"));
	
			case Logger::FATAL:
				return STRING(_TT("FATAL"));
	
			case Logger::ALERT:
				return STRING(_TT("ALERT"));
	
			case Logger::CRIT:
				return STRING(_TT("CRIT"));
	
			case Logger::ERROR:
				return STRING(_TT("ERROR"));

			//case Logger::ERR:
			//	return STRING(_TT("ERR"));

			case Logger::WARN:
				return STRING(_TT("WARN"));

			case Logger::NOTICE:
				return STRING(_TT("NOTICE"));

			case Logger::INFO:
				return STRING(_TT("INFO"));

			case Logger::DEBUG:
				return STRING(_TT("DEBUG"));

			//case Logger::DBG:
			//	return STRING(_TT("DBG"));
			
			//case Logger::VDEBUG:
			//	return STRING(_TT("VDEBUG"));

			case Logger::TRACE:
				return STRING(_TT("TRACE"));

			case Logger::NOTSET:
				return STRING(_TT("NOTSET"));

			default:
				return STRING(_TT("UNKNOWN"));
		}
	}/* Logger::getLevelName() */

	static DebugLevelType LevelNameToDebugLevel(STRING sLevel) {
	
		if( sLevel.compare(_TT("EMERG")) == 0 ){
			return Logger::EMERG;
		}
		else if( sLevel.compare(_TT("FATAL")) == 0 ){
			return Logger::FATAL;
		}
		else if( sLevel.compare(_TT("ALERT")) == 0 ){
			return Logger::ALERT;
		}
		else if( sLevel.compare(_TT("CRIT")) == 0 ){
			return Logger::CRIT;
		}
		else if( sLevel.compare(_TT("ERROR")) == 0 ){
			return Logger::ERROR;
		}
		else if( sLevel.compare(_TT("ERR")) == 0 ){
			return Logger::ERR;
		}
		else if( sLevel.compare(_TT("WARN")) == 0 ){
			return Logger::WARN;
		}
		else if( sLevel.compare(_TT("NOTICE")) == 0 ){
			return Logger::NOTICE;
		}
		else if( sLevel.compare(_TT("INFO")) == 0 ){
			return Logger::INFO;
		}
		else if( sLevel.compare(_TT("DEBUG")) == 0 ){
			return Logger::DEBUG;
		}
		else if( sLevel.compare(_TT("DBG")) == 0 ){
			return Logger::DBG;
		}
		else if( sLevel.compare(_TT("VDEBUG")) == 0 ){
			return Logger::VDEBUG;
		}
		else if( sLevel.compare(_TT("TRACE")) == 0 ){
			return Logger::TRACE;
		}
		else if( sLevel.compare(_TT("NOTSET")) == 0 ){
			return Logger::NOTSET;
		}
		else {
			return Logger::NOTSET;
		}
	}/*	static DebugLevelType Logger::LevelNameToDebugLevel(STRING sLevel) */

}; /* class Logger */ 


}/* namespace JDA */

#endif /* #ifndef _T_LOGGER_H */
