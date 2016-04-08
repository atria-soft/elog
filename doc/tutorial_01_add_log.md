Elog Tutorial: Add some Log (using)                                {#elog_tutorial_01}
===================================

@tableofcontents

You might not use the log directly.
The reson is simple:
- It is designed to be replaced by an other log library.

This permit you to use custom log library just replacing Macro and basic functions


Declaring the list of macro                                         {#elog_tutorial_01_declaration}
===========================


**debug.h**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
#pragma once

#include <elog/log.h>

namespace appl {
	int32_t getLogId();
};
#define APPL_BASE(info,data) ELOG_BASE(appl::getLogId(),info,data)

#define APPL_PRINT(data)         APPL_BASE(-1, data)
#define APPL_CRITICAL(data)      APPL_BASE(1, data)
#define APPL_ERROR(data)         APPL_BASE(2, data)
#define APPL_WARNING(data)       APPL_BASE(3, data)
#ifdef DEBUG
	#define APPL_INFO(data)          APPL_BASE(4, data)
	#define APPL_DEBUG(data)         APPL_BASE(5, data)
	#define APPL_VERBOSE(data)       APPL_BASE(6, data)
	#define APPL_TODO(data)          APPL_BASE(4, "TODO : " << data)
#else
	#define APPL_INFO(data)          do { } while(false)
	#define APPL_DEBUG(data)         do { } while(false)
	#define APPL_VERBOSE(data)       do { } while(false)
	#define APPL_TODO(data)          do { } while(false)
#endif

#define APPL_ASSERT(cond,data) \
	do { \
		if (!(cond)) { \
			APPL_CRITICAL(data); \
			assert(!#cond); \
		} \
	} while (0)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**debug.cpp**

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
#include "debug.h"

int32_t appl::getLogId() {
	static int32_t g_val = elog::registerInstance("your application name");
	return g_val;
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- on your main application:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
#include <elog/elog.h>
int main(int _argc, const char *_argv[]) {
	elog::init(_argc, _argv);
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Using it                                                      {#elog_tutorial_01_using_it}
========

You just need to call the macro whe you want to add debug log:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
APPL_INFO("Hello, how are you?");
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Specification of logs
=====================

  - *_CRITICAL(***); This will log the data and asert just after (display backtrace if possible)
  - *_PRINT(***); display on console (can not be removed with the log-level)



