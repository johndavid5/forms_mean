#ifndef _W_LOGGER_H_

  #undef _T_LOGGER_WIDE_
  #undef _T_LOGGER_NARROW_

  #define _T_LOGGER_WIDE_

  #ifdef _T_LOGGER_WIDE_ 
	  #pragma message("wlogger.h: _T_LOGGER_WIDE_ is set...")
  #endif

  #ifdef _T_LOGGER_NARROW_ 
	  #pragma message("wlogger.h: _T_LOGGER_NARROW_ is set...")
  #endif
  
  #pragma message("wlogger.h: Pulling in tlogger.h...")

  #include "tlogger.h"

#endif
