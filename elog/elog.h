/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <string>
/**
 * @brief basic namespace of the elog library. (it might contain all the elog fuctions without macro)
 */
namespace elog {
	/**
	 * @brief Initialize elog
	 * @param[in] _argc Number of argument list
	 * @param[in] _argv List of arguments
	 */
	void init(int _argc, const char** _argv);
}
