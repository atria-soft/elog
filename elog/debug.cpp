/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <elog/debug.h>

int32_t elog::getLogId() {
	static int32_t g_val = elog::registerInstance("elog");
	return g_val;
}

