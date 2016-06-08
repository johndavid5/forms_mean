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
			(void)sWho; /* unused...? who says...? */
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

		/** Right now blindly adds dashes to an 8 character input string, or if input is not 8 characters,
		* returns the input unchanged.  We may decide to add some error checking later.
		*
		* e.g.,
		* 	FormsMeanUtils::isoDateFromYyyyMmDd("20160606") return "2016-06-06"
		* 	FormsMeanUtils::isoDateFromYyyyMmDd("HELLOABC") return "HELL-OA-BC" -- since it doesn't check for numerical input.
		*   FormsMeanUtils::isoDateFromYyyyMmDd("201606") return "201606" -- or the original input, because the input string is not 8 characters...
		*/
		static string isoDateFromYyyyMmDd( const string& sYyyyMmDd );

}; /* class FormsMeanUtils */


} /* namespace JDA */


#endif /* #ifndef _FORMS_MEAN_UTILS_H_ */
