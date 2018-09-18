/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/String.hpp>
/**
 * @brief basic namespace of the elog library. (it might contain all the elog fuctions without macro).
 */
namespace elog {
	/**
	 * @brief Initialize elog.
	 * @param[in] _argc Number of argument list.
	 * @param[in] _argv List of arguments.
	 * @param[in] _applName Current application Name.
	 */
	void init(int _argc, const char** _argv, const etk::String& _applName);
	/**
	 * @brief Un-Initialize elog.
	 */
	void unInit();
}

