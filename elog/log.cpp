/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license MPL v2.0 (see license file)
 */

#include <elog/log.hpp>
#include <elog/debug.hpp>
#include <time.h>
#include <ethread/Mutex.hpp>
#include <ethread/Thread.hpp>
#include <etk/Map.hpp>
#include <inttypes.h>
#include <ethread/tools.hpp>
#include <elog/debug.hpp>

#if defined(__TARGET_OS__Android)
#	include <android/log.h>
#endif

#include <elog/logIOs.h>

#if    !defined(__STDCPP_LLVM__) \
    && defined(__TARGET_OS__Linux) \
    && defined(DEBUG) \
    && !defined(__TARGET_OS__Web) \
    && !defined(__TARGET_OS__buildroot)
	#include <execinfo.h>
	#include <cxxabi.h>
	#include <dlfcn.h>
	#define MAX_DEPTH  (256)
	void elog::displayBacktrace(bool _breakAtEnd, int32_t _removeElement) {
		// retrieve call-stack
		void * trace[MAX_DEPTH];
		int stack_depth = backtrace(trace, MAX_DEPTH);
		
		ELOG_PRINT("Back-trace : ");
		for (int32_t iii = 1; iii < stack_depth; ++iii) {
			Dl_info dlinfo;
			if(!dladdr(trace[iii], &dlinfo)) {
				break;
			}
			const char * symname = dlinfo.dli_sname;
			int    status;
			char * demangled = abi::__cxa_demangle(symname, null, 0, &status);
			if(status == 0 && demangled) {
				symname = demangled;
			}
			if (_removeElement <= 0) {
				ELOG_PRINT("  " << dlinfo.dli_fname << ": ");
				ELOG_PRINT("        " << symname);
			}
			_removeElement--;
			if(demangled != null) {
				free(demangled);
			}
		}
		if (_breakAtEnd == true) {
			assert(false);
		}
	}
#elif    defined(__TARGET_OS__MacOs) \
      && defined(DEBUG) \
      && defined(__MAC_10_5)
	#include <execinfo.h>
	#include <stdio.h>
	#define MAX_DEPTH  (256)
	void elog::displayBacktrace(bool _breakAtEnd, int32_t _removeElement) {
		void* callstack[MAX_DEPTH];
		uint32_t frames = backtrace(callstack, MAX_DEPTH);
		char** strs = backtrace_symbols(callstack, frames);
		ELOG_PRINT("Back-trace : ");
		for (uint32_t iii=0; iii<frames; ++iii) {
			ELOG_PRINT("    " << strs[iii]);
		}
		if (_breakAtEnd == true) {
			assert(false);
		}
		free(strs);
	}
#else
	void elog::displayBacktrace(bool _breakAtEnd, int32_t _removeElement) {
		#ifdef DEBUG
			assert(false);
		#endif
	}
#endif

#ifdef DEBUG
	#define DEFAULT_LOG_LEVEL elog::level_info
	#if    defined(__TARGET_OS__Windows) \
	    || defined(__TARGET_OS__Web)
		#define DEFAULT_LOG_COLOR false
	#else
		#define DEFAULT_LOG_COLOR true
	#endif
	#define DEFAULT_LOG_LINE true
	#define DEFAULT_LOG_THREAD_ID true
	#define DEFAULT_LOG_THREAD_NAME true
	#define DEFAULT_LOG_CLASS true
	#define DEFAULT_LOG_TIME true
	#define DEFAULT_LOG_LIB_NAME true
#else
	#define DEFAULT_LOG_LEVEL elog::level_error
	#define DEFAULT_LOG_COLOR false
	#define DEFAULT_LOG_LINE false
	#define DEFAULT_LOG_THREAD_ID false
	#define DEFAULT_LOG_THREAD_NAME false
	#define DEFAULT_LOG_CLASS false
	#define DEFAULT_LOG_TIME true
	#define DEFAULT_LOG_LIB_NAME true
#endif

enum elog::level& getDefaultLevel() {
	static enum elog::level g_val = DEFAULT_LOG_LEVEL;
	return g_val;
}

size_t& getFunctionSizeLog() {
	static size_t g_val = 5;
	return g_val;
}

size_t& getThreadSizeLog() {
	static size_t g_val = 5;
	return g_val;
}

size_t& getNameSizeLog() {
	static size_t g_val = 5;
	return g_val;
}

static etk::Vector<etk::Pair<etk::String, enum elog::level> >& getList() {
	static etk::Vector<etk::Pair<etk::String, enum elog::level> > g_val;
	return g_val;
}

int32_t elog::registerInstance(const etk::String& _name) {
	for (size_t iii = 0; iii < getList().size(); ++iii) {
		if (getList()[iii].first == _name) {
			return iii;
		}
	}
	getList().pushBack(etk::makePair(_name, getDefaultLevel()));
	if (_name.size() >= getNameSizeLog()) {
		getNameSizeLog() = _name.size()+1;
	}
	//printf("register log : '%s'=%ld\n", _name.c_str(), getList().size()-1);
	return getList().size()-1;
}

void elog::setLevel(const etk::String& _name, enum level _level) {
	for (size_t iii = 0; iii < getList().size(); ++iii) {
		if (getList()[iii].first == _name) {
			getList()[iii].second = _level;
			return;
		}
	}
	getList().pushBack(etk::makePair(_name, _level));
}

void elog::setLevel(enum level _level) {
	getDefaultLevel() = _level;
	for (size_t iii = 0; iii < getList().size(); ++iii) {
		getList()[iii].second = _level;
	}
}

void elog::setLevel(int32_t _id, enum level _level) {
	if (_id < 0 || _id > (int32_t)getList().size()) {
		// ERROR...
		return;
	}
	getList()[_id].second = _level;
}


int32_t elog::getLevel(int32_t _id) {
	if (_id < 0 || _id > (int32_t)getList().size()) {
		// ERROR...
		return 0;
	}
	return (int32_t)getList()[_id].second;
}

etk::Vector<etk::String> elog::getListInstance() {
	etk::Vector<etk::String> out;
	for (size_t iii = 0; iii < getList().size(); ++iii) {
		out.pushBack(getList()[iii].first);
	}
	return out;
}

void elog::logStream(int32_t _id, int32_t _level, int32_t _ligne, const char* _funcName, const etk::Stream& _log) {
	elog::logChar(_id, _level, _ligne, _funcName, _log.c_str());
}

void elog::logChar1(int32_t _id, int32_t _level, const char* _log) {
	elog::logChar(_id, _level, -1, null, _log);
}

void elog::logStream1(int32_t _id, int32_t _level, const etk::Stream& _log) {
	elog::logChar(_id, _level, -1, null, _log.c_str());
}

static bool& getColor() {
	static bool g_val = DEFAULT_LOG_COLOR;
	return g_val;
}

void elog::setColor(bool _status) {
	getColor() = _status;
}

static bool& getTime() {
	static bool g_val = DEFAULT_LOG_TIME;
	return g_val;
}
void elog::setTime(bool _status) {
	getTime() = _status;
}

static bool& getLine() {
	static bool g_val = DEFAULT_LOG_LINE;
	return g_val;
}
void elog::setLine(bool _status) {
	getLine() = _status;
}

static bool& getThreadId() {
	static bool g_val = DEFAULT_LOG_THREAD_ID;
	return g_val;
}
void elog::setThreadId(bool _status) {
	getThreadId() = _status;
}

static bool& getThreadNameEnable() {
	static bool g_val = DEFAULT_LOG_THREAD_NAME;
	return g_val;
}
void elog::setThreadNameEnable(bool _status) {
	getThreadNameEnable() = _status;
}

static bool& getFunction() {
	static bool g_val = DEFAULT_LOG_CLASS;
	return g_val;
}
void elog::setFunction(bool _status) {
	getFunction() = _status;
}

static bool& getLibName() {
	static bool g_val = DEFAULT_LOG_LIB_NAME;
	return g_val;
}
void elog::setLibName(bool _status) {
	getLibName() = _status;
}

static bool& getDisplayBackTrace() {
	static bool g_val = false;
	return g_val;
}
void elog::setBackTrace(bool _status) {
	getDisplayBackTrace() = _status;
}

static void getDisplayTime(char* data) {
#ifdef __TARGET_OS__Android
	struct timeval  now;
	gettimeofday(&now, null);
	sprintf(data, " %2dh%2d'%2d ", (int32_t)(now.tv_sec/3600)%24, (int32_t)(now.tv_sec/60)%60, (int32_t)(now.tv_sec%60));
#else
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	sprintf(data, " %2dh%2d'%2d ", (timeinfo->tm_hour)%24, timeinfo->tm_min, timeinfo->tm_sec);
#endif
}

static ethread::Mutex g_lock;
static elog::callbackLog callbackUserLog(null);

static etk::String& getLogFileName() {
	static etk::String g_val="";
	return g_val;
}
static FILE*& getLogFile() {
	static FILE* g_val=null;
	return g_val;
}

static size_t& getLogFileCount() {
	static size_t g_val = 0;
	return g_val;
}

static size_t& getLogFileMaxCount() {
	static size_t g_val = 0;
	return g_val;
}

void elog::setMaxLineNumberInFile(size_t _status) {
	getLogFileMaxCount() = _status;
}


// Copy for ETK FSNODE ...     [START]

extern "C" {
	// file browsing ...
	#include <dirent.h>
	#include <sys/stat.h>
	#include <errno.h>
}

static int32_t FSNODE_LOCAL_mkdir(const char* _path, mode_t _mode) {
	struct stat st;
	int32_t status = 0;
	if (stat(_path, &st) != 0) {
		/* Directory does not exist. EEXIST for race condition */
		#ifdef __TARGET_OS__Windows
		if(mkdir(_path) != 0
		#else
		if(mkdir(_path, _mode) != 0
		#endif
		    && errno != EEXIST) {
			status = -1;
		}
	} else if (!S_ISDIR(st.st_mode)) {
		errno = ENOTDIR;
		status = -1;
	}
	return(status);
}

static int32_t FSNODE_LOCAL_mkPath(const char* _path, mode_t _mode) {
	char *pp;
	char *sp;
	int status;
	char *copypath = strdup(_path);
	if (null==copypath) {
		return -1;
	}
	status = 0;
	pp = copypath;
	while (status == 0 && (sp = strchr(pp, '/')) != 0) {
		if (sp != pp) {
			/* Neither root nor double slash in path */
			*sp = '\0';
			status = FSNODE_LOCAL_mkdir(copypath, _mode);
			*sp = '/';
		}
		pp = sp + 1;
	}
	if (status == 0) {
		status = FSNODE_LOCAL_mkdir(_path, _mode);
	}
	free(copypath);
	return (status);
}

static bool FSNODE_LOCAL_exist(const etk::String& _path) {
	struct stat st;
	int32_t status = 0;
	if (stat(_path.c_str(), &st) != 0) {
		return false;
	}
	return true;
}
// Copy for ETK FSNODE ...     [END]

void elog::setLogInFile(const etk::String& _filename) {
	elog::unsetLogInFile();
	ELOG_PRINT("Log in file: '" << _filename << "'");
	getLogFileName() = _filename;
	FILE*& file = getLogFile();
	// create path of the file:
	size_t found=_filename.rfind("/\\");
	etk::String path = _filename.extract(0,found);
	if (FSNODE_LOCAL_exist(path) == false) {
		FSNODE_LOCAL_mkPath(path.c_str(), 0760);
	}
	g_lock.lock();
	file = fopen(_filename.c_str(), "w");
	g_lock.unLock();
	if (file == null) {
		ELOG_ERROR("Can not open file: '" << _filename << "'");
	}
}

void elog::unsetLogInFile() {
	getLogFileName() = "";
	g_lock.lock();
	FILE*& file = getLogFile();
	// close file only if needed ...
	if (file != null) {
		fflush(file);
		fclose(file);
		file = null;
	}
	g_lock.unLock();
}


void elog::setCallbackLog(const elog::callbackLog& _callback) {
	// TODO : Check atomicity ...
	g_lock.lock();
	callbackUserLog = _callback;
	g_lock.unLock();
}
//regular colors
#define ETK_BASH_COLOR_BLACK			"\e[0;30m"
#define ETK_BASH_COLOR_RED				"\e[0;31m"
#define ETK_BASH_COLOR_GREEN			"\e[0;32m"
#define ETK_BASH_COLOR_YELLOW			"\e[0;33m"
#define ETK_BASH_COLOR_BLUE				"\e[0;34m"
#define ETK_BASH_COLOR_MAGENTA			"\e[0;35m"
#define ETK_BASH_COLOR_CYAN				"\e[0;36m"
#define ETK_BASH_COLOR_WHITE			"\e[0;37m"
//emphasized (bolded) colors
#define ETK_BASH_COLOR_BOLD_BLACK		"\e[1;30m"
#define ETK_BASH_COLOR_BOLD_RED			"\e[1;31m"
#define ETK_BASH_COLOR_BOLD_GREEN		"\e[1;32m"
#define ETK_BASH_COLOR_BOLD_YELLOW		"\e[1;33m"
#define ETK_BASH_COLOR_BOLD_BLUE		"\e[1;34m"
#define ETK_BASH_COLOR_BOLD_MAGENTA		"\e[1;35m"
#define ETK_BASH_COLOR_BOLD_CYAN		"\e[1;36m"
#define ETK_BASH_COLOR_BOLD_WHITE		"\e[1;37m"
//background colors
#define ETK_BASH_COLOR_BG_BLACK			"\e[40m"
#define ETK_BASH_COLOR_BG_RED			"\e[41m"
#define ETK_BASH_COLOR_BG_GREEN			"\e[42m"
#define ETK_BASH_COLOR_BG_YELLOW		"\e[43m"
#define ETK_BASH_COLOR_BG_BLUE			"\e[44m"
#define ETK_BASH_COLOR_BG_MAGENTA		"\e[45m"
#define ETK_BASH_COLOR_BG_CYAN			"\e[46m"
#define ETK_BASH_COLOR_BG_WHITE			"\e[47m"
// Return to the normal color setings
#define ETK_BASH_COLOR_NORMAL			"\e[0m"
//go to the Top of bash
#define ETK_BASH_GO_TOP					"\e[0;0f"

#define LENGHT_MAX_LOG (2048)

void elog::logChar(int32_t _id, int32_t _level, int32_t _ligne, const char* _funcName, const char* _log) {
	// special callback mode:
	if (callbackUserLog != null) {
		const char* libName = "";
		if (_id >= 0) {
			libName = getList()[_id].first.c_str();
		}
		g_lock.lock();
		if (callbackUserLog != null) {
			callbackUserLog(libName, elog::level(_level), _ligne, _funcName, _log);
		}
		g_lock.unLock();
		return;
	}
	char handle[LENGHT_MAX_LOG] = "";
	memset(handle, ' ', LENGHT_MAX_LOG);
	handle[0] = '\0';
	char* pointer = handle;
	if(getColor() == true) {
		switch(_level) {
			default:
				// nothing to do ...
				break;
			case elog::level_critical:
				strcat(pointer, ETK_BASH_COLOR_BOLD_RED);
				break;
			case elog::level_error:
				strcat(pointer, ETK_BASH_COLOR_RED);
				break;
			case elog::level_warning:
				strcat(pointer, ETK_BASH_COLOR_MAGENTA);
				break;
			case elog::level_info:
				strcat(pointer, ETK_BASH_COLOR_CYAN);
				break;
			case elog::level_debug:
				strcat(pointer, ETK_BASH_COLOR_YELLOW);
				break;
			case elog::level_verbose:
				strcat(pointer, ETK_BASH_COLOR_WHITE);
				break;
			case elog::level_print:
				strcat(pointer, ETK_BASH_COLOR_WHITE);
				break;
		}
		pointer = handle+strlen(handle);
	}
	if(getTime() == true) {
		getDisplayTime(pointer);
		pointer = handle+strlen(handle);
	}
	#ifndef __TARGET_OS__Android
		switch(_level) {
			default:
				strcat(pointer, "[?] ");
				break;
			case elog::level_print:
				strcat(pointer, "[P] ");
				break;
			case elog::level_critical:
				strcat(pointer, "[C] ");
				break;
			case elog::level_error:
				strcat(pointer, "[E] ");
				break;
			case elog::level_warning:
				strcat(pointer, "[W] ");
				break;
			case elog::level_info:
				strcat(pointer, "[I] ");
				break;
			case elog::level_debug:
				strcat(pointer, "[D] ");
				break;
			case elog::level_verbose:
				strcat(pointer, "[V] ");
				break;
		}
		pointer = handle+strlen(handle);
	#endif
	if (getLibName() == true) {
		if (_id >= 0) {
			int32_t len = strlen(handle);
			strcat(pointer, getList()[_id].first.c_str());
			pointer = handle+strlen(handle);
			while (strlen(handle) - len < getNameSizeLog()) {
				*pointer++ = ' ';
				*pointer = '\0';
			}
			*pointer++ = '|';
			*pointer++ = ' ';
			*pointer = '\0';
		}
	}
	#ifdef ELOG_BUILD_ETHREAD
		if(getThreadId() == true) {
			// display thread ID
			uint32_t iddd = ethread::getId();
			sprintf(pointer, "%3d", iddd);
			pointer = handle+strlen(handle);
			*pointer++ = ' ';
			*pointer++ = '|';
			*pointer++ = ' ';
			*pointer = '\0';
		}
		if(getThreadNameEnable() == true) {
			// display thread ID
			etk::String name = ethread::getName();
			if (name.size() >= getThreadSizeLog() ) {
				getThreadSizeLog() = name.size() + 1;
			}
			sprintf(pointer, "%s", name.c_str());
			pointer = handle+strlen(handle);
			size_t nbSpaceToAdd = getThreadSizeLog()-name.size();
			for (size_t iii=0; iii<nbSpaceToAdd; ++iii) {
				*pointer++ = ' ';
				*pointer = '\0';
			}
			*pointer++ = '|';
			*pointer++ = ' ';
			*pointer = '\0';
		}
	#endif
	if(getLine() == true) {
		if (_ligne >= 0) {
			sprintf(pointer, "(l=%5d)", _ligne);
			pointer = handle+strlen(handle);
			*pointer++ = ' ';
			*pointer = '\0';
		}
	}
	// TODO :Maybe optimize this one ...
	if(getFunction() == true) {
		int32_t len = strlen(handle);
		char tmpName[1024];
		char *tmpPointer = tmpName;
		if (_funcName != null) {
			// cleen for android :
			char* startPos = strchr((char*)_funcName, ' ');
			char* stopPos = strchr((char*)_funcName, '(');
			if (startPos != null) {
				if (stopPos != null) {
					char* startPos2 = strchr(startPos+1, ' ');
					while (    startPos2 != null
					        && startPos2 < stopPos) {
						startPos = startPos2;
						startPos2 = strchr(startPos+1, ' ');
					}
					if(uint64_t(stopPos) < uint64_t(startPos)) {
						snprintf(tmpPointer, etk::min(uint64_t(1024), uint64_t(stopPos)-uint64_t(_funcName)), "%s", _funcName);
					} else {
						snprintf(tmpPointer, etk::min(uint64_t(1024), uint64_t(stopPos)-uint64_t(startPos)), "%s", startPos+1);
					}
				} else {
					snprintf(tmpPointer, 1024, "%s", startPos);
				}
			} else {
				if (stopPos != null) {
					snprintf(tmpPointer, etk::min(uint64_t(1024), uint64_t(stopPos)-uint64_t(_funcName)+1), "%s", _funcName);
				} else {
					snprintf(tmpPointer, 1024, "%s", _funcName);
				}
			}
			tmpPointer = tmpPointer+strlen(tmpPointer);
		}
		size_t lenFunc = strlen(tmpName);
		if (lenFunc >= getFunctionSizeLog()) {
			getFunctionSizeLog() = lenFunc+1;
		}
		size_t nbSpaceToAdd = getFunctionSizeLog() - lenFunc;
		for (size_t iii=0; iii<nbSpaceToAdd; ++iii) {
			*tmpPointer++ = ' ';
			*tmpPointer = '\0';
		}
		*tmpPointer++ = '|';
		*tmpPointer++ = ' ';
		*tmpPointer = '\0';
		strcat(pointer, tmpName);
		pointer += strlen(tmpName);
	}
	if (strlen(_log) > LENGHT_MAX_LOG - strlen(handle)-20) {
		memcpy(pointer, _log, LENGHT_MAX_LOG - strlen(handle)-21);
		handle[1024-25] = ' ';
		handle[1024-24] = '.';
		handle[1024-23] = '.';
		handle[1024-22] = '.';
		handle[1024-21] = '\0';
	} else {
		strcat(pointer, _log);
	}
	pointer = handle+strlen(handle);
	if(getColor() == true) {
		strcat(pointer, ETK_BASH_COLOR_NORMAL);
	}
	
	g_lock.lock();
	{
		FILE*& file = getLogFile();
		size_t& fileCurrentCount = getLogFileCount();
		size_t& fileMaxCount = getLogFileMaxCount();
		// close file only if needed ...
		if (file != null) {
			*pointer++ = '\n';
			*pointer = '\0';
			fprintf(file, "%s", handle);
			switch(_level) {
				default:
					break;
				case elog::level_critical:
				case elog::level_error:
					fflush(file);
					break;
			}
			// Increment the line counter in file
			fileCurrentCount++;
			if (fileMaxCount != 0) {
				// the user request a maximum line number in the file:
				if (fileCurrentCount > fileMaxCount) {
					fileCurrentCount = 0;
					fflush(file);
					fclose(file);
					etk::String tmpFileName = getLogFileName();
					if (    tmpFileName.size() > 0
					     && tmpFileName[tmpFileName.size()-1] == '2') {
						getLogFileName().popBack();
					} else {
						getLogFileName() += "2";
					}
					file = fopen(getLogFileName().c_str(), "w");
				}
			}
			// if we log in file, we have no need to log otherwise ... just "tail -f log.txt"
			g_lock.unLock();
			return;
		}
	}
	#if defined(__TARGET_OS__Android)
		switch(_level) {
			default:
				__android_log_print(ANDROID_LOG_VERBOSE, "EWOL", "%s", handle);
				break;
			case elog::level_print:
				__android_log_print(ANDROID_LOG_INFO, "EWOL", "%s", handle);
				break;
			case elog::level_critical:
				__android_log_print(ANDROID_LOG_FATAL, "EWOL", "%s", handle);
				break;
			case elog::level_error:
				__android_log_print(ANDROID_LOG_ERROR, "EWOL", "%s", handle);
				break;
			case elog::level_warning:
				__android_log_print(ANDROID_LOG_WARN, "EWOL", "%s", handle);
				break;
			case elog::level_info:
				__android_log_print(ANDROID_LOG_INFO, "EWOL", "%s", handle);
				break;
			case elog::level_debug:
				__android_log_print(ANDROID_LOG_DEBUG, "EWOL", "%s", handle);
				break;
			case elog::level_verbose:
				__android_log_print(ANDROID_LOG_VERBOSE, "EWOL", "%s", handle);
				break;
		}
	#elif defined(__TARGET_OS__IOs)
		iosNSLog(handle);
	#else
		printf("%s\n", handle);
	#endif
	g_lock.unLock();
	if (_level == level_critical) {
		#if defined(DEBUG)
		ethread::sleepMilliSeconds(700);
		#endif
		displayBacktrace(true, 2);
	}
	// Display backtrace to facilitate the error problems
	if (    _level == level_error
	     && getDisplayBackTrace() == true) {
		displayBacktrace(false, 2);
	}
}


void elog::flush() {
	g_lock.lock();
	FILE*& file = getLogFile();
	if (file != null) {
		fflush(file);
	}
	fflush(stdout);
	g_lock.unLock();
	return;
}