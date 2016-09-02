#ifndef _TESTS_COMMON_H_
#define _TESTS_COMMON_H_

#include "logger.h"

class TestsCommon {

	public:

		const static JDA::Logger::DebugLevelType DEFAULT_DEBUG_LEVEL;

		static JDA::Logger::DebugLevelType debug_level;

};

#endif  /* #ifndef _TESTS_COMMON_H_ */
