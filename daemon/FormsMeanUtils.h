#ifndef _FORMS_MEAN_UTILS_H_
#define _FORMS_MEAN_UTILS_H_

#include <string>
#include <iostream>
#include <memory> // shared_ptr<>, etc.
#include <vector>
using namespace std;


#include "utils.h"
#include "logger.h"

namespace JDA { 

class FormsMeanUtils {

	public:

		enum DownloadWindowType { DAY, EVENING, WEEKEND }; 

		// Mirrors DownloadWindowType enum for easy conversion to std::string...
		static const char* downloadWindowTypeEnumStrings[];

		static string downloadWindowTypeToString( DownloadWindowType downloadWindowType ){
			const char* sWho = "::downloadWindowTypeToString";
			return downloadWindowTypeEnumStrings[ downloadWindowType ];
		}

		DownloadWindowType in_which_download_window(
			int evening_start_hour,
			int evening_end_hour,
			int hour = -1,
			int day_of_week = -1
		);

		/**
		* e.g.,
		* accessionNumberFromFilePath("edgar/data/1000275/0001214659-14-000507.txt") = "0001214659-14-000507"
		* accessionNumberFromFilePath("edgar/data--1000275--0001214659-14-000507.txt") = "0001214659-14-000507"
		* accessionNumberFromFilePath("0001214659-14-000507.txt") = "0001214659-14-000507"
		* accessionNumberFromFilePath("0001214659-14-000507.xml") = "0001214659-14-000507"
		* accessionNumberFromFilePath("0001214659-14-000507") = "0001214659-14-000507"
		*/	
		static string accessionNumberFromFilePath( const string& sFilePath );

}; /* class FormsMeanUtils */


} /* namespace JDA */


#endif /* #ifndef _FORMS_MEAN_UTILS_H_ */
