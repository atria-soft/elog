/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <elog/log.hpp>
#include <cassert>

namespace elog {
	/**
	 * @brief Get local id of the library
	 * @return Unique ID of the library
	 */
	int32_t getLogId();
};

#define ELOG_BASIC(info,data)    ELOG_BASE(elog::getLogId(),info,data)

#define ELOG_PRINT(data)         ELOG_BASIC(-1, data)
#define ELOG_CRITICAL(data)      ELOG_BASIC(1, data)
#define ELOG_ERROR(data)         ELOG_BASIC(2, data)
#define ELOG_WARNING(data)       ELOG_BASIC(3, data)
#ifdef DEBUG
	#define ELOG_INFO(data)      ELOG_BASIC(4, data)
	#define ELOG_DEBUG(data)     ELOG_BASIC(5, data)
	#define ELOG_VERBOSE(data)   ELOG_BASIC(6, data)
	#define ELOG_TODO(data)      ELOG_BASIC(4, "TODO : " << data)
#else
	#define ELOG_INFO(data)      do { } while(false)
	#define ELOG_DEBUG(data)     do { } while(false)
	#define ELOG_VERBOSE(data)   do { } while(false)
	#define ELOG_TODO(data)      do { } while(false)
#endif

#define ELOG_HIDDEN(data)          do { } while(false)

#define ELOG_ASSERT(cond,data) \
	do { \
		if (!(cond)) { \
			ELOG_CRITICAL(data); \
			assert(!#cond); \
		} \
	} while (0)


