#include <map>
using namespace std;

#include <QApplication>
#include "mainwindow.h"

#include "logger.h"
#include "utils.h"

const char* DEFAULT_DOS_WINDOW = "false";
const char* DEFAULT_DEBUG_LEVEL = "INFO";

void getConfig( map<string, string>& leConfigMap, JDA::Logger& logger  );
void setUpLogger(map<string, string>& configMap, JDA::Logger& logger );

int main(int argc, char* argv[])
{
	const char* sWho = "main.cpp::main";

	for(int i=1; i < argc; i++){
		if( strcmp( argv[i], "-dos_window" ) == 0 ){
			JDA::Utils::redirectIOToConsole(); /* Create DOS window for debug output of STDOUT. */
		}
	}

	map<string, string> configMap;
	JDA::Logger logger;

	try{
		getConfig( configMap, logger );
		setUpLogger( configMap, logger );

		logger(JDA::Logger::INFO) << sWho << "(): argc = " << argc << "..." << endl;

		bool bDosWindow = JDA::Utils::stringToBool( configMap["dos_window"] );

		if( bDosWindow ){
			JDA::Utils::redirectIOToConsole(); /* Create DOS window for debugging. */
		}

		logger(JDA::Logger::INFO) << sWho << "(): configMap = " << configMap << "..." << endl;
		logger(JDA::Logger::INFO) << sWho << "(): bDosWindow = " << JDA::Utils::boolToString( bDosWindow ) << "..." << endl;

    	QApplication app(argc, argv);
	    MainWindow mainWindow(argc, argv, configMap, logger );
   		mainWindow.show();
    	return app.exec();

	} catch ( JDA::Utils::Exception& e) {
	
			logger(JDA::Logger::ERROR) << sWho << "(): Caught JDA::Utils::Exception: \"" 
				<< e.what() << "\"..." << endl;
	
	} catch ( std::exception& e) {
	
			logger(JDA::Logger::ERROR) << sWho << "(): Caught std::exception: \"" 
				<< e.what() << "\"..." << endl;
	
	} catch (...) {
	
			logger(JDA::Logger::ERROR) << sWho << "(): Caught an unknown exception." << endl;
	
	}/* catch ( JDA::Utils::Exception& e) */

	//QApplication app(argc, argv);
	//QLabel* label = new QLabel
	//(
	// "<h2 style=\"color: purple; border: 2px;\">Hello SEC Forms!</h2>\n"
	// "<h2 style=\"color: green; font-family: courier; border: 2px;\">Let off some steam, Bennett!</h2>\n"
	//);
	//label->show();
	//return app.exec();

}/* main() */

void getConfig( map<string, string>& leConfigMap, JDA::Logger& logger ){

	string sWho = "main.cpp::getConfig";

	string sExecutablePath = JDA::Utils::getExecutablePath(); 
	string sConfigFilePath = JDA::Utils::getDefaultConfigFilePath( sExecutablePath );

	/* Set to defaults first, then they can be overridden 
	* by settings in *.ini file.
	*/
	leConfigMap["dos_window"] = DEFAULT_DOS_WINDOW;
	leConfigMap["debug_level"] = DEFAULT_DEBUG_LEVEL;

	logger(JDA::Logger::INFO) << sWho << "(): sExecutablePath = '" << sExecutablePath << "'..." << endl;
	logger(JDA::Logger::INFO) << sWho << "(): sConfigFilePath = '" << sExecutablePath << "'..." << endl;

	logger(JDA::Logger::INFO) << sWho << "(): Reading config file..." << endl;

	try {
		JDA::Utils::read_config_file( sConfigFilePath, leConfigMap );
	}
	catch(JDA::Utils::Exception& e) {

		logger(JDA::Logger::WARN) << sWho << "(): Cannot read config file '" << sConfigFilePath << "': \"" << e.what() << "\", using defaults..." << endl;
	
	}

	logger(JDA::Logger::INFO) << sWho << "(): After reading config file, leConfigMap = " << leConfigMap << "..." << endl;
	
}/* void MainWindow::getConfig() */

void setUpLogger(map<string, string>& configMap, JDA::Logger& logger ){

	string sWho = "main.cpp::setUpLogger";

	string sConfigLogFilePath = configMap["log_file_path"];

	if( sConfigLogFilePath.length() > 0 ){
		// Take log file path verbatim, but replace "YYYY-MM-DD" with actual datestamp...
		// e.g., "C:\path\to\GfCompare.YYYY-MM-DD.log" becomes "C:\path\to\GfCompare.2014-07-21.log" if today is July 21st, 2014...
		string sDateTag = "YYYY-MM-DD";
		size_t found = sConfigLogFilePath.rfind( sDateTag );
		if( found != std::string::npos ){
			string sNycDateStamp = JDA::Utils::get_nyc_datestamp();
			sConfigLogFilePath.replace( found, sDateTag.length(), sNycDateStamp ); 		
		}
	}

	/* Default log file path should be <name_of_exe>.<yyyy-mm-dd>.log in same directory
	* that executable is found in, e.g.,
	* if executable is... 
	*   "c:\joe\bin\QaCompare.exe"
	* ...default log file path is...
	*   "c:\joe\bin\QaCompare.2014-01-28.log"
	*/
	string sExecutablePath = JDA::Utils::getExecutablePath(); 
	string sSuffix = "." +  JDA::Utils::get_nyc_datestamp() + ".log";
	string sDefaultLogFilePath = JDA::Utils::getDefaultLogFilePath( sExecutablePath, sSuffix );

	string sLogFilePath = sDefaultLogFilePath;

	if( sConfigLogFilePath.length() > 0 ){
		sLogFilePath = sConfigLogFilePath;
	}
	else {
		sLogFilePath = sDefaultLogFilePath;
	}


	logger.setDebugLevel( JDA::Logger::INFO );

	logger(JDA::Logger::INFO) << sWho << "(): First, I set JDA::Logger's debug level to JDA::Logger::INFO..." << endl;

	logger(JDA::Logger::INFO) << sWho << "(): configMap[\"debug_level\"] = " << configMap["debug_level"] << "..." << endl;
	JDA::Logger::DebugLevelType debugLevel = JDA::Logger::LevelNameToDebugLevel( configMap["debug_level"] );
	logger(JDA::Logger::INFO) << sWho << "(): debugLevel from configMap[\"debug_level\"] is " << JDA::Logger::getLevelName( debugLevel ) << "(" << debugLevel << ")..." << endl;

	if( debugLevel != JDA::Logger::NOTSET ){
		logger(JDA::Logger::INFO) << sWho << "(): Setting debug level to " << JDA::Logger::getLevelName( debugLevel ) << "(" << debugLevel << ")..." << endl;  
		logger.setDebugLevel( debugLevel );
	}
	else {
		logger(JDA::Logger::INFO) << sWho << "(): Doesn't look like a viable debug level, so not changing debug level..." << endl; 
	}

	logger.setLogFilePath( sLogFilePath );
	logger.setLogFileAppend( true );
	logger.setLogFileOn( true );

	logger(JDA::Logger::INFO) << sWho << "(): Prologue: sLogFilePath = '" << sLogFilePath << "'..." << endl;
	logger(JDA::Logger::INFO) << sWho << "(): Prologue: sDefaultLogFilePath = '" << sDefaultLogFilePath << "'..." << endl;
	logger(JDA::Logger::INFO) << sWho << "(): Prologue: sConfigLogFilePath = '" << sConfigLogFilePath << "'..." << endl;

}/* setUpLogger() */
