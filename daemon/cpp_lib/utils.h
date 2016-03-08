#ifndef _UTILS_H
#define _UTILS_H

// To disable deprecation warnings, try -D_CRT_SECURE_NO_WARNINGS as part of CFLAGS in makefile instead.
//#ifdef _MSC_VER
// #define _CRT_SECURE_NO_WARNINGS
//#endif
//#pragma warning(disable:4786)

#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <exception>

#include <windows.h>
#include <process.h>
#include <memory.h>
#include <fcntl.h>
#include <io.h>
#include <tchar.h>
#include <math.h>

#include <iomanip> /* For numberFormatWithCommas() */
//#include <locale>  /* For numberFormatWithCommas() */
#include <cctype> /* isdigit() */

#include <ctime>

#ifdef _BOOST_
	#include "boost/date_time/local_time/local_time.hpp"
#endif

using namespace std;

namespace JDA
{

class Utils {

	public:

	static int debug;

	static void setDebug( int debug ){
		JDA::Utils::debug = debug;
	}

	static int getDebug(){
		return JDA::Utils::debug;
	}

	static string s_nyc_posix_timezone; /* "EST-5EDT,M4.1.0,M10.5.0" */
	static string s_utc_posix_timezone; /* "UTC-0" */

	/** Our own little Exception class...niftily stores an error string for us... */
	class Exception : std::exception {

		public:

			string errString;

			Exception(const string& errString){
				this->errString = errString;
			}

			virtual const char* what() const throw()
			{
				return this->errString.c_str();
			}

	};

	class Stopwatch {

		protected:
			clock_t m_le_tic_tak_start;		
			clock_t m_le_tic_tak_stop;		

		public:
			Stopwatch(){
				m_le_tic_tak_start = clock();
				m_le_tic_tak_stop = m_le_tic_tak_stop;
			}

			void start(){
				m_le_tic_tak_start = clock();
				m_le_tic_tak_stop = m_le_tic_tak_stop;
			}

			void stop(){
				m_le_tic_tak_stop = clock();
			}

			float get_elapsed_seconds(){
				return ((float)(m_le_tic_tak_stop - m_le_tic_tak_start))/CLOCKS_PER_SEC;
			}

			string get_s_elapsed_seconds( int precision = 3 );

	}; /* JDA::Utils::Stopwatch */


	/** Maximum number of lines the output console should have,
	* used by redirectIOToConsole()...
	*/
	static const WORD MAX_CONSOLE_LINES = 500;

	/**
	* @return C<true> if s contains whitespace according to isspace(), C<false> otherwise.
	*/
	static inline bool has_whitespace(const std::string &s) {
		for( size_t i = 0; i < s.size(); i++ ){
			if( isspace( s[i] ) ){
				return true;
			}
		}

		return false;
	}

	static inline std::string& trim_leading_zeroes(std::string& src)
	{
		return JDA::Utils::ltrim(src, '0');
	} 

	/* Trim all of a specific character from left side of string,
	* for example, supply '0' - zero to trim leading zero's...
	*
	* http://forums.codeguru.com/showthread.php?301840-removing-leading-zeros*
	*/
	static inline std::string& ltrim(std::string& src, char chr)
	{
		std::string::size_type pos =  src.find_first_not_of(chr,0);

		//if(pos > 0){
		if(pos != std::string::npos ){
			src.erase(0,pos);     
		}

		return src;
	} 



	/**
	* trim from start
	* @source: http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring 
	*/
	static inline std::string &ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
		return s;
	}

	/**
	* trim from end 
	* @source: http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
	*/
	static inline std::string &rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
		return s;
	}

	/**
	* trim() white-space from both ends.  Operates on the string supplied and returns a reference to it.
	* @source: http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
	*/
	static inline std::string &trim(std::string &s) {
		return ltrim(rtrim(s));
	}

	/** Reads "key = value" pairs from file at <code>filePath</code> and loads them into
	 * supplied <code>map</code>.
	 *
	 * @return Number of key value pairs loaded into <code>map</code>.
	 *
	 * @throws JDA::Utils::Exception() if file at <code>filePath</code> cannot be opened.
	 */
	static int read_config_file(const string& filePath,
					map<string, string>& map /* in-out */);

	/** Used by read_config_file() to process each config line. */	
	static int process_config_line( string sLine,
					string& sKey /* out */,
					string& sValue /* out */ );


	/** 
	* Outputs current local timestamp of form "2013-07-25 14:21:25"
	* using the string s_in_out supplied, and returning a reference to it.
	*
	* If epoch_seconds in seconds since January 1st, 1970 is supplied
	* prints timestamp for that epoch_seconds, for example
	* get_local_timestamp( 0 ) outputs "1969-12-31 19:00:00"
	* which is the local (GMT-5) time conversion of midnight
	* January 1st, 1970 UTC.
	*/
	static inline string& get_local_timestamp( string& s_in_out, time_t epoch_seconds = -1){

		if( epoch_seconds == -1 ) {
			time( &epoch_seconds );
		}

		//struct tm *p_newtime;
		struct tm newtime;
		//newtime = localtime( &epoch_seconds );
		localtime_s(&newtime, &epoch_seconds);
	
		const int buf_sz = 256;
		char buffy[buf_sz];
	
		//sprintf_s(buffy, buf_sz, "%04d-%02d-%02d %02d:%02d:%02d", 
	   	//    1900 + newtime.tm_year,
		//	newtime.tm_mon + 1,
		//	newtime.tm_mday,
		//	newtime.tm_hour,
		//	newtime.tm_min,
		//	newtime.tm_sec
		//);
		
		strftime(buffy, buf_sz, "%Y-%m-%d %H:%M:%S %z", &newtime); 
	
	    s_in_out = buffy;
		return s_in_out;

	}/* get_local_timestamp() */

	/** 
	* Outputs current _local_ timestamp of form "2013-07-25 14:21:25",
	* returning a string object by value.
	*
	* If epoch_seconds in seconds since January 1st, 1970 is supplied
	* prints timestamp for that epoch_seconds, for example
	* get_local_timestamp( 0 ) outputs "1969-12-31 19:00:00"
	* which is the local (GMT-5) time conversion of midnight
	* January 1st, 1970 UTC.
	*/
	static inline string get_local_timestamp( time_t epoch_seconds = -1){

	    string s_out;

		JDA::Utils::get_local_timestamp( s_out, epoch_seconds );	

		return s_out;

	}/* get_local_timestamp() */

	static inline string& get_local_pretty_timestamp( string& s_in_out, time_t epoch_seconds = -1){

		if( epoch_seconds == -1 ) {
			time( &epoch_seconds );
		}

		//struct tm *p_newtime;
		struct tm newtime;

		//newtime = localtime( &epoch_seconds );
		localtime_s(&newtime, &epoch_seconds);
	
		const int buf_sz = 256;
		char buffy[buf_sz];
	
		//sprintf_s(buffy, buf_sz, "%04d-%02d-%02d %02d:%02d:%02d", 
	   	//    1900 + newtime.tm_year,
		//	newtime.tm_mon + 1,
		//	newtime.tm_mday,
		//	newtime.tm_hour,
		//	newtime.tm_min,
		//	newtime.tm_sec
		//);
		
		//strftime(buffy, buf_sz, "%Y-%m-%d %H:%M:%S %z", &newtime); 
		strftime(buffy, buf_sz, "%A, %B %d, %Y %I:%M:%S %p %z", &newtime); 
	
	    s_in_out = buffy;
		return s_in_out;
	}

	static inline string get_local_pretty_timestamp( time_t epoch_seconds = -1){
	    string s_out;
		JDA::Utils::get_local_pretty_timestamp( s_out, epoch_seconds );	
		return s_out;
	}


	static inline bool nth_wday_in_month( 
		int n,
		int wday, /* 0=sunday, 1=monday, ... */
		int month, /* 0=january, 1=february, ... */
		int year, /* (year-1900), e.g., 113="2013", 114="2014", ... */
		int* mday_out,
		int* yday_out
	){
		const char* sWho = "JDA::Utils::nth_wday_in_month";

		if( JDA::Utils::debug ){
			cout << sWho << "(): n=" << n <<", wday=" << wday << ", month=" << month << ", year=" << year << "..." << endl;
		}

		struct tm tm_struct;


		int iHitCount = 0;

		char buff[80];
		
		for(int i=1; i<31; i++){

			//tm_struct.tm_year = year - 1900;
			tm_struct.tm_year = year;
			//tm_struct.tm_mon = month - 1;
			tm_struct.tm_mon = month;

			/** IMPORTANT: Setting tm_hour to 0 (i.e., midnight) caused freaky behavior
			* ... March 1st became February 28th
			* ...  so we were missing counting the first Sunday if it occurred on March 1st...
			* ...  the simple remedy is to set tm_hour to 11 (i.e. 11 AM ).
			*/
			tm_struct.tm_hour = 11; 
			tm_struct.tm_min = 0;
			tm_struct.tm_sec = 0;

			tm_struct.tm_mday = i;

			tm_struct.tm_wday = 0;
			tm_struct.tm_yday = 0;

			if( JDA::Utils::debug ){
				cout << sWho << "(): i=" << i << ": BEFORE: tm_struct = \n" << JDA::Utils::t_struct_to_string( tm_struct ) << "..." << endl;
			}

			// time_t mktime( struc tm * );
			// A call to this function automatically adjusts the values of the members of timeptr
			// if they are off-range or -in the case of tm_wday and tm_yday- if they have values
			// that do not match the date described by the other members.
			//mktime( &tm_struct );

			time_t result;

			if( (result = mktime( &tm_struct)) != (time_t)-1 ) {

				asctime_s( buff, sizeof(buff), &tm_struct );

				if( JDA::Utils::debug ){
					cout << "AFTER: mktime time is " << buff << endl;
				}
			} else {
				//perror( "mktime failed" );
				cerr << "mktime failed: " << JDA::Utils::s_error() << endl;
			}


			if( JDA::Utils::debug ){
				cout << sWho << "(): i=" << i << ": AFTER: tm_struct = \n" << JDA::Utils::t_struct_to_string( tm_struct ) << "..." << endl;
			}

			if( tm_struct.tm_mon != month ){ 
				// NOTE: We were getting freaky behavior for March 1st ... call it "The mktime() March 1st Bug"...
				// ...it was getting pushed to February 28th...
				// ... causing us to "miss" counting a Sunday if March 1st was a Sunday.  We were going to try ignoring
				// the iterations where tm_struct.tm_mon != month, then "backstrapolate" to get to the missed Sunday,
				// but a simpler remedy was to simply set the hour to 11 (i.e., 11AM) instead of 0 (i.e., midnight,
				// the bewitching hour)...
				//if( JDA::Utils::debug ){
					cerr << sWho << "(): i=" << i << ": DANGER!  DANGER, WILL ROBINSON!  mktime adjusted the month from " << month << " to " << tm_struct.tm_mon << "..." << endl; 
				//}
			}

			if( tm_struct.tm_wday ==  wday ){

				iHitCount++;

				if( JDA::Utils::debug ){
					cout << sWho << "(): i=" << i << ": AFTER: A hit!!! iHitCount now equals " << iHitCount << "..." << endl;
				}

				if( iHitCount == n ){

					if( JDA::Utils::debug ){
						cout << sWho << "(): i=" << i << ": We're at target hitcount, so setting *mday_out to i = " << i << ", setting *yday_out to tm_struct.tm_yday = " << tm_struct.tm_yday << "..." << endl; 
					}

					*(mday_out) = i;
					*(yday_out) = tm_struct.tm_yday;

					return true;
				}
			}
		}

		return false;

}/* static inline int nth_wday_in_month()  */

static string t_struct_to_string(const struct tm & t_struct){
	ostringstream oss;
	oss
	 << "tm_year = " << t_struct.tm_year << "\n"
	 << "tm_mon = " << t_struct.tm_mon << "\n"
	 << "tm_mday = " << t_struct.tm_mday << "\n"
	 << "tm_hour = " << t_struct.tm_hour << "\n"
	 << "tm_min = " << t_struct.tm_hour << "\n"
	 << "tm_sec = " << t_struct.tm_sec << "\n" 
	 << "tm_wday = " << t_struct.tm_wday << "\n"
	 << "tm_yday = " << t_struct.tm_yday << "\n"
	 << "tm_isdst = " << t_struct.tm_isdst;

	return oss.str();
}

static bool is_dst( const struct tm & t_struct ){

		string sWho = "JDA::Utils::is_dst";

		if( JDA::Utils::debug ){
			cout << sWho << "(): OK, Escamillo: t_struct =\n"
				 << JDA::Utils::t_struct_to_string( t_struct ) << "..." << endl;
		}

		int dst_start_n = 2; /* 2nd */
		int dst_start_wday = 0; /* Sunday */
		int dst_start_month = 2; /* March */ 
		int dst_start_mday;
	    int dst_start_yday;
		//JDA::Utils::nth_wday_in_month( dst_start_n, dst_start_wday, dst_start_month, t_struct.tm_year + 1900, &dst_start_mday, &dst_start_yday ); 
		JDA::Utils::nth_wday_in_month( dst_start_n, dst_start_wday, dst_start_month, t_struct.tm_year, &dst_start_mday, &dst_start_yday ); 

		if( JDA::Utils::debug ){
			cout << sWho << "(): dst_start_mday=" << dst_start_mday << ", dst_start_yday=" << dst_start_yday << "..." << endl;
		}

		int dst_end_n = 1; /* 1st */
		int dst_end_wday = 0; /* Sunday */
		int dst_end_month = 10; /* November */ 
		int dst_end_mday;
		int dst_end_yday;
	   	//JDA::Utils::nth_wday_in_month( dst_end_n, dst_end_wday, dst_end_month, t_struct.tm_year + 1900, &dst_end_mday, &dst_end_yday ); 
	   	JDA::Utils::nth_wday_in_month( dst_end_n, dst_end_wday, dst_end_month, t_struct.tm_year, &dst_end_mday, &dst_end_yday ); 

		if( JDA::Utils::debug ){
			cout << sWho << "(): dst_end_mday=" << dst_end_mday << ", dst_end_yday=" << dst_end_yday << "..." << endl;
		}

		bool geStart = false;

		//if( t_struct.tm_yday > dst_start_yday )
		//{
		//	/* Past first day of DST */
		//	geStart = true;
		//
		//	if( JDA::Utils::debug ){
		//		cout << sWho << "(): Case 1A..." << endl;
		//	}
		//}

		if( t_struct.tm_mon > dst_start_month )
		{
			geStart = true;

			if( JDA::Utils::debug ){
				cout << sWho << "(): Case 1A..." << endl;
			}
		}
		else if(
			t_struct.tm_mon == dst_start_month 
				&&
			t_struct.tm_mday > dst_start_mday
		){
			geStart = true;

			if( JDA::Utils::debug ){
				cout << sWho << "(): Case 1B..." << endl;
			}
		}
		else if(
			t_struct.tm_mon == dst_start_month 
				&&
			t_struct.tm_mday == dst_start_mday
				&&
			t_struct.tm_hour >= 2 /* at or after 2AM EST */
		){
			geStart = true;

			if( JDA::Utils::debug ){
				cout << sWho << "(): Case 1C..." << endl;
			}
		}

		bool ltEnd = false;

		if( t_struct.tm_mon < dst_end_month )
		{
			ltEnd = true;

			if( JDA::Utils::debug ){
				cout << sWho << "(): Case 2A..." << endl;
			}
		}
		else if( t_struct.tm_mon == dst_end_month
				&&
			t_struct.tm_mday < dst_end_mday
		)
		{
			ltEnd = true;

			if( JDA::Utils::debug ){
				cout << sWho << "(): Case 2B..." << endl;
			}
		}
		else if( t_struct.tm_mon == dst_end_month
				&&
			t_struct.tm_mday == dst_end_mday
				&&
			//t_struct.tm_hour < 2 /* Before 2AM EDT */
			t_struct.tm_hour < 1 /* Before 1AM EST */
		){
			ltEnd = true;

			if( JDA::Utils::debug ){
				cout << sWho << "(): Case 2C..." << endl;
			}
		}

		return( geStart && ltEnd );
		
}/* bool is_dst( const struct tm & t_struct ) */

/** Fills in t_struct pointed to by p_t_struct with NYC time settings
* for epoch_seconds.  Explicitly sets tm_isdst to 1 if daylight savings time,
* or to 0 if not daylight savings time.  Many Bothans died to bring 
* us this function, especially during testing of nth_wday_in_month()
* upon which is_dst() depends.  The original plan was to use 
* boost::date_time, but it unfortunately proved buggy...
*/
static void nyctime( struct tm * p_t_struct, time_t epoch_seconds){

		//bool isDst = false;

		time_t epoch_seconds_adjusted = epoch_seconds - (3600 * 5); /* Begin by subtracting 5 hours. */

		//struct tm t_struct;

		gmtime_s(p_t_struct, &epoch_seconds_adjusted );

		if( JDA::Utils::is_dst( *(p_t_struct) ) ){

			//isDst = true;

			/* If DST, add one hour, and re-load t_struct. */	
			epoch_seconds_adjusted += 3600;
			gmtime_s(p_t_struct, &epoch_seconds_adjusted );

			p_t_struct->tm_isdst = 1;
		}
		else {
			p_t_struct->tm_isdst = 0;
		}

		//if( JDA::Utils::debug ){
		//	cout << sWho << "(): isDst = " << JDA::Utils::boolToString( isDst ) << "..." << endl;
		//}
}/* nyctime() */

/** e.g., "2014-01-28" or "2014-09-05", suitable for embedding in a filename... */
static inline string& get_nyc_datestamp( string& s_in_out, time_t epoch_seconds = -1){

		//const char* sWho = "JDA::Utils::get_nyc_datestamp";

		if( epoch_seconds == -1 ) {
			time( &epoch_seconds );
		}

		struct tm t_struct;

		JDA::Utils::nyctime( &t_struct, epoch_seconds );

		const int buf_sz = 256;
		char buffy[buf_sz];
	
		if( t_struct.tm_isdst ){
			strftime(buffy, buf_sz, "%Y-%m-%d", &t_struct); 
		}
		else {
			strftime(buffy, buf_sz, "%Y-%m-%d", &t_struct); 
		}
	
	    s_in_out = buffy;

		return s_in_out;

}/* static inline string& get_nyc_datestamp( string& s_in_out, time_t epoch_seconds = -1) */

static inline string get_nyc_datestamp( time_t epoch_seconds = -1){
	string sInOut;
	JDA::Utils::get_nyc_datestamp( sInOut, epoch_seconds );
	return sInOut;
}

/** Generates a string with format "YYYYMMDD" representing
* yesterday's date stamp, where yesterday is always a weekday,
* e.g., if today is Saturday, Sunday, or Monday, "yesterweekday" is
* Friday.  If today is Tuesday, yesterweekday is Monday...
* If today is Friday, yesterday is Thursday.
*
* Used by Forms Daemon and Waldo Daemon to form filename of
* daily index to be downloaded from EDGAR site, for example
*   master.20151203.idx
* downloaded from
*   ftp://ftp.sec.gov/edgar/daily-index/master.20151203.idx
*
* @epoch_seconds -- optional: if not supplied, defaults to current
*   epoch_seconds as determined by the time() function...
*/
static inline string get_nyc_yesterweekday_date_stamp( time_t epoch_seconds = 0 ){

	//const char* sWho = "JDA::Utils::get_nyc_yesterweekday_date_stamp";

	//time_t epoch_seconds; 

	if( epoch_seconds == 0 ){
		time( &epoch_seconds );
	}

	struct tm t_struct;
	JDA::Utils::nyctime( &t_struct, epoch_seconds );

	time_t epoch_seconds_shifted;

	const int SECONDS_PER_DAY = 3600 * 24;

    /* struct tm.tm_wday:
	* days since Sunday: (0-6):
	* 0 = Sunday
	* 1 = Monday
	* 2 = Tuesday
	* 3 = Wednesday
	* 4 = Thursday
	* 5 = Friday
	* 6 = Saturday
	*/
	if( 0 == t_struct.tm_wday ){
		/* Sunday: Go back two days to Friday */
		epoch_seconds_shifted = epoch_seconds - (2*SECONDS_PER_DAY);
	}
	else if( 1 == t_struct.tm_wday ){
		/* Monday: Go back three days to Friday */
		epoch_seconds_shifted = epoch_seconds - (3*SECONDS_PER_DAY);
	}
	else {
		/* Tuesday, Wednesday, Thursday, Friday, or Saturday: Go back one day. */
		epoch_seconds_shifted = epoch_seconds - (1*SECONDS_PER_DAY);
	}

	struct tm t_struct_2;
	JDA::Utils::nyctime( &t_struct_2, epoch_seconds_shifted );

	const int buf_sz = 256;
	char buffy[buf_sz];

	strftime(buffy, buf_sz, "%Y%m%d", &t_struct_2); 

	string s_output = buffy;

	return s_output;

}/* get_nyc_yesterweekday_date_stamp() */

/** e.g., "2014-01-28 17:17:05 EST" or "2014-09-05 14:05:15 EDT". */
static inline string& get_nyc_timestamp( string& s_in_out, time_t epoch_seconds = -1){

		//const char* sWho = "JDA::Utils::get_nyc_timestamp";

		if( epoch_seconds == -1 ) {
			time( &epoch_seconds );
		}

		struct tm t_struct;

		JDA::Utils::nyctime( &t_struct, epoch_seconds );

		const int buf_sz = 256;
		char buffy[buf_sz];
	
		if( t_struct.tm_isdst ){
			strftime(buffy, buf_sz, "%Y-%m-%d %H:%M:%S EDT", &t_struct); 
		}
		else {
			strftime(buffy, buf_sz, "%Y-%m-%d %H:%M:%S EST", &t_struct); 
		}
	
	    s_in_out = buffy;

		return s_in_out;

}/* static inline string& get_nyc_timestamp( string& s_in_out, time_t epoch_seconds = -1) */


static inline string get_nyc_timestamp( time_t epoch_seconds = -1){
	string sInOut;
	JDA::Utils::get_nyc_timestamp( sInOut, epoch_seconds );
	return sInOut;
}

static inline string& get_nyc_pretty_timestamp( string& s_in_out, time_t epoch_seconds = -1){

		//const char* sWho = "JDA::Utils::get_nyc_pretty_timestamp";

		if( epoch_seconds == -1 ) {
			time( &epoch_seconds );
		}

		struct tm t_struct;

		JDA::Utils::nyctime( &t_struct, epoch_seconds );

		const int buf_sz = 256;
		char buffy[buf_sz];
	
		//strftime(buffy, buf_sz, "%Y-%m-%d %H:%M:%S %z", p_t_struct); 
		if( t_struct.tm_isdst ){
			strftime(buffy, buf_sz, "%A, %B %d, %Y %I:%M:%S %p EDT", &t_struct); 
		}
		else {
			strftime(buffy, buf_sz, "%A, %B %d, %Y %I:%M:%S %p EST", &t_struct); 
		}
	
	    s_in_out = buffy;

		return s_in_out;

}/* static inline string& get_nyc_pretty_timestamp( string& s_in_out, time_t epoch_seconds = -1) */

static inline string get_nyc_pretty_timestamp( time_t epoch_seconds = -1){
	string sInOut;
	JDA::Utils::get_nyc_pretty_timestamp( sInOut, epoch_seconds );
	return sInOut;
}

/**
* Accepts a LOCAL time of format "YYYY-MM-DD HH:MM:SS", and return number of epoch seconds.
*/
static time_t yyyymmddhhmmss_to_time_t( const string& yyyymmddhhmmss ){

	int year, month, day, hours, minutes, seconds;

	if( sscanf_s(yyyymmddhhmmss.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hours, &minutes, &seconds) == 6 ){
		struct tm tm_struct;
		tm_struct.tm_year = year - 1900; 	
		tm_struct.tm_mon = month - 1; 	
		tm_struct.tm_mday = day;

		tm_struct.tm_hour = hours;
		tm_struct.tm_min = minutes;
		tm_struct.tm_sec = seconds;

		time_t le_time = mktime( &tm_struct ); /* should return -1 if it can't figure the time... */

		return le_time;
	}
	else {
		return -2;
	}

}/* time_t yyyymmddhhmmss_to_time_t( const string& yyyymmddhhmmss ) */

/**
* Accepts a LOCAL date of format "YYYY-MM-DD", and return number of epoch seconds (assuming midnight or HH:MM:SS=00:00:00) .
*/
static time_t yyyymmdd_to_time_t( const string& yyyymmdd ){

	int year, month, day, hours, minutes, seconds;

	hours = 0;
	minutes = 0;
	seconds = 0;

	if( sscanf_s(yyyymmdd.c_str(), "%d-%d-%d", &year, &month, &day) == 3 ){
		struct tm tm_struct;
		tm_struct.tm_year = year - 1900; 	
		tm_struct.tm_mon = month - 1; 	
		tm_struct.tm_mday = day;

		tm_struct.tm_hour = hours;
		tm_struct.tm_min = minutes;
		tm_struct.tm_sec = seconds;

		time_t le_time = mktime( &tm_struct ); /* should return -1 if it can't figure the time... */

		return le_time;
	}
	else {
		return -2;
	}

}/* time_t yyyymmdd_to_time_t( const string& yyyymmdd ) */


	/** 
	* Outputs current UTC timestamp of form "2013-07-25 19:21:25 UTC"
	* using the string s_in_out supplied, and returning a reference to it.
	*
	* If epoch_seconds in seconds since January 1st, 1970 is supplied
	* prints timestamp for that epoch_seconds, for example
	* get_utc_timestamp( 0 ) outputs "1970-01-01 00:00:00"
	* which is midnight January 1st, 1970 UTC, the beginning
	* of the "epoch".
	*/
	static inline string& get_utc_timestamp( string& s_in_out,
					time_t epoch_seconds = -1,
					const string& s_format = "%Y-%m-%d %H:%M:%S UTC"){

		if( epoch_seconds == -1 ) {
			time( &epoch_seconds );
		}

		//struct tm *p_newtime;
		struct tm newtime;

		//newtime = gmtime( &epoch_seconds );
		gmtime_s(&newtime, &epoch_seconds);
	
		const int buf_sz = 256;
		char buffy[buf_sz];
	
		//sprintf_s(buffy, buf_sz, "%04d-%02d-%02d %02d:%02d:%02d", 
	   	//    1900 + newtime.tm_year,
		//	newtime.tm_mon + 1,
		//	newtime.tm_mday,
		//	newtime.tm_hour,
		//	newtime.tm_min,
		//	newtime.tm_sec
		//);
		
		strftime(buffy, buf_sz, s_format.c_str(), &newtime); 
	
	    s_in_out = buffy;
		return s_in_out;

	}/* get_utc_timestamp() */

	/** 
	* Outputs current UTC timestamp of form "2013-07-25 19:21:25 UTC"
	* returning a string object by value. 
	*
	* If epoch_seconds in seconds since January 1st, 1970 is supplied
	* prints timestamp for that epoch_seconds, for example
	* get_utc_timestamp( 0 ) outputs "1970-01-01 00:00:00"
	* which is midnight January 1st, 1970 UTC, the beginning
	* of the "epoch".
	*/
	static inline string get_utc_timestamp( time_t epoch_seconds = -1){

	    string s_out;

		JDA::Utils::get_utc_timestamp( s_out, epoch_seconds );	

		return s_out;

	}/* get_utc_timestamp() */


	/** 
	* Outputs current UTC timestamp of file-name-friendly format "2013-07-25-19-21-25-utc"
	* using the string s_in_out supplied, and returning a reference to it.
	*/
	static inline string& get_utc_filename_timestamp( string& s_in_out, time_t epoch_seconds = -1 ){
		return get_utc_timestamp( s_in_out,
						epoch_seconds,
						"%Y-%m-%d-%H-%M-%S-utc");
	}

	static inline string get_utc_filename_timestamp( time_t epoch_seconds = -1 ){
	    string s_out;

		JDA::Utils::get_utc_filename_timestamp( s_out, epoch_seconds );	

		return s_out;
	}

	/** 
	* Outputs current UTC timestamp of in pretty format "..."
	* using the string s_in_out supplied, and returning a reference to it.
	*/
	static inline string& get_utc_pretty_timestamp( string& s_in_out, time_t epoch_seconds = -1 ){
		return get_utc_timestamp( s_in_out,
						epoch_seconds,
						"%A, %B %d, %Y %I:%M:%S %p UTC");
	}

	static inline string get_utc_pretty_timestamp( time_t epoch_seconds = -1 ){
	    string s_out;

		JDA::Utils::get_utc_pretty_timestamp( s_out, epoch_seconds );	

		return s_out;
	}



	/** 
	* Run this if you want to create and read and write
    * to a command-prompt via stdin and stdout even if you're
	* in a Win32 GUI program.
	*
	* Create DOS window we can access via stdout and stdin... 
	*
	* ORIGINAL SOURCE courtesy Andrew Tucker: http://dslweb.nwnexus.com/~ast/dload/guicon.htm
	*/
	static void redirectIOToConsole();

	/** 
    * split() functionality where you supply your own vector<std::string>.
	*
	* Note that this only splits on a single character.  Some day, Johnny, you can
	* hack up a version that splits on multiple characters, or even regular
	* expressions...
	*
	* ORIGINAL SOURCE: http://stackoverflow.com/questions/236129/splitting-a-string-in-c
	*/
	static inline std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems, bool empties_too = false ) {
		std::stringstream ss(s);
		std::string item;
	    while (std::getline(ss, item, delim)) {
			// Don't bother to push empty strings onto the vector...unless empties_too is TRUE...
			if( empties_too || JDA::Utils::trim( item ).length() > 0 ) {	
				elems.push_back(item);
			}
		}
		return elems;
	}


	/** 
    * split() functionality where a vector<std::string> is created and returned to you.
	*
	* ORIGINAL SOURCE: http://stackoverflow.com/questions/236129/splitting-a-string-in-c
	*/
	static inline std::vector<std::string> split(const std::string &s, char delim, bool empties_too = false ) {
		std::vector<std::string> elems;
		split(s, delim, elems, empties_too );
		return elems;
	}


	/** 
    * split() functionality where you supply your own vector<std::string>.
	*
	* This is our own little hack that splits on a string rather than single character.
	*
	* ORIGINAL SOURCE: http://stackoverflow.com/questions/236129/splitting-a-string-in-c
	*/
	static inline std::vector<std::string> &split(const std::string &s, const std::string& delim, std::vector<std::string> &elems, bool empties_too = false ) {

		const char* sWho = "JDA::Utils::split";

		if( JDA::Utils::debug ){
    		cout << sWho << "(): SHEMP: Moe, s.length() = " << s.length() << ", Moe..." << endl;
		}

		size_t i_where_seek = 0;

		size_t i_where_last_found = string::npos;
		size_t i_where_found = string::npos;

		size_t i_substr_start = string::npos;
		size_t i_substr_length = string::npos;

		while( true ){

			i_where_found = s.find( delim, i_where_seek );  

			if( i_where_found == string::npos ){
				break;
			}

			if( i_where_last_found == string::npos ){
				// We're at beginning of the string...
				i_substr_start = 0;
			}
			else {
				i_substr_start = i_where_last_found + delim.length(); 
			}

			i_substr_length = i_where_found - i_substr_start;

			if( empties_too == false ){
				if( i_substr_length > 0 ){
					elems.push_back( s.substr( i_substr_start, i_substr_length ) );	
				}
			}
			else {
				elems.push_back( s.substr( i_substr_start, i_substr_length ) );	
			}

			i_where_last_found = i_where_found;
			i_where_seek = i_where_found + delim.length(); 

		}/* while( true ) */


		if( JDA::Utils::debug ){
    		cout << sWho << "(): SHEMP: Moe, after exiting dha while() loop..." << "\n"
			<< "\t" << "i_where_seek = " << i_where_seek << "\n"
			<< "\t" << "i_where_last_found = " << i_where_last_found << "\n"
			<< "\t" << "i_where_found = " << i_where_found << "\n"
			<< "\t" << "i_substr_start = " << i_substr_start << "\n"
			<< "\t" << "i_substr_length = " << i_substr_length << endl;
		}

		// Handle any leftover string...
		if( i_where_seek < s.length() ){
			elems.push_back( s.substr( i_where_seek ) ); // Remainder of string...
		}

		return elems;

	}/* split() */


	/** 
    * split() functionality where a vector<std::string> is created and returned to you.
	*
	* ORIGINAL SOURCE: http://stackoverflow.com/questions/236129/splitting-a-string-in-c
	*/
	static inline std::vector<std::string> split(const std::string &s, const std::string& delim, bool empties_too = false ) {
		std::vector<std::string> elems;
		split(s, delim, elems, empties_too );
		return elems;
	}


/** Apply "sprintf()" type functionality to a std::string created on stack and returned. */
static string JDA::Utils::s_string_printf( char* szFormat, ... ){

	string s;

	// REFERENCE: http://msdn.microsoft.com/en-us/library/28d5ce15.aspx

	va_list pArgList;

	// The va_start macro (defined in STDARG.H) is usually
	// equivalent to:
	// pArgList = (char *) &szFormat + sizeof( szFormat );
	va_start( pArgList, szFormat );

	// Use _vscprintf() to figure out what size buffer we need.
	// _vscprintf() doesn't count terminating '\0', so add 1...
	int len = _vscprintf( szFormat, pArgList ) + 1;

	// Allocate a sufficiently sized buffer...
	char* buffer = (char*) malloc( len * sizeof(char) );

	vsprintf_s( buffer, len, szFormat, pArgList ); // C4996

	// The va_end macro just zeroes out pArgList for no good reason
	va_end( pArgList );

	s = buffer;

	// Free the buffer...
	free( buffer );

	return s;
}/* static string JDA::Utils::s_string_printf( char* format, ... ) */

/** Apply "sprintf()" type functionality to a std::string supplied as argument,
* with a reference to the same string object returned.
*/
static string& JDA::Utils::string_printf( string& s, char* format, ... ){

	// REFERENCE: http://msdn.microsoft.com/en-us/library/28d5ce15.aspx

	va_list args;
	va_start( args, format );

	// Use _vscprintf() to figure out what size buffer we need.
	// _vscprintf() doesn't count terminating '\0', so add 1...
	int len = _vscprintf( format, args ) + 1;

	// Allocate a sufficiently sized buffer...
	char* buffer = (char*) malloc( len * sizeof(char) );

	vsprintf_s( buffer, len, format, args ); // C4996

	s = buffer; // Load char buffer into a std::string...

	// Free the buffer...
	free( buffer );

	return s;

}/* static string& JDA::Utils::string_printf( string& s, char* format, ... ) */

#ifdef _GET_ENV_STRINGS_
/** 
* Uses Windows function GetEnvironmentString() to load all environment
* variables into a vector<string>.
*/
static vector<string>& JDA::Utils::get_env_strings( vector<string>& env_strings /* in-out */){

	// Returns env variables as single char buffer delimited by '\0' characters...
	const char* a = (LPWCH) GetEnvironmentStrings();

	int prev = 0;
	for(int i = 0; ; i++) {
		if (a[i] == '\0') {
			env_strings.push_back(std::string(a + prev + 1, a + i));
			prev = i;
			if (a[i + 1] == '\0') {
				break;
			}
		}
	}

	return env_strings;

}/* get_env_strings() */
#endif

template<class T> static string& vector_to_string( vector<T>& v , string& s ) {
	ostringstream oss;
	oss << v;
	s = oss.str();
	return s;
}

template<class T> static string vector_to_string( vector<T>& v ) {
	string s;
   	JDA::Utils::vector_to_string(v, s);
	return s;
}

template<class K, class V> static string& map_to_string( map<K,V>& m, string& s ) {
	ostringstream oss;
	oss << m;
	s = oss.str();
	return s;
}

template<class K, class V> static string map_to_string( map<K,V>& m ) {
	string s;
   	JDA::Utils::map_to_string(m, s);
	return s;
}
 
/**
* @reference http://stackoverflow.com/questions/5343190/how-do-i-replace-all-instances-of-of-a-string-with-another-string 
*/
static void replace_string_in_place(std::string& subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}


/* In-place conversion C:/UNIX/style/file/path to C:\DOS\style\file\path */
static void unix_path_to_dos_path_in_place ( string& s_in_out ) {

	for( size_t i = 0; i < s_in_out.length(); i++ ){ 
		if( s_in_out[i] == '/' ){
			s_in_out[i] = '\\';
		}
	}

}/* unix_path_to_dos_path_in_place ( string& s_in_out ) */


/** Returns a string that is based on s_in but with 
* conversion of C:/UNIX/style/file/path to C:\DOS\style\file\path
*
* Convenience method so you don't hafta take an extra line
* of code for calling unix_path_to_dos_path_in_place().
*/
static string unix_path_to_dos_path ( const string& s_in ) {

	string s_out = s_in;

	JDA::Utils::unix_path_to_dos_path_in_place ( s_out );

	return s_out;

}/* unix_path_to_dos_path() */



/** Reference: http://msdn.microsoft.com/en-us/library/s7wzt4be.aspx 
*
*  errno_t wcstombs_s(
* 	size_t *pReturnValue,
* 	char *mbstr,
* 	size_t sizeInBytes,
* 	const wchar_t *wcstr,
* 	size_t count 
* );
*
*  [out] pReturnValue: The number of characters converted.
*  [out] mbstr:  The address of a buffer for the resulting converted multibyte character string.
*  [in]sizeInBytes:  The size in bytes of the mbstr buffer.
*  [in] wcstr:  Points to the wide character string to be converted.
*  [in] count:  The maximum number of wide characters to be stored in the mbstr buffer,
*   not including the terminating null character, or _TRUNCATE.
*/ 
static char* JDA::Utils::wideToNarrowP(const wchar_t* input)
{
    size_t   iReturnValue;
	errno_t  iReturnCode;

	const char* sWho = "wideToNarrowP";
	
	if( JDA::Utils::debug ){
		cout << sWho << "(): How many bytes do we need...?" << endl;
	}

	size_t iNumCharsNeeded;

    // Call the first time with NULL supplied for buffer to figure out how many bytes we'll need...
    iReturnCode = wcstombs_s(&iNumCharsNeeded, NULL, (size_t)0, 
               input, _TRUNCATE);

    if( iReturnCode != 0 )
	{
    	cerr << sWho << "(): Error on first call to wcstombs_s(): " << iReturnCode << endl;
    	//wcerr << sWho << L"(): Error on first call to wcstombs_s( '" << input << "' ): " << iReturnCode << endl;
		return NULL;
	}

	if( JDA::Utils::debug ){
    	cout << sWho << "(): We need " << iNumCharsNeeded << " chars..." << endl;
	}

	size_t iNumBytesNeeded = iNumCharsNeeded * sizeof(char);

	if( JDA::Utils::debug ){
    	cout << sWho << "(): We need " << iNumBytesNeeded << " bytes..." << endl;
	}

    char* pMBBuffer = (char *)malloc( iNumBytesNeeded );

	if( JDA::Utils::debug ){
    	cout << sWho << "(): Doing conversion..." << endl;
	}

    // Conversion
    iReturnCode = wcstombs_s(&iReturnValue, pMBBuffer, iNumCharsNeeded, 
               input, iNumCharsNeeded );

    if( iReturnCode != 0 )
	{
    	cerr << sWho << "(): Error on second call to wcstombs_s(): " << iReturnCode << endl;
		return NULL;
	}

	if( JDA::Utils::debug ){
    	cout << sWho << "(): Number of Characters converted: " << iReturnValue << endl;
	    cout << sWho << "(): Multibyte character string: \"" << pMBBuffer << "\"..." << endl;
	}

	return pMBBuffer;

}/* char* wideToNarrowP(const wchar_t* input) */

static string JDA::Utils::wideToNarrowS(const wstring& input){

	char* pMBBuffer = JDA::Utils::wideToNarrowP((const wchar_t *)input.c_str());

	if( pMBBuffer != NULL ){

		string sOut(pMBBuffer);

		free(pMBBuffer);

		return sOut;
	}
	else {
		return string("");
	}
}/* string JDA::Utils::wideToNarrowS(const wstring& input) */


/** REFERENCE: http://msdn.microsoft.com/en-us/library/eyktyxsx.aspx
* errno_t mbstowcs_s(
*    size_t *pReturnValue,
*    wchar_t *wcstr,
*    size_t sizeInWords,
*    const char *mbstr,
*    size_t count 
* );
*
* [out] pReturnValue: The number of characters converted.
* [out] wcstr:  Address of buffer for the resulting converted wide character string.
* [in] sizeInWords: The size of the wcstr buffer in words.
* [in] mbstr: The address of a sequence of null terminated multibyte characters.
* [in] count: The maximum number of wide characters to store in the wcstr buffer,
* not including the terminating null, or _TRUNCATE.
*/
static wchar_t* JDA::Utils::narrowToWideP(const char* input) {
    size_t   iReturnValue;
	errno_t  iReturnCode;

	const char* sWho = "narrowToWideP";
	
	if(JDA::Utils::debug){	
		wcout << sWho << "(): How many bytes do we need...?" << endl;
	}

	size_t iNumWideCharsNeeded;

    // Call the first time with NULL supplied for buffer to figure out how many bytes we'll need...
    iReturnCode = mbstowcs_s(&iNumWideCharsNeeded, NULL, (size_t)0, 
               input, _TRUNCATE);

    if( iReturnCode != 0 )
	{
    	wcerr << sWho << "(): Error on first call to mbstowcs_s(): " << iReturnCode << endl;
		return NULL;
	}

	if(JDA::Utils::debug){	
    	wcout << sWho << "(): We need " << iNumWideCharsNeeded << " wide chars..." << endl;
	}

	size_t iNumBytesNeeded = iNumWideCharsNeeded * sizeof( wchar_t );

	if(JDA::Utils::debug){	
    	wcout << sWho << "(): We need " << iNumBytesNeeded << " bytes..." << endl;
	}

    wchar_t* pWCBuffer = (wchar_t *)malloc( iNumBytesNeeded );

	if(JDA::Utils::debug){	
    	wcout << sWho << "(): Doing conversion..." << endl;
	}

    // Conversion
    iReturnCode = mbstowcs_s(&iReturnValue, pWCBuffer, iNumWideCharsNeeded, 
               input, iNumWideCharsNeeded);

    if( iReturnCode != 0 )
	{
    	wcerr << sWho << "(): Error on second call to mbstowcs_s(): " << iReturnCode << endl;
		return NULL;
	}

    
	if(JDA::Utils::debug){	
    	wcout << sWho << "(): Number of Characters converted: " << iReturnValue << endl;
	    wcout << sWho << "(): wide character string: \"" << pWCBuffer << "\"..." << endl;
	}	

	return pWCBuffer;

}/* wchar_t* narrowToWideP(const wchar_t* input) */



static wstring JDA::Utils::narrowToWideS(const string& input){

	wchar_t* pWCBuffer = JDA::Utils::narrowToWideP((const char*)input.c_str());

	if( pWCBuffer != NULL ){
		wstring sOut(pWCBuffer);

		free(pWCBuffer);

		return sOut;
	}
	else {
		return wstring(L"");
	}

}/* wstring narrowToWideS(const string& input) */ 


/**
* Ascertain the path to where this executable is based
* via Windows-specific GetModuleFileName()...e.g., e:\xpp\xz\bin\spawner.exe
*
* @return path to this executable or "" if cannot find it.
*
* @see http://stackoverflow.com/questions/1528298/get-path-of-executable 
*/
//static std::basic_string<TCHAR> getExecutablePath(){
static std::string getExecutablePath(){

	std::string sWho = "getExecutablePath";

	char ownPth[MAX_PATH];
    //wchar_t ownPth[MAX_PATH]; 
	//TCHAR ownPth[MAX_PATH]; 
	std::string sOut="";

    // Will contain exe path
    HMODULE hModule = GetModuleHandle(NULL);
    if (hModule != NULL)
    {
		// When passing NULL to GetModuleHandle, it returns handle of exe itself
		GetModuleFileNameA(hModule, ownPth, (sizeof(ownPth))); 

		// Use above module handle to get the path using GetModuleFileName()
		//JDA::Logger::printf(JDA::Logger::DEBUG, "%s(): ownPth='%s'\n", sWho.c_str(), ownPth );
		//cout << ownPth << endl ;

		//sOut = JDA::Utils::wideToNarrowS(ownPth);	
		sOut = ownPth;
    }
    else
    {
		//cout << "Module handle is NULL" << endl ;
		//JDA::Logger::printf(0, "%s(): Module handle is NULL, cannot determine ownPath...\n", sWho.c_str() );
		//sOut = __T("");
		sOut = "";
    }

	return sOut;

}/* getExecutablePath() */

/** Replaces whatever the path of this executable is with <executable_name>.ini, e.g.
* getExecPath( "c:\joe\bin\cap.exe" ) returns "c:\joe\bin\cap.ini".
* getExecPath( "c:\joe\bin\cap" ) returns "c:\joe\bin\cap.ini".
*/
static std::string getDefaultConfigFilePath( const string& executablePath ){

	string sWho = "getDefaultConfigFilePath";

	string sOut = executablePath;

	size_t found;

	found = sOut.rfind(".exe");

	if( found != std::string::npos ){
		sOut.replace( found, 4, ".ini");
	}
	else {
		sOut = sOut + ".ini";
	}

	return sOut;

}/* getDefaultConfigFilePath( string& executablePath ) */

/**
* Replaces whatever the path of this executable is with <executable_name><suffix>
*
* Examples:
*
* getDefaultLogFilePath( "c:\joe\bin\cap.exe" ) returns "c:\joe\bin\cap.log".
* getDefaultLogFilePath( "c:\joe\bin\cap" ) returns "c:\joe\bin\cap.log".
*
* getDefaultLogFilePath( "c:\joe\bin\cap.exe", ".2014-01-28.log" ) returns "c:\joe\bin\cap.2014-01-28.log".
* getDefaultLogFilePath( "c:\joe\bin\cap", ".2014-01-28.log" ) returns "c:\joe\bin\cap.2014-01-18.log".
*/
static std::string getDefaultLogFilePath( const string& executablePath, const string& suffix = ".log" ){

	string sWho = "getDefaultLogFilePath";

	string sOut = executablePath;

	size_t found;

	found = sOut.rfind(".exe");

	if( found != std::string::npos ){
		//sOut.replace( found, 4, ".log");
		sOut.replace( found, suffix.length(), suffix );
	}
	else {
		//sOut = sOut + ".log";
		sOut = sOut + suffix;
	}

	return sOut;

}/* getDefaultLogFilePath( string& executablePath ) */


/**
* Case insensitive string equality. 
*/
static bool iequals(const string& a, const string& b)
{
    unsigned int sz = a.size();

    if (b.size() != sz){
        return false;
	}

    for (unsigned int i = 0; i < sz; ++i){
        if (tolower(a[i]) != tolower(b[i])){
            return false;
		}
	}

    return true;

}/* iequals() */

/**
* Case insensitive string compare.
*/
static int icompare(const string& a, const string& b)
{
	string a_upper = a;
	JDA::Utils::toUpper( a_upper );

	string b_upper = b;
	JDA::Utils::toUpper( b_upper );

	return a.compare(b);	

}/* icompare() */

/**
* Are string a and string b equal if compared numerically?
*/
static bool num_equals(const string& a, const string& b)
{
	int a_int;
	bool a_int_numerical;
	if( sscanf_s( a.c_str(), "%d", &a_int ) ){ 
		a_int_numerical = true;
	}
	else {
		a_int_numerical = false;
	}

	int b_int;
	bool b_int_numerical;
	if( sscanf_s( b.c_str(), "%d", &b_int ) ){ 
		b_int_numerical = true;
	}
	else {
		b_int_numerical = false;
	}

	if( ! a_int_numerical || ! b_int_numerical ){
		return false;
	}

	return (a_int == b_int);

}/* num_equals() */


/** Case-insensitive string find...
* As with string::find(), will return 
* string::npos if "contact_lense" not found in "swimming_pool".
*/
static size_t ifind( const string& swimming_pool, const string& contact_lense ){

	string contact_lense_upper = contact_lense;				
	JDA::Utils::toUpper(contact_lense_upper);

	string swimming_pool_upper = swimming_pool;				
	JDA::Utils::toUpper(swimming_pool_upper);

	return swimming_pool_upper.find( contact_lense_upper ); 
}/* ifind() */

static string& toLower(string& input){

	size_t sz = input.size();

	for(size_t i = 0 ; i<sz; i++ ){
		input[i] = tolower(input[i]);
	}

	return input;
}/* toLower() */

static string& toUpper(string& input){

	size_t sz = input.size();

	for(size_t i = 0 ; i<sz; i++ ){
		input[i] = toupper(input[i]);
	}

	return input;
}/* toUpper() */


static bool stringToBool( const string& sInput ){ 

	string sInputTrimmed = sInput;

	JDA::Utils::trim( sInputTrimmed );
	
	if( JDA::Utils::iequals( sInputTrimmed, "true" ) 
				||
		 sInputTrimmed.compare("1") == 0
							){
		return true;
	}
	else {
		return false;
	}
}/* stringToBool */

static string boolToString( const bool& bInput ){ 

	if( bInput ){
		return "true";
	}
	else {
		return "false";
	}

}/* boolToString() */

static int boolToInt( const bool& bInput ){ 

	if( bInput ){
		return 1;
	}
	else {
		return 0;
	}

}/* boolToInt() */

#if 0
template<class InputIterator, class T>
  InputIterator find (InputIterator first, InputIterator last, const T& val)
{
  while (first!=last) {
    if (*first==val) return first;
    ++first;
  }
  return last;
}
#endif

template<class T> static int findIndex(const vector<T>& vec, const T& val)
{
	std::vector<T>::const_iterator iter;

	iter = std::find(
				vec.begin(),
				vec.end(),
			    val
	);

	if( iter == vec.end() ){
		return -1;
	}
	else {
		return (int) std::distance( vec.begin(), iter );
	}

}/* findIndex() */

/**
* @throws JDA::Utils::Exception if cannot parse sInput into an integer.
*/
static int stringToInt( const string& sInput ){

	int iOutput;

	if( sscanf_s( sInput.c_str(), "%d", &iOutput) == 1 ){
		return iOutput;
	}
	else {

		ostringstream oss_msg;
		oss_msg  << "Cannot parse input '" << sInput << "' to an integer";

		throw JDA::Utils::Exception( oss_msg.str() );
	}

}/* stringToInt() */

/**
* @return
*
*   true if parse succeeded, in which case int pointed to by piOutput
*    will be filled in with the int value.
*
*   false if parse failed, in which case int pointed to by piOutput 
*    will be left unchanged.
*/
static bool stringToInt( const string& sInput, int* piOutput ){

	int iTemp;

	if( sscanf_s( sInput.c_str(), "%d", &iTemp ) == 1 ){
		*piOutput = iTemp;
		return true;
	}
	else {
		return false;
	}

}/* stringToInt() */

/** @returns
* A. sInput as an integer if parsing into integer succeeds.
* B. iDefault if can't parse sInput into an integer.
*/
static int stringToInt( const string& sInput, int iDefault ){

	int iTemp;

	if( sscanf_s( sInput.c_str(), "%d", &iTemp ) == 1 ){
		return iTemp;
	}
	else {
		return iDefault;
	}
}

/** Returns string that looks something like "0xA034FE34"... */
static string intToHex( int iInput, int iWidth = 8, const string& sSuffix="0x"  ){

	ostringstream oss_hex;

	if( iWidth > 0 ){
		oss_hex << setfill('0') << setw(iWidth) << std::hex << iInput;
	}
	else {
		oss_hex << std::hex << iInput;
	}

	string s_hex = oss_hex.str();
	JDA::Utils::toUpper(s_hex);

	s_hex = sSuffix + s_hex;	

	return s_hex;

}/* intToHex */

/** Returns input as decimal string... */
static string intToString( int iInput ){

	ostringstream oss;

	oss << iInput;

	return oss.str();

}/* intToString */

/**
* If finds a file-path-separator, returns contents to the right of that separator.
* If finds no file-path separator, returns input unchanged.
* If finds a file-path separator, but nothing to the right of it, returns empty string "".
*
* e.g., baseFileName(".\\joe\\bin\\joe.txt") returns "joe.txt".
*       baseFileName("./joe/bin/joe.txt") returns "joe.txt".
*       baseFileName(".\\joe.txt") returns "joe.txt".
*       baseFileName("./joe.txt") returns "joe.txt".
*
*       baseFileName("joe.txt") returns "joe.txt".
*       baseFileName("moe") returns "moe"
*
*       baseFileName("./joe/bin/") returns "".
*       baseFileName(".\\joe\\bin\\") returns "".
*/
static string baseFileName( const string& sInput ){ 

	size_t len = sInput.length();

    size_t pos = sInput.rfind("/"); 

	if( pos == string::npos ){
    	pos = sInput.rfind("\\"); 
	}

	if( pos != string::npos ){
		if( pos + 1 < len ){ 
			return sInput.substr( pos + 1 );
		}
		else {
			return "";
		}
	}

	return sInput;

}/* baseFileName() */


static string baseDir( const string& sInput ){ 

	//size_t len = sInput.length();

    size_t pos = sInput.rfind("/"); 

	if( pos == string::npos ){
    	pos = sInput.rfind("\\"); 
	}

	if( pos != string::npos ){
		return sInput.substr( 0, pos );
	}
	else {
		if( sInput.compare(".") == 0 || sInput.compare("..") == 0 ){
			return sInput;
		}
		else{
			return ".";
		}
	}

}/* baseDir() */

/** 
* @source http://stackoverflow.com/questions/4403986/c-which-is-the-best-method-of-checking-for-file-existence-on-windows-platform
*
* @note uses Windows API
*/
//static bool fileExists(const TCHAR *fileName)
static bool fileExists(const char* fileName)
{
	// windows-specific functionality...
    DWORD       fileAttr;

    //fileAttr = GetFileAttributes(fileName); 
  	fileAttr = GetFileAttributesA(fileName);

    if (0xFFFFFFFF == fileAttr && GetLastError()==ERROR_FILE_NOT_FOUND){
        return false;
	}
	else {
		return true;
	}

}/* fileExists() */

/**
* @source http://stackoverflow.com/questions/8233842/how-to-check-if-directory-exist-using-c-and-winapi
*
* @note uses Windows API
*/
static bool dirExists(const std::string& dirName_in)
//static bool dirExists(const TCHAR* dirName_in)
{
  DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
  //DWORD ftyp = GetFileAttributes( dirName_in );

  if (ftyp == INVALID_FILE_ATTRIBUTES){
    return false;  //something is wrong with your path!
  }

  if (ftyp & FILE_ATTRIBUTE_DIRECTORY){
    return true;   // this is a directory!
  }

  return false;    // this is not a directory!

}/* dirExists() */

/**
* @source (based on) http://stackoverflow.com/questions/8991192/check-filesize-without-opening-file-in-c
*
* @note uses Windows API
*
* @throws JDA::Utils::Exception() if has difficulty determining file attributes. 
*/
#if 0
static __int64 getFileSize(const std::wstring& name_wide )
{
    WIN32_FILE_ATTRIBUTE_DATA fad;

    if (!GetFileAttributesEx(name_wide.c_str(), GetFileExInfoStandard, &fad))
	{
		DWORD lastError = GetLastError();
		string s_error = JDA::Utils::strerror( lastError );

		std::string name_narrow = JDA::Utils::wideToNarrowS( name_wide );	

		ostringstream oss_msg;
		oss_msg  << "Trouble with GetFileAttributesEx( '" << name_narrow << "' ): "
			<< lastError << ": \"" << s_error << "\"";

		throw JDA::Utils::Exception( oss_msg.str().c_str() );
	}

    LARGE_INTEGER size;
    size.HighPart = fad.nFileSizeHigh;
    size.LowPart = fad.nFileSizeLow;

    return size.QuadPart;
}
#endif

/**
* @source (based on) http://stackoverflow.com/questions/8991192/check-filesize-without-opening-file-in-c
*
* @note uses Windows API
*
* @throws JDA::Utils::Exception() if has difficulty determining file attributes. 
*/
static __int64 getFileSize(const std::string& name_narrow)
{
	//std::wstring name_wide = JDA::Utils::narrowToWideS( name_narrow );	

    WIN32_FILE_ATTRIBUTE_DATA fad;

    if (!GetFileAttributesExA(name_narrow.c_str(), GetFileExInfoStandard, &fad))
	{
		DWORD lastError = GetLastError();
		string s_error = JDA::Utils::strerror( lastError );

		ostringstream oss_msg;
		oss_msg  << "Trouble with GetFileAttributesEx( '" << name_narrow << "' ): "
			<< lastError << ": \"" << s_error << "\"";

		throw JDA::Utils::Exception( oss_msg.str().c_str() );
	}

    LARGE_INTEGER size;
    size.HighPart = fad.nFileSizeHigh;
    size.LowPart = fad.nFileSizeLow;

    return size.QuadPart;
}


///* For numberFormatWithCommas()... */
//    struct Numpunct: public std::numpunct<char>{
//    	protected:
//       	 virtual char do_thousands_sep() const{return ',';}
//       	 virtual std::string do_grouping() const{return "\03";}
//    };

/** SOURCE: http://stackoverflow.com/questions/7276826/c-format-number-with-commas */
//template<class T> static std::string numberFormatWithCommas(T value, size_t precision = 0 ){
//
//    std::stringstream ss;
//
//	Numpunct numpunct;
//
//    //ss.imbue({std::locale(), new Numpunct});
//    ss.imbue(std::locale(), &numpunct);
//
//    ss << std::setprecision(precision) << std::fixed << value;
//
//    return ss.str();
//}

template <class T> static string padTo(T foo, const size_t num, const char paddingChar = ' '){

	std::ostringstream oss;

	oss << setw( num );

	oss << foo;

	return oss.str();

}/* template <class T> static string padTo(T foo, const size_t num, const char paddingChar = ' ') */

template <class T> static string commify(T value, int precision = 0){

	std::ostringstream ss1;

	ss1 << fixed << showpoint; // Prevent displaying in scientific notation...

	if( precision >= 0 ){
		ss1 << setprecision(precision);
	}

	ss1 << value;

	//const char* sWho = "JDA::Utils::commify";
	//cout << sWho << "(): value=" << value << ", precision = " << precision << ", ss1='" << ss1.str() << "'..." << endl;

	size_t len = ss1.str().length();

    size_t dotPos = ss1.str().find(".");

	string sOut = "";

	int count = 0;
	/* Go from end of string, prepend to output string, insert commas every three if left of the decimal point... */
	/* NOTE: It may be more efficient to append to an ostrstream then reverse it, or maybe there's a way to set
	* an ostrstream to "Hebrew" mode so it prepends when you use the << operator...?
	*/
	for( size_t i = len-1 ; ; i-- ){

		if( dotPos == string::npos || i < dotPos ){	
			/* No decimal or we're to the left of it... */
			count++;
		}

		if( count > 1 && (count-1) % 3 == 0 && isdigit( ss1.str()[i]) ){
			sOut = "," + sOut; /* Prepend a comma, but only if this is a digit, not if it's a "-" or "+" sign, for example... */
		}

		sOut = ss1.str()[i] + sOut; /* Prepend to output... */
	
		if( i == 0 ){
			// Important!  Cannot use i >= 0 as loop condition
			// because we're using size_t which is unsigned, and
			// goes to UNSIGNED_MAX or something if decremented
			// from 0...causing us to access ss1.str()[UINT_MAX]
			// ...el crasho!
			break;
		}
		
	}

	return sOut;

}/* template <class T> static string commify(T value, size_t precision = 0) */


/** @throws JDA::Utils::Exception() */
static void writeStringToFile( const string& sInput, const string& sFilePath, bool binary = false ){

	FILE* filePtr = NULL;

	string sMode = "w";
	if( binary ){
		sMode = "wb";
	}

	//filePtr = fopen_s ( sCapturesFilePath.c_str(), "w" );
	errno_t lastError = fopen_s ( &filePtr, sFilePath.c_str(), sMode.c_str() );

	if( lastError != 0 ){

		const int err_buf_sz = 256;
		char sz_err_buf[err_buf_sz];
		strerror_s(sz_err_buf, err_buf_sz, lastError );

		ostringstream oss_msg;
		oss_msg  << "Trouble opening '" << sFilePath << "' for writing: " 
				<< lastError << ": \"" << sz_err_buf << "\"";

		//JDA::Logger::log(JDA::Logger::ERR) << sWho << "(): " << oss_msg.str() << ", tossing exception..." << endl;

		throw JDA::Utils::Exception( oss_msg.str().c_str() );
	}

	fputs( sInput.c_str(), filePtr ); 

	fclose( filePtr );

}/* void writeStringToFile() */

/** @throws JDA::Utils::Exception() */
static string slurpStringFromFile( const string& sFilePath, bool binary = false ){

	FILE* filePtr = NULL;

	string sMode = "r";
	if( binary ){
		sMode = "rb";
	}

	//filePtr = fopen_s ( sCapturesFilePath.c_str(), "r" );
	errno_t lastError = fopen_s ( &filePtr, sFilePath.c_str(), sMode.c_str() );

	if( lastError != 0 ){

		string sLastError = JDA::Utils::strerror(lastError);

		ostringstream oss_msg;
		oss_msg  << "Trouble opening '" << sFilePath << "' for reading: " 
				<< lastError << ": \"" << sLastError << "\"";

		throw JDA::Utils::Exception( oss_msg.str().c_str() );
	}

	const int BUF_SZ = 257;

	char buf[BUF_SZ];

	ostringstream oss_out;

	// size_t fread ( void * ptr, size_t size, size_t count, FILE * stream );

	size_t numRead;
	do {
		numRead = fread( buf, sizeof(char), BUF_SZ-1, filePtr );
		buf[numRead] = '\0';
		oss_out << buf;	
	} while( numRead == BUF_SZ-1 );

	fclose( filePtr );

	return oss_out.str();

}/* void slurpStringFromFile() */

/** Uses Windows-specific secure functions fopen_s (and strerror_s in case of error),
* and tosses JDA::Utils::Exception if unsuccessful...
*/
static FILE* fopen( const char *filename, const char *mode ){

		FILE* filePtr = NULL;

		errno_t lastError = fopen_s( &filePtr, filename, mode );
	
		if( lastError != 0 ){

			const int err_buf_sz = 256;
			char sz_err_buf[err_buf_sz];
			strerror_s(sz_err_buf, err_buf_sz, lastError );

			ostringstream oss_msg;
			oss_msg  << "Trouble opening '" << filename << "' for writing: " 
					<< lastError << ": \"" << sz_err_buf << "\"";
	
			//JDA::Logger::log(JDA::Logger::ERROR) << sWho << "(): " << oss_msg.str() << ", tossing exception..." << endl;
	
			throw JDA::Utils::Exception( oss_msg.str().c_str() );
		}

		return filePtr;
}


//static basic_string<TCHAR> getenv( const basic_string<TCHAR>& varname, const basic_string<TCHAR>& default="")
static string getenv( const string& varname, const string& default="")
{

//errno_t _dupenv_s(
//  char **buffer,
//   size_t *numberOfElements,
//   const char *varname
//);
//
	char* pBuf;
	//TCHAR* pBuf;

	size_t len;

	//errno_t err = _tdupenv_s( &pBuf, &len, varname.c_str() );
	errno_t err = _dupenv_s( &pBuf, &len, varname.c_str() );

	if( err ){
		return default;
	}
	else {
		//basic_string<TCHAR> sOut = pBuf;
		string sOut = pBuf;

		/* _dupenv_s allocates pBuf for you...so you need to free it if successful... */
		free(pBuf);

		return sOut;
	}

}/* getenv() */

static bool setenv( const string& varname, const string& varvalue )
{
	ostringstream oss_setenv;

	oss_setenv << varname << "=" << varvalue;

	int i_return_code = _putenv( oss_setenv.str().c_str() );

	if( i_return_code == 0 ){
		// success
		return true;
	}
	else if( i_return_code == -1 ){
		// failure
		return false;
	}

	// Shouldn't happen, but return false...
	return false;

}/* setenv() */

static string getUserName(){
	return JDA::Utils::getenv("USERNAME", "unknown.user");
}

/* Returns an error string based on GetLastError(). */
static string s_error(){

	DWORD lastError = GetLastError();

	const int err_buf_sz = 256;
	char err_buf[err_buf_sz];

	strerror_s(err_buf, err_buf_sz, lastError);

	ostringstream oss;
	oss << lastError << ": \"" << err_buf << "\"";

	return oss.str();
}

/**
* Returns the strerror_s() output for lastError,
* which you probably obtained from GetLastError()
* or maybe from global variable errno.
*/ 
static string strerror( DWORD lastError ){

	const int err_buf_sz = 256;
	char err_buf[err_buf_sz];

	strerror_s(err_buf, err_buf_sz, lastError);

	//ostringstream oss;
	//oss << lastError << ": \"" << err_buf << "\"";

	//return oss.str();

	string sOut = err_buf;

	return sOut;

}/* strerror() */

/**
* Ported from ./Modules/JDA/Utils.pm Perl module...
*
* Returns ordinal suffix for numerical input, e.g., 
* 
*  get_ordinal_suffix(0) = "th";
*  get_ordinal_suffix(1) = "st";
*  get_ordinal_suffix(2) = "nd";
*  get_ordinal_suffix(3) = "rd";
*  get_ordinal_suffix(4) = "th";
*  get_ordinal_suffix(5) = "th";
*  get_ordinal_suffix(6) = "th";
*  get_ordinal_suffix(7) = "th";
*  get_ordinal_suffix(8) = "th";
*  get_ordinal_suffix(9) = "th";
*  get_ordinal_suffix(10) = "th";
*  get_ordinal_suffix(11) = "th";
*  get_ordinal_suffix(12) = "th";
*  get_ordinal_suffix(13) = "th";
*  get_ordinal_suffix(14) = "th";
*  get_ordinal_suffix(15) = "th";
*  get_ordinal_suffix(16) = "th";
*  get_ordinal_suffix(17) = "th";
*  get_ordinal_suffix(18) = "th";
*  get_ordinal_suffix(19) = "th";
*  get_ordinal_suffix(20) = "th";
*  get_ordinal_suffix(21) = "st";
*  get_ordinal_suffix(22) = "nd";
*  get_ordinal_suffix(23) = "rd";
*  get_ordinal_suffix(24) = "th";
*  get_ordinal_suffix(25) = "th";
*  get_ordinal_suffix(26) = "th";
*  get_ordinal_suffix(27) = "th";
*  get_ordinal_suffix(28) = "th";
*  get_ordinal_suffix(29) = "th";
*  get_ordinal_suffix(30) = "th";
*  ...
*  get_ordinal_suffix(100) = "th";
*  get_ordinal_suffix(101) = "st";
*  get_ordinal_suffix(102) = "nd";
*  get_ordinal_suffix(103) = "rd";
*  get_ordinal_suffix(104) = "th";
*  ...
*  get_ordinal_suffix(1000) = "th";
*  get_ordinal_suffix(1001) = "st";
*  get_ordinal_suffix(1002) = "nd";
*  get_ordinal_suffix(1003) = "rd";
*  get_ordinal_suffix(1004) = "th";
*  ...
*/
static string get_ordinal_suffix( int input ){

	if( input == 0 ){
		return "th";
	}
	else if( input == 1 ){
		return "st";
	}
	else if( input == 2 ){
		return "nd";
	}
	else if( input == 3 ){
		return "rd";
	}
	else if( input >= 4 && input <= 10 ){
		return "th";
	}
	else if( input >= 11 && input <= 19 ) {
		return "th";
	}

	ostringstream oss_input;

	oss_input << input;
	string s_input = oss_input.str();

	string s_last_digit = s_input.substr( s_input.length()-1, 1);
	string s_last_two_digits = s_input.substr( s_input.length()-2, 2);
	string s_first_of_last_two_digits = s_last_two_digits.substr( 0, 1 );

	if( s_first_of_last_two_digits[0] == '1' ){
		return "th";
	}

	if( s_last_digit[0] == '0' ){
		return "th";	
	}
	else if( s_last_digit[0] == '1' ){
		return "st";	
	}
	else if( s_last_digit[0] == '2' ){
		return "nd";	
	}
	else if( s_last_digit[0] == '3' ){
		return "rd";	
	}
	else if(
		s_last_digit[0] =='4'
		|| s_last_digit[0] == '5'
		|| s_last_digit[0] == '6'
		|| s_last_digit[0] == '7'
		|| s_last_digit[0] == '8'
		|| s_last_digit[0] == '9'
	){
		return "th";	
	}

	return "";

}/* get_ordinal_suffix() */


static bool military_to_civilian_hour( int militaryHourIn, int* pCivilianHourOut, string* pCivilianAmPmOut ){

	if( militaryHourIn >= 0 && militaryHourIn <= 11 ){
		*pCivilianHourOut = militaryHourIn;
		*pCivilianAmPmOut = "AM";	
		return true;
	}
	else if( militaryHourIn == 12 ){
		*pCivilianHourOut = militaryHourIn;
		*pCivilianAmPmOut = "PM";
		return true;
	}
	else if( militaryHourIn >= 13 && militaryHourIn <= 23 ){
		*pCivilianHourOut = militaryHourIn-12;
		*pCivilianAmPmOut = "PM";	
		return true;
	}
	else {
		*pCivilianAmPmOut = "??";	
		return false;
	}

}/* military_to_civilian_hour() */

template<typename T> static string to_binary_string(const T& t){  

	const char* sWho = "to_binary_string";

	ostringstream oss_out;

	size_t n_bytes = sizeof(T);
	char* raw_ptr((char*)(&t));

	int bit_count=0;

	for( int byte_num = n_bytes-1; byte_num >= 0; byte_num-- ){
		for(int bit_num = CHAR_BIT-1; bit_num >= 0; bit_num--){

			bit_count++;

			//cout << sWho << "(): byte_num = " << byte_num << ", bit_num = " << bit_num << ", bit_count = " << bit_count << "..." << endl;

			if( (bit_count > 1) && (((bit_count - 1) % 4 ) == 0 ) ){
				//cout << sWho << "(): 'ere...put in spacer, guv'nor..." << endl;
				oss_out << " ";
			}
			else {
				//cout << sWho << "(): 'ere...don't put in spacer, guv'nor..." << endl;
			}
			oss_out << (( (raw_ptr[byte_num]) >> bit_num ) & 1 );		

			//cout << sWho << "(): oss_out = \"" << oss_out.str() << "\"..." << endl;
		}
	}

	//cout << sWho << "(): FINAL: oss_out = \"" << oss_out.str() << "\"..." << endl;
	return oss_out.str();

}/* to_binary_string() */

template<typename T> static string to_hex_string(const T& t){  

	const char* sWho = "to_hex_string";

	ostringstream oss_out;

	size_t n_bytes = sizeof(T);

	char* raw_ptr((char*)(&t));
	unsigned int le_uint;

	for( int byte_num = n_bytes-1; byte_num >= 0; byte_num-- ){

		le_uint = (unsigned int) raw_ptr[byte_num];
		le_uint &= 0xFF;

		//cout << sWho << "(): raw_ptr[" << byte_num << "] = " << setfill('0') << setw(2) << std::hex << raw_ptr[byte_num] << endl;
		//cout << sWho << "(): le_uint[" << byte_num << "] = " << setfill('0') << setw(2) << std::hex << le_uint << endl;

		if( (unsigned int)byte_num < n_bytes-1 ){
			oss_out << " ";
		}

		oss_out << setfill('0') << setw(2) << std::hex << le_uint;
		//cout << sWho << "(): byte_num = " << byte_num << ": oss_out = \"" << oss_out.str() << "\"..." << endl;
	}

	//cout << sWho << "(): FINAL: oss_out = \"" << oss_out.str() << "\"..." << endl;

	string s_out = oss_out.str(); 
	JDA::Utils::toUpper( s_out );
	//cout << sWho << "(): FINAL: s_out = \"" << s_out << "\"..." << endl;

	return s_out;

}/* to_hex_string() */

static string stringMultiply( const string& sInput, int iFactor ){

	ostringstream oss_out;	

	for( int i = 1; i <= iFactor; i++ ){
		oss_out << sInput;
	}

	return oss_out.str();
}/* stringMultiply() */

/* Classic swap routine from CS 101... */
static void swap( int* x, int* y){
	int i_temp = *x;
	*x = *y; 
	*y = i_temp;
	return;
}

/** W.C. FIELDS: Pick a random number between i_min and i_max, my chickadee... */
static int randRanger( int i_min, int i_max ){

	if( i_min == i_max ){
		return i_min;
	}
	else {
		// Make sure i_min < i_max...
		if( i_min > i_max ){
			JDA::Utils::swap( &i_min, &i_max);
		}
		
		int i_diff = i_max - i_min;

		/* initialize a random seed using current timestamp: */
		srand ( (unsigned int) time(NULL));

		/* rand() returns a pseudo-random integral number in the range between 0 and RAND_MAX. */
		/* ( rand() % (i_diff+1) ) is a pseudo-random integer in the range 0 and i_diff  */
		int i_rand_ranger = i_min + ( rand() % (i_diff+1) );

		return i_rand_ranger;
	}

}/* randRanger() */

static string stringMapToString( const map<string, string>& map, const string& name = "map" ){ 
	ostringstream oss_out;

	oss_out << map.size() << " element" << (name.size()==1?"":"s") << (name.size()==0?".":":") << "\n";

	for(std::map<string,string>::const_iterator it = map.begin(); it != map.end(); it++) {
	    //s_key = it->first;
	    //s_value = it->second;
		oss_out << name << "{\"" <<  it->first << "} = \"" << it->second << "\"" << "\n"; 
	}

	return oss_out.str();
}/* stringMapToString( const map<string, string>& map, const string& name = "map" ) */

static string stringMapVectorToString( vector< map<string, string> >& vec, const string& name = "vec" ){ 

	ostringstream oss_out;
	oss_out.str(""); // clear it

	size_t i_len = vec.size();
	
	oss_out << name << ": " << i_len << " element" << (i_len==1?"":"s") << (i_len==0?".":":") << "\n";

	//string s_key;
	//string s_value;

	for( size_t i = 0; i < i_len; i++ ){

		oss_out << "\t" << name << "[" << i << "]:" << "\n"; 

		for(std::map<string,string>::iterator it = vec.at(i).begin(); it != vec.at(i).end(); it++) {
		    //s_key = it->first;
		    //s_value = it->second;
			oss_out << "\t" << "\t" << it->first << "=" << "'" << it->second << "'" << "\n"; 
		}
	}

	return oss_out.str();

}/* stringMapVectorToString() */

static string stringVectorToString( vector< string >& vec, const string& name = "vec" ){ 

	ostringstream oss_out;
	oss_out.str(""); // clear it

	size_t i_len = vec.size();
	
	oss_out << name << ": " << i_len << " element" << (i_len==1?"":"s") << (i_len==0?".":":") << "\n";

	for( size_t i = 0; i < i_len; i++ ){

		oss_out << "\t" << name << "[" << i << "] : \"" << vec[i] << "\"\n"; 

	}

	return oss_out.str();

}/* stringVectorToString() */

static long factorial(long n){
	if( n == 0 ){
		return 1;
	}
	else {
		return n * JDA::Utils::factorial(n-1); 
	}
}/* static long factorial(long n) */

static double dfactorial(long n){
	if( n == 0 ){
		return 1.0;
	}
	else {
		return (double)n * JDA::Utils::dfactorial(n-1); 
	}
}/* static double dfactorial(long n) */

static long pow(long base, long exponent ){

	if( exponent < 0 ){
		exponent = 0; 
	}

	if( exponent == 0 ){
		return 1;
	}
	else {
		return base * JDA::Utils::pow(base, exponent-1); 
	}
}/* static long pow(long base, long exponent ) */

static double dpow(double base, long exponent ){

	if( exponent < 0 ){
		exponent = 0; 
	}

	if( exponent == 0 ){
		return (double)1.0;
	}
	else {
		return base * JDA::Utils::dpow(base, exponent-1); 
	}
}/* static double dpow(long base, long exponent ) */

/* From Taylor series for arctan(x)
*
* PI = \sum_{n=0}^{n=\infty} 2^(n+1) * (n!)^2 / (2n+1)! = 2 + 2/3 + 4/15 + 4/35 + 16/315 + 16/693 + 32/3003 +...
*     
* see: https://en.wikipedia.org/wiki/Approximations_of_%CF%80
*/
#define PI_CALC 1
#if PI_CALC
static double PI_calculate (long n_max){

	const char* sWho = "JDA::Utils::PI_calculate";

	if( JDA::Utils::debug ){
		cout << sWho << "(): n_max =" << n_max << "..." << endl;
	}

	if( n_max < 0 ){
		n_max = 0;
	}

	if( JDA::Utils::debug ){
		cout << sWho << "(): After adjusting, n_max =" << n_max << "..." << endl;
	}

	long n;
	double a;
	double b;
	double numerator;
	double denominator;
	double term;
	double sum = 0.0;
	for( n = 0; n <= n_max; n++ ){

		if( JDA::Utils::debug ){
			cout << sWho << "(): n = " << n << "..." << endl;
		}

		a = JDA::Utils::dpow( (double)2, (long)(n+1) );

	   	b = JDA::Utils::dfactorial(n);
		b *= b;

		numerator = a * b;

	 	denominator = JDA::Utils::dfactorial( 2*n + 1 );

		term = numerator / denominator;

		sum += term;

		if( JDA::Utils::debug ){
			cout << sWho << "(): n = " << n << ": a * b / denominator = (" << a << " * " << b << ") / " << denominator << endl;
			cout << sWho << "(): n = " << n << ": numerator / denominator = " << numerator << " / " << denominator << endl;
			cout << sWho << "(): n = " << n << ": term = " << term << endl;
			cout << sWho << "(): n = " << n << ": sum = " << sum << endl;
		}
	}

	if( JDA::Utils::debug ){
		cout << sWho << "(): After exiting loop: n = " << n << ": sum = " << sum << ", returning sum..." << endl;
	}

	return sum;

}/* PI_calculate() */
#endif



}; /* class Utils */



/**
* Overloaded ostream insertion operator
* Converts from wchar_t to char
*
* Courtesy Dale Rogerson, Inside COM, Chapter 11 
*/
//ostream& operator<< ( ostream& os, const wchar_t* wsz )
//{
//	// Length of incoming string
//	int iLength = wcslen(wsz)+1 ;
//
//	// Allocate buffer for converted string.
//	char* psz = new char[iLength] ;
//
//	// Convert from wchar_t to char.
//	wcstombs(psz, wsz, iLength) ;
//
//	// Send it out.
//	os << psz ;
//
//	// cleanup
//	delete [] psz ;
//	return os ;
//}

} /* namespace JDA */

ostream& operator<<(ostream& s, const vector<string>& v);

template<class T> ostream& operator<<(ostream& s, vector<T>& v);

template<class K, class V> ostream& operator<<(ostream& s, map<K,V>& m) {

    s << "size=" << m.size() << "\n";

    int iCount = 0;
    for(map<K,V>::iterator theIterator = m.begin();
        theIterator != m.end();
        theIterator++)
    {
		s << " [" << (iCount++) << "] element{'" << theIterator->first << "'} = '" << theIterator->second << "'\n";
    }

    return s;

} /* template<class K, class V> ostream& operator<<(ostream& s, map<K,V>& m) */



#endif /* #ifndef _UTILS_H */


