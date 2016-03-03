#include "FormsMeanUtils.h"


void FormsMeanUtils::getConfig() {

	string sWho = "FormsMeanUtils::getConfig";

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
