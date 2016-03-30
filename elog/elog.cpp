/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <elog/log.h>
#include <elog/elog.h>
#include <elog/debug.h>

static elog::level getLogLevel(const std::string& _value) {
	if (_value == "0") {
		return elog::logLevelNone;
	} else if (_value == "1") {
		return elog::logLevelCritical;
	} else if (_value == "2") {
		return elog::logLevelError;
	} else if (_value == "3") {
		return elog::logLevelWarning;
	} else if (_value == "4") {
		return elog::logLevelInfo;
	} else if (_value == "5") {
		return elog::logLevelDebug;
	} else if (_value == "6") {
		return elog::logLevelVerbose;
	}
	ELOG_ERROR("Unknow log level : " << _value);
	return elog::logLevelVerbose;
}

static bool startWith(const std::string& _obj, const std::string& _val) {
	if (_val.size() == 0) {
		return false;
	}
	if (_val.size() > _obj.size()) {
		return false;
	}
	for( size_t iii = 0;
	     iii < _val.size();
	     iii++) {
		if (std::tolower(_val[iii]) != std::tolower(_obj[iii])) {
			return false;
		}
	}
	return true;
}

static std::vector<std::string> split(const std::string& _input, char _val) {
	std::vector<std::string> list;
	size_t lastStartPos = 0;
	for(size_t iii=0; iii<_input.size(); iii++) {
		if (_input[iii]==_val) {
			list.push_back(std::string(_input, lastStartPos, iii - lastStartPos));
			lastStartPos = iii+1;
		}
	}
	if (lastStartPos<_input.size()) {
		list.push_back(std::string(_input, lastStartPos));
	}
	return list;
}

void elog::init(int _argc, const char** _argv) {
	ELOG_INFO("E-log system init (BEGIN)");
	for (int32_t iii=0; iii<_argc ; ++iii) {
		std::string data = _argv[iii];
		if (startWith(data, "--elog-level=")) {
			elog::setLevel(getLogLevel(std::string(data.begin()+13, data.end())));
		} else if (startWith(data, "--elog-color")) {
			elog::setColor(true);
		} else if (startWith(data, "--elog-no-color")) {
			elog::setColor(false);
		} else if (startWith(data, "--elog-config=")) {
			std::string value(data.begin()+17, data.end());
			elog::setTime(false);
			elog::setLine(false);
			elog::setFunction(false);
			elog::setLibName(false);
			elog::setThreadId(false);
			elog::setThreadNameEnable(false);
			for (size_t iii=0; iii<value.size(); ++iii) {
				if (value[iii] == 't') {
					elog::setTime(true);
				} else if (value[iii] == 'T') {
					elog::setThreadId(true);
				} else if (value[iii] == 'N') {
					elog::setThreadNameEnable(true);
				} else if (value[iii] == 'L') {
					elog::setLine(true);
				} else if (value[iii] == 'l') {
					elog::setLibName(true);
				} else if (value[iii] == 'f') {
					elog::setFunction(true);
				} else {
					ELOG_ERROR("In program argument: --elog-config= , the value '" << value[iii] << "' is not supported");
				}
			}
		} else if (startWith(data, "--elog-lib=")) {
			std::string value(data.begin()+14, data.end());
			std::vector<std::string> list = split(value, ':');
			if (list.size() != 2) {
				ELOG_ERROR("Can not set the --elog-lib= with value='" << value << "' not formated name:X");
				continue;
			}
			elog::setLevel(list[0], getLogLevel(list[1]));
		} else if (    data == "-h"
		            || data == "--help") {
			ELOG_PRINT("elog - help : ");
			ELOG_PRINT("    " << _argv[0] << " [options]");
			ELOG_PRINT("        --elog-level=      Change the default log level (set all Log lovel):");
			ELOG_PRINT("            0: debug None (default in release)");
			ELOG_PRINT("            1: debug Critical");
			ELOG_PRINT("            2: debug Error");
			ELOG_PRINT("            3: debug Warning");
			ELOG_PRINT("            4: debug Info (default in debug)");
			ELOG_PRINT("            5: debug Debug");
			ELOG_PRINT("            6: debug Verbose");
			ELOG_PRINT("        --elog-lib=name:X  Set a library specific level:");
			ELOG_PRINT("            name  Name of the library");
			ELOG_PRINT("            X     Log level to set [0..6]");
			ELOG_PRINT("        --elog-color       Enable color in log (default in Linux/debug)");
			ELOG_PRINT("        --elog-no-color    Disable color in log (default in Linux/release and Other)");
			ELOG_PRINT("        --elog-config=     Configure the Log interface");
			ELOG_PRINT("            t: diplay time");
			ELOG_PRINT("            T: diplay thread id");
			ELOG_PRINT("            N: diplay thread name");
			ELOG_PRINT("            L: diplay line number");
			ELOG_PRINT("            l: diplay lib name");
			ELOG_PRINT("            f: diplay fundtion name");
			ELOG_PRINT("        -h/--help: this help");
			ELOG_PRINT("    example:");
			ELOG_PRINT("        " << _argv[0] << " --elog-color --elog-level=2 --elog-lib=etk:5 --elog-lib=appl:6 --elog-config=NLlf");
		} else if (startWith(data, "--elog")) {
			ELOG_ERROR("Can not parse the argument : '" << data << "'");
		}
	}
	ELOG_INFO("E-LOG system init (END)");
}


