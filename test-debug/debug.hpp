/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etk/types.hpp>

#pragma once

#include <elog/log.hpp>

namespace test {
	int32_t getLogId();
};

#define TEST_BASE(info,data) ELOG_BASE(test::getLogId(),info,data)

#define TEST_PRINT(data)         TEST_BASE(-1, data)
#define TEST_CRITICAL(data)      TEST_BASE(1, data)
#define TEST_ERROR(data)         TEST_BASE(2, data)
#define TEST_WARNING(data)       TEST_BASE(3, data)
#define TEST_INFO(data)          TEST_BASE(4, data)
#define TEST_DEBUG(data)         TEST_BASE(5, data)
#define TEST_VERBOSE(data)       TEST_BASE(6, data)
#define TEST_TODO(data)          TEST_BASE(4, "TODO : " << data)

#define TEST_HIDDEN(data)          do { } while(false)

#define TEST_ASSERT(cond,data) \
	do { \
		if (!(cond)) { \
			TEST_CRITICAL(data); \
			assert(!#cond); \
		} \
	} while (0)


