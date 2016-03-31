/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <sstream>
#include <ostream>
#include <vector>

namespace elog {
	/**
	 * @brief Log level is a simple list of all log availlable. This enum is used when setting a log and when user chose the level of log displayed.
	 */
	enum level {
		level_print = -1, //!< basic print for Help or result (never filtered)
		level_none = 0, //!< no display requested
		level_critical = 1, //!< Display only critical logs (note that critical generally assert with a backtrace (when we can))
		level_error = 2, //!< Display Error and critical logs
		level_warning = 3, //!< Display log critical to warning
		level_info = 4, //!< Display log critical to information (removed in release mode)
		level_debug = 5, //!< Display log critical to debug (removed in release mode)
		level_verbose = 6 //!< Display all logs (removed in release and debug mode)
	};
	/**
	 * @brief Register an element in the log system
	 * @param[in] _name Name of the module
	 * @return reference Id of an instance name
	 */
	int32_t registerInstance(const std::string& _name);
	/**
	 * @brief Set the log level of a specific instance
	 * @param[in] _name Name of the intance
	 * @param[in] _level New level to set on the instance
	 */
	void setLevel(const std::string& _name, enum elog::level _level);
	/**
	 * @brief Set the log level of a specific instance
	 * @param[in] _id Id of the intance
	 * @param[in] _level New level to set on the instance
	 */
	void setLevel(int32_t _id, enum elog::level _level);
	/**
	 * @brief Set global debug level
	 * @param[in] _level New level to set on the instance
	 */
	void setLevel(enum elog::level _level);
	/**
	 * @brief Get the current level of debug for a specific intance
	 * @param[in] _id Id Of the intance
	 * @return the enum casted in an integer ==> generise the API (not dependent of etk)
	 */
	int32_t getLevel(int32_t _id);
	/**
	 * @brief Get list of all intance
	 * @return the name list of all intance
	 */
	std::vector<std::string> getListInstance();
	/**
	 * @brief Set Color enable or disable.
	 * @param[in] _status New value of color.
	 */
	void setColor(bool _status);
	/**
	 * @brief Set Time display enable or disable.
	 * @param[in] _status New value.
	 */
	void setTime(bool _status);
	/**
	 * @brief Set Line display enable or disable.
	 * @param[in] _status New value.
	 */
	void setLine(bool _status);
	/**
	 * @brief Set Function display enable or disable.
	 * @param[in] _status New value.
	 */
	void setFunction(bool _status);
	/**
	 * @brief Set thread id enable or disable.
	 * @param[in] _status New value.
	 */
	void setThreadId(bool _status);
	/**
	 * @brief Set thread name enable or disable.
	 * @param[in] _status New value.
	 */
	void setThreadNameEnable(bool _status);
	/**
	 * @brief Set library display enable or disable.
	 * @param[in] _status New value.
	 */
	void setLibName(bool _status);
	/**
	 * @brief Call log to display
	 * @param[in] _id Id of the instance type
	 * @param[in] _level Level debug
	 * @param[in] _ligne Line of the debug
	 * @param[in] _funcName Function name for debug (compleate decorate signature)
	 * @param[in] _log Stream to log
	 */
	void logChar(int32_t _id, int32_t _level, int32_t _ligne, const char* _funcName, const char* _log);
	/**
	 * @brief Call log to display
	 * @param[in] _id Id of the instance type
	 * @param[in] _level Level debug
	 * @param[in] _ligne Line of the debug
	 * @param[in] _funcName Function name for debug (compleate decorate signature)
	 * @param[in] _log Stream to log
	 */
	void logStream(int32_t _id, int32_t _level, int32_t _ligne, const char* _funcName, const std::ostream& _log);
	/**
	 * @brief Call log to display
	 * @param[in] _id Id of the instance type
	 * @param[in] _level Level debug
	 * @param[in] _log Stream to log
	 */
	void logChar1(int32_t _id, int32_t _level, const char* _log);
	/**
	 * @brief Call log to display
	 * @param[in] _id Id of the instance type
	 * @param[in] _level Level debug
	 * @param[in] _log Stream to log
	 */
	void logStream1(int32_t _id, int32_t _level, const std::ostream& _log);
	/**
	 * @brief Display the current backtrace
	 * @param[in] _breakAtEnd assert program when backtrace is printed
	 * @param[in] _removeElement Number of element remove in the stack before display (permit to remove log function call)
	 */
	void displayBacktrace(bool _breakAtEnd = false, int32_t _removeElement=0);
};

/**
 * @brief Basic macro of all logs macros
 * @param[in] logId Id of the library that log
 * @param[in] info Log level of this log: elog::level
 * @param[in] data Stream separaated with "<<" convertible in std::ostream
 */
#define ELOG_BASE(logId,info,data) \
	do { \
		if (info <= elog::getLevel(logId)) { \
			std::stringbuf sb; \
			std::ostream tmpStream(&sb); \
			tmpStream << data; \
			elog::logStream(logId, info, __LINE__, __PRETTY_FUNCTION__, tmpStream); \
		} \
	} while(0)
