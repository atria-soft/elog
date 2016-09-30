/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#include "debug.hpp"

int32_t appl::getLogId() {
	static int32_t g_val = elog::registerInstance("your application name");
	return g_val;
}

