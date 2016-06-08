#include "FormsMeanCommon.h"

namespace JDA { 

	/* static */ const string FormsMeanCommon::APP_NAME = "forms-mean-daemon";

	/* static */ const Logger::DebugLevelType FormsMeanCommon::DEFAULT_DEBUG_LEVEL = Logger::INFO;
	/* static */ const string FormsMeanCommon::DEFAULT_DEBUG_LOG_FILE_PATH = "";
	/* static */ const bool FormsMeanCommon::DEFAULT_DEBUG_LOG_FILE_ON = true;
	/* static */ const bool FormsMeanCommon::DEFAULT_DEBUG_LOG_FILE_APPEND = true;
	/* static */ const bool FormsMeanCommon::DEFAULT_DEBUG_STDOUT_ON = true;

	/* static */ const int FormsMeanCommon::DEF_DAY_DOWNLOAD_SLEEP_TIME = 30; // seconds
	/* static */ const int FormsMeanCommon::DEF_EVENING_DOWNLOAD_SLEEP_TIME = 3; // seconds
	/* static */ const int FormsMeanCommon::DEF_WEEKEND_DOWNLOAD_SLEEP_TIME = 1; // seconds

	/* static */ const int FormsMeanCommon::DEF_EVENING_DOWNLOAD_START_HOUR = 21; /* 9:00 PM */
	/* static */ const int FormsMeanCommon::DEF_EVENING_DOWNLOAD_END_HOUR = 6; /* 6:00 AM */

	/* static */ const int FormsMeanCommon::DEFAULT_DAILY_INDEX_BACKFILL_DAYS = 1;
	/* static */ const int FormsMeanCommon::DEFAULT_DAILY_INDEX_RECHECK_INTERVAL = 300; // seconds
	/* static */ const int FormsMeanCommon::DEFAULT_DAILY_INDEX_RETRY_INTERVAL = 3600; // seconds
	/* static */ const int FormsMeanCommon::DEFAULT_DAILY_INDEX_MAX_ATTEMPTS = 10;

	/* static */ const bool FormsMeanCommon::DEFAULT_LOAD_DAILY_INDEXES = true;
	/* static */ const bool FormsMeanCommon::DEFAULT_LOAD_NEXT_EDGAR_FILING_HEADER = true;

} /* namespace JDA */
