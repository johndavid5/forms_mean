#include "utils.h"

/* static */ int JDA::Utils::debug = 0;

/* static */ string JDA::Utils::s_nyc_posix_timezone = "EST-5EDT,M4.1.0,M10.5.0"; 
/* static */ string JDA::Utils::s_utc_posix_timezone = "UTC-0";

/* static */ 
	int JDA::Utils::read_config_file(const string& filePath,
					map<string, string>& confMap /* in-out */
	)
	{

	const string sWho = "JDA::Utils::read_config_file";

	//ifstream myFile;
	//FILE* myFile;

	if( JDA::Utils::debug ){
		cout << sWho << "(): Opening file '" << filePath << "' for reading..." << endl;
	}

	// Can't figure out how to get the shareflag set for C++ ifstream, as filebuf::sh_read doesn't seem
	// to be defined...so to Gehenna with C++ streams...
	//myFile.open( filePath.c_str(), ios::in, filebuf::sh_read );
	//FILE *_fsopen( const char *filename, const char *mode, int shflag );	

	/* Open file with shared access... */

	//#ifdef WIN32
	//	myFile = _fsopen( filePath.c_str(), "r", _SH_DENYNO );
	//#else
	//
	//	if( JDA::Utils::debug ){
	//		cout << sWho << "(): Calling myFile = fopen( \"" << filePath.c_str() << "\", \"r\")..." << endl;
	//	}
	//	myFile = fopen( filePath.c_str(), "r" );
	//#endif

	std::ifstream le_ifs;

	// T-Dogg: Turn on exceptions...live life dangerously for once...
	le_ifs.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

	try {
		if( JDA::Utils::debug ){
			cout << sWho << "(): " << "Opening filePath = '" << filePath << "' for reading..." << endl;
		}

		le_ifs.open( filePath.c_str(), std::ifstream::in );

	}catch( std::ifstream::failure e){

			ostringstream oss_error;

			oss_error << "Trouble opening filePath = '" << filePath << "' for reading: \"" << e.what() << "\", ";
			oss_error << "s_error = \"" << JDA::Utils::s_error() << "\"...";

			if( JDA::Utils::debug ){
				cout << sWho << "(): " << oss_error.str() << "\n" 
				<< "...tossin' a Utils::Exception..." << endl;
			}

			throw Utils::Exception( oss_error.str() );
	}

	string sLine;

	string sKey, sValue;
	int iReturn;

	int iLineCount = 0;
	int iKeyValCount = 0;

	while( !le_ifs.eof() && ! le_ifs.fail() ) {

		try {
			std::getline( le_ifs, sLine );
		} catch( std::ifstream::failure e){
			ostringstream oss_error;

			oss_error << "Trouble with getline(), filePath = '" << filePath << "': \"" << e.what() << "\"";

			if( JDA::Utils::debug ) {
				cout << sWho << "(): " << oss_error.str() << "\n"
				<< "SHEMP: Moe...Moe...I think this is just the end o' the file, so I'm gonna eat this exception, OK, Moe...?" << "\n"
				<< "MOE: Ya gotta eat it...it's part of the plot..." << endl;
			}

		}

		iLineCount++;

		if( JDA::Utils::debug ){
			cout << "sLine[" << iLineCount << "]: \"" << sLine << "\"" << endl;
		}

		iReturn = process_config_line( sLine, sKey, sValue);

		//cout << "From process_config_line(): iReturn = " << iReturn << ", sKey='" << sKey << "', sValue='" << sValue << "'..." << endl;
		//JDA::Logger::debugPrintf(2, "%s(): From process_config_line(): iReturn = %d, sKey = '%s', sValue = '%s'...\n"
		//	,sWho.c_str(), iReturn, sKey.c_str(), sValue.c_str()  
		//);

		if( iReturn != 0 ) {
			confMap[sKey] = sValue;
			iKeyValCount++;
		}

		iLineCount++;

	}/* while( !le_ifs.eof() && ! le_ifs.fail() ) */

	//if( JDA::Utils::debug ){
	//	cout << sWho << "(): Calling fclose( myFile )..." << endl;
	//}
	//fclose( myFile );

	if( JDA::Utils::debug ){
		cout << sWho << "(): Returning iKeyValCount = " << iKeyValCount << "..." << endl;
	}
	
	return iKeyValCount; 

}/* read_config_file() */

/* static */ int JDA::Utils::process_config_line( string sLine,
				string& sKey /* out */,
				string& sValue /* out */ ) {

	const string sWho = "process_config_line";

	size_t iSize = sLine.size();

	const int START = 1;
	const int IN_KEY = 2;
	const int IN_PRE_VALUE = 3;
	const int IN_VALUE = 4;
	const int IN_QUOTED_VALUE = 5;
	const int IN_COMMENT = 6;

	int iState = IN_KEY;
	char c;

	sKey = "";
	sValue = "";

	for( size_t i = 0; i < iSize && iState != IN_COMMENT; i++ ) {

		c = sLine[i];		

		//cout << "sLine[" << i << "] = '" << c << "'" << endl;	
		//JDA::Logger::debugPrintf(3, "%s(): sLine[%d] = '%c'...\n"
		//	,sWho.c_str(), i, c
		//);

		switch( c ) {
			case '=':
				if( iState == IN_KEY ) {
					iState = IN_PRE_VALUE;
					continue;
				}
				break;

			case '"': 
				if( iState == IN_PRE_VALUE ){
					iState = IN_QUOTED_VALUE;
					continue;
				}
				else if( iState == IN_QUOTED_VALUE ){
					iState = IN_VALUE;
					continue;
				}
				break;

			case ';': case '#':
				if( iState != IN_QUOTED_VALUE ){
					iState = IN_COMMENT;
				}
				break;

			default:
				if( ! isspace(c) ){ 
					if( iState == IN_PRE_VALUE ){
						iState = IN_VALUE;
					}
				}
				break;
		}

		switch( iState ) {
			case IN_KEY:
				sKey += c;			
				break;

			case IN_VALUE: case IN_QUOTED_VALUE:
				sValue += c;			
				break;
		}

	}/* for( size_t i = 0; i<iSize; i++ ) */

	JDA::Utils::trim( sKey );
	JDA::Utils::trim( sValue );

	if( sKey.size() > 0 ) {
		return 1;
	}
	else { 
		return 0;
	}

}/* process_config_line() */


/* This was the previous version...it doesn't have provision for quoting
* the value so it would completely garble something like...
*   db_url_unquoted = DRIVER=SQL Native Client;SERVER=PRNJDA-JAYNE\SQLExpress;Trusted_Connection=No;Initial Catalog=Waldo; 
* with the new version you can do...
*   db_url_unquoted = "DRIVER=SQL Native Client;SERVER=PRNJDA-JAYNE\SQLExpress;Trusted_Connection=No;Initial Catalog=Waldo;" ;This comment handled properly
*/
/* static */ int /* JDA::Utils::*/ process_config_line_alpha( string sLine,
				string& sKey /* out */,
				string& sValue /* out */ ) {

	const string sWho = "process_config_line_alpha";

	size_t iSize = sLine.size();

	const int START = 1;
	const int IN_KEY = 2;
	const int IN_VALUE = 3;
	const int IN_QUOTE = 4;
	const int IN_COMMENT = 5;

	int iState = IN_KEY;
	char c;

	sKey = "";
	sValue = "";

	for( size_t i = 0; i < iSize && iState != IN_COMMENT; i++ ) {

		c = sLine[i];		

		//cout << "sLine[" << i << "] = '" << c << "'" << endl;	
		//JDA::Logger::debugPrintf(3, "%s(): sLine[%d] = '%c'...\n"
		//	,sWho.c_str(), i, c
		//);

		switch( c ) {
			case '=':
				if( iState == IN_KEY ) {
					iState = IN_VALUE;
					continue;
				}
				break;

			case ';': case '#':
				iState = IN_COMMENT;
				break;
		}

		switch( iState ) {
			case IN_KEY:
				sKey += c;			
				break;

			case IN_VALUE:
				sValue += c;			
				break;
		}

	}/* for( size_t i = 0; i<iSize; i++ ) */

	JDA::Utils::trim( sKey );
	JDA::Utils::trim( sValue );

	if( sKey.size() > 0 ) {
		return 1;
	}
	else { 
		return 0;
	}

}/* process_config_line_alpha() */


/** 
* Run this if you want to create and read and write
* to a command-prompt via stdin and stdout even if you're
* in a Win32 GUI program.
*
* ORIGINAL SOURCE courtesy Andrew Tucker: http://dslweb.nwnexus.com/~ast/dload/guicon.htm
*/
#ifdef WIN32
/* static */ void JDA::Utils::redirectIOToConsole()
{
	int hConHandle;
	long lStdHandle;

	CONSOLE_SCREEN_BUFFER_INFO coninfo;

	FILE *fp;
 
	// allocate a console for this app
	AllocConsole();
 
	// set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);

	coninfo.dwSize.Y = MAX_CONSOLE_LINES;

	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);
 
	// redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen( hConHandle, "w" );

	*stdout = *fp;

	setvbuf( stdout, NULL, _IONBF, 0 );
 
	// redirect unbuffered STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);

	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen( hConHandle, "r" );

	*stdin = *fp;

	setvbuf( stdin, NULL, _IONBF, 0 );
 
	// redirect unbuffered STDERR to the console

	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);

	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen( hConHandle, "w" );

	*stderr = *fp;

	setvbuf( stderr, NULL, _IONBF, 0 );

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog 
	// point to console as well
	ios::sync_with_stdio();

}/* JDA::Utils::redirectIOToConsole() */
#endif /* #ifdef WIN32 */


			string JDA::Utils::Stopwatch::get_s_elapsed_seconds( int precision ){

				const char* sWho = "JDA::Utils::Stopwatch::get_s_elapsed_seconds";

				(void)sWho; // Unused?  Who says sWho is unused?

				//float elapsed_seconds = this->get_elapsed_seconds();

				//if( JDA::Utils::debug ){
				//	cout << sWho << "(): elapsed_seconds = " << elapsed_seconds << "..." << endl;
				//}

				//ostringstream oss_out;
				//oss_out << std::fixed << std::setprecision( precision ) << elapsed_seconds;

				//if( JDA::Utils::debug ){
				//	cout << sWho << "(): returning oss_out.str() = \"" << oss_out.str() << "\"..." << endl;
				//}

				//return oss_out.str();

				return JDA::Utils::commify( this->get_elapsed_seconds(), precision );

			}/* get_s_elapsed_seconds() */

ostream& operator<<(ostream& s, const vector<string>& v) {
    s << "size=" << v.size() << "\n";

    size_t i;
    for(i=0; i < v.size(); i++)
    {
		s << " element[" << i << "]='" << v[i] << "'\n";
    }

    return s;

}/* ostream& operator<<(ostream& s, vector<string>& v) */

#define _USE_ITERATORS_
template<class T> ostream& operator<<(ostream& s, vector<T>& v) {
    s << "size=" << v.size() << "\n";
    int i;
    #ifdef _USE_ITERATORS_
    i=0;
    for(typename vector<T>::iterator theIterator = v.begin();
        theIterator != v.end();
        theIterator++)
    {
		s << " element[" << (i++) << "]='" << (*theIterator) << "'\n";
    }
    #else
    for(i=0; i<v.size(); i++)
    {
		s << " element[" << i << "]='" << v[i] << "'\n";
    }
    #endif

    return s;

}/* template<class T> ostream& operator<<(ostream& s, vector<T>& v) */



