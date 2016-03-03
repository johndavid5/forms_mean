#ifndef _FORMS_MEAN_UTILS_H_
#define _FORMS_MEAN_UTILS_H_

#include <string>
#include <iostream>
#include <memory> // shared_ptr<>, etc.
#include <vector>
using namespace std;


#include "utils.h"
#include "logger.h"

#include "AdoDbClient.h"

#include "WaldoCommon.h"

namespace JDA { 

class FormsMeanUtils {

	public:

		void getConfig();

string getLogFilePath();

void setupLogger( JDA::Logger& kenny_loggins );

/**
* Keep setting logFilePath with current datestamp.
*
* Whereupon, at the crack of midnight, whence the datestamp changeth,
* JDA::Logger will by black witchcraft and deviltry beginneth a new logfileth.
*/
void resetLogFilePath();

enum DownloadWindowType { DAY, EVENING, WEEKEND }; 

// Mirrors DownloadWindowType enum for easy conversion to std::string...
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


}; /* class FormsMeanUtils */


} /* namespace JDA */


#endif /* #ifndef _FORMS_MEAN_UTILS_H_ */
