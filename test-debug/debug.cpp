/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <test-debug/debug.hpp>

int32_t test::getLogId() {
	static int32_t g_val = elog::registerInstance("test");
	return g_val;
}

