/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/Stream.hpp>
#include <etk/Vector.hpp>
#include <etk/String.hpp>
#include <etk/Stream.hpp>
#include <etk/Function.hpp>

namespace elog {
	/**
	 * @brief Log level is a simple list of all log availlable. This enum is used when setting a log and when user chose the level of log displayed.
	 * @note: I use a "enum" and not an "enum class" because it create some problem on the macro declaration (not easy to convert number in an "enum class"
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
	int32_t registerInstance(const etk::String& _name);
	/**
	 * @brief Set the log level of a specific instance
	 * @param[in] _name Name of the intance
	 * @param[in] _level New level to set on the instance
	 * @code
	 *   elog::setLevel("ewol", elog::level_critical);
	 * @endcode
	 */
	void setLevel(const etk::String& _name, enum elog::level _level);
	/**
	 * @brief Set the log level of a specific instance
	 * @param[in] _id Id of the intance
	 * @param[in] _level New level to set on the instance
	 */
	void setLevel(int32_t _id, enum elog::level _level);
	/**
	 * @brief Set global debug level
	 * @param[in] _level New level to set on the instance
	 * @code
	 *   elog::setLevel(elog::level_verbose);
	 * @endcode
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
	etk::Vector<etk::String> getListInstance();
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
	 * @brief Set back-trace display on Error log enable or disable.
	 * @param[in] _status New value.
	 */
	void setBackTrace(bool _status);
	/**
	 * @brief Set back-trace display on Error log enable or disable.
	 * @param[in] _status New number of lines.
	 */
	void setMaxLineNumberInFile(size_t _status);
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
	void logStream(int32_t _id, int32_t _level, int32_t _ligne, const char* _funcName, const etk::Stream& _log);
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
	void logStream1(int32_t _id, int32_t _level, const etk::Stream& _log);
	/**
	 * @brief Display the current backtrace
	 * @param[in] _breakAtEnd assert program when backtrace is printed
	 * @param[in] _removeElement Number of element remove in the stack before display (permit to remove log function call)
	 */
	void displayBacktrace(bool _breakAtEnd = false, int32_t _removeElement=0);
	/**
	 * @brief function definition of an external log function
	 * @param[in] _libName pounter on a string containing the library name
	 * @param[in] _level Level of the log (can be filter with @ref setLevel)
	 * @param[in] _ligne Line of the log
	 * @param[in] _funcName Full function name: 'void etk::MyClass::functionName(...)'
	 * @param[in] _log String containing the Log
	 */
	using callbackLog = etk::Function<void (const char* _libName, enum elog::level _level, int32_t _ligne, const char* _funcName, const char* _log)>;
	/**
	 * @brief Set a callback to display all log in an other framework
	 * @param[in] _callback Function to call when log arrive
	 */
	void setCallbackLog(const elog::callbackLog& _callback);
	/**
	 * @brief Set log done in a specific file
	 * @param[in] _filename Name of the file to log (if not set the log system select alone the log file)
	 * @note in release the log is automatically store in a file in the system. (windows log is done in file automatically)
	 */
	void setLogInFile(const etk::String& _filename="");
	/**
	 * @brief Disable log in a file
	 */
	void unsetLogInFile();
	/**
	 * @brief When loggin in file, the flush is not done automaticaly ==> need to do it manualy
	 */
	void flush();
};

/**
 * @brief Basic macro of all logs macros
 * @param[in] logId Id of the library that log
 * @param[in] info Log level of this log: elog::level
 * @param[in] data Stream separaated with "<<" convertible in etk::Stream
 */
#define ELOG_BASE(logId,info,data) \
	do { \
		if (info <= elog::getLevel(logId)) { \
			etk::Stream tmpStream; \
			tmpStream << data; \
			elog::logStream(logId, info, __LINE__, __PRETTY_FUNCTION__, tmpStream); \
		} \
	} while(0)
