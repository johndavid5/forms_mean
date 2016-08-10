#ifndef _FORMS_MEAN_COMMON_H_
#define _FORMS_MEAN_COMMON_H_

#include <string>
#include <iostream>
#include <memory> // shared_ptr<>, etc.
#include <vector>
using namespace std;

#include "utils.h"
#include "logger.h"

namespace JDA { 

	class ParseException : std::exception {

		public:
			string errString;

		ParseException(const string& errString){
			this->errString = errString;
		}

		virtual ~ParseException() throw() {}

		virtual const char* what() const throw()
		{
			return this->errString.c_str();
		}

	}; /* class ParseException */

	class FormsMeanCommon {
	
		public:
			static const string APP_NAME;

			static const Logger::DebugLevelType DEFAULT_DEBUG_LEVEL;
			static const string DEFAULT_DEBUG_LOG_FILE_PATH;
			static const bool DEFAULT_DEBUG_LOG_FILE_ON;
			static const bool DEFAULT_DEBUG_LOG_FILE_APPEND;
			static const bool DEFAULT_DEBUG_STDOUT_ON;

			static const int DEF_DAY_DOWNLOAD_SLEEP_TIME;
			static const int DEF_EVENING_DOWNLOAD_SLEEP_TIME;
			static const int DEF_WEEKEND_DOWNLOAD_SLEEP_TIME;

			static const int DEF_EVENING_DOWNLOAD_START_HOUR;
			static const int DEF_EVENING_DOWNLOAD_END_HOUR;

			static const int DEFAULT_DAILY_INDEX_BACKFILL_DAYS;
			static const int DEFAULT_DAILY_INDEX_RECHECK_INTERVAL;
			static const int DEFAULT_DAILY_INDEX_RETRY_INTERVAL;
			static const int DEFAULT_DAILY_INDEX_MAX_ATTEMPTS;

			static const bool DEFAULT_LOAD_DAILY_INDEXES;
			static const bool DEFAULT_LOAD_NEXT_EDGAR_FILING_HEADER;

			static const int DEFAULT_DOWNLOAD_SLEEP_TIME;

	};


} /* namespace JDA */


#endif /* #ifndef _FORMS_MEAN_COMMON_H_ */
