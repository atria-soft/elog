/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
//! [elog_sample_main_include]
#include "debug.hpp"
#include <elog/elog.hpp>
//! [elog_sample_main_include]
#include <iostream>

//! [elog_sample_main_callback]
//! [elog_sample_main_callback_DECLARATION]
static void myExternalLogCallback(const char* _libName, enum elog::level _level, int32_t _ligne, const char* _funcName, const char* _log) {
//! [elog_sample_main_callback_DECLARATION]
	switch(_level) {
		default:
			std::cout << "[?] ";
			break;
		case elog::level_print:
			std::cout << "[P] ";
			break;
		case elog::level_critical:
			std::cout << "[C] ";
			break;
		case elog::level_error:
			std::cout << "[E] ";
			break;
		case elog::level_warning:
			std::cout << "[W] ";
			break;
		case elog::level_info:
			std::cout << "[I] ";
			break;
		case elog::level_debug:
			std::cout << "[D] ";
			break;
		case elog::level_verbose:
			std::cout << "[V] ";
			break;
	}
	std::cout << _libName << " (" << _ligne << ") " << _funcName << " | " << _log << std::endl;
}
//! [elog_sample_main_callback]


//! [elog_sample_main]
/**
 * @brief Main of the program (This can be set in every case, but it is not used in Andoid...).
 * @param std IO
 * @return std IO
 */
//! [elog_sample_main_base]
int main(int _argc, const char *_argv[]) {
	// if you use etk/ewol/gale, elog init in contain in it.
	elog::init(_argc, _argv);
	//! [elog_sample_main_base]
	//! [elog_sample_main_log]
	APPL_VERBOSE("VERBOSE display");
	APPL_DEBUG("DEBUG display");
	APPL_INFO("INFO display");
	APPL_WARNING("WARNING display");
	APPL_ERROR("ERROR display");
	APPL_PRINT("PRINT display");
	//APPL_CRITICAL("CRITICAL display"); // Disable critical because it create an assert ...
	//! [elog_sample_main_log]
	
	// Change the global log level:
	elog::setLevel(elog::level_verbose);
	//! [elog_sample_main_callback_link]
	// Set a callback:
	elog::setCallbackLog(&myExternalLogCallback);
	//! [elog_sample_main_callback_link]
	
	// try again new logs:
	APPL_VERBOSE("VERBOSE display");
	APPL_DEBUG("DEBUG display");
	APPL_INFO("INFO display");
	APPL_WARNING("WARNING display");
	APPL_ERROR("ERROR display");
	APPL_PRINT("PRINT display");
	
	
	return 0;
}
//! [elog_sample_main]


