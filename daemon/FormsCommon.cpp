#include "FormsCommon.h"

namespace JDA { 

	/* static */ const string FormsCommon::APP_NAME = "forms-mean-daemon";

	/* static */ const Logger::DebugLevelType FormsCommon::DEFAULT_DEBUG_LEVEL = Logger::INFO;
	/* static */ const string FormsCommon::DEFAULT_DEBUG_LOG_FILE_PATH = "";
	/* static */ const bool FormsCommon::DEFAULT_DEBUG_LOG_FILE_ON = true;
	/* static */ const bool FormsCommon::DEFAULT_DEBUG_LOG_FILE_APPEND = true;
	/* static */ const bool FormsCommon::DEFAULT_DEBUG_STDOUT_ON = true;

	/* static */ const int FormsCommon::DEF_DAY_DOWNLOAD_SLEEP_TIME = 30; // seconds
	/* static */ const int FormsCommon::DEF_EVENING_DOWNLOAD_SLEEP_TIME = 3; // seconds
	/* static */ const int FormsCommon::DEF_WEEKEND_DOWNLOAD_SLEEP_TIME = 1; // seconds

	/* static */ const int FormsCommon::DEF_EVENING_DOWNLOAD_START_HOUR = 21; /* 9:00 PM */
	/* static */ const int FormsCommon::DEF_EVENING_DOWNLOAD_END_HOUR = 6; /* 6:00 AM */

	/* static */ const int FormsCommon::DEFAULT_DAILY_INDEX_BACKFILL_DAYS = 1;
	/* static */ const int FormsCommon::DEFAULT_DAILY_INDEX_RECHECK_INTERVAL = 300; // seconds
	/* static */ const int FormsCommon::DEFAULT_DAILY_INDEX_RETRY_INTERVAL = 3600; // seconds
	/* static */ const int FormsCommon::DEFAULT_DAILY_INDEX_MAX_ATTEMPTS = 10;

	/* static */ const bool FormsCommon::DEFAULT_LOAD_DAILY_INDEXES = true;
	/* static */ const bool FormsCommon::DEFAULT_LOAD_NEXT_EDGAR_FILING_HEADER = true;

	/* static */ const int FormsCommon::DEFAULT_DOWNLOAD_SLEEP_TIME = 60; // seconds

} /* namespace JDA */
