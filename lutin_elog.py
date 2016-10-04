#!/usr/bin/python
import lutin.debug as debug
import lutin.tools as tools


def get_type():
	return "LIBRARY"

def get_desc():
	return "Ewol log basic interface"

def get_licence():
	return "APACHE-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return "authors.txt"

def get_version():
	return "version.txt"

def configure(target, my_module):
	my_module.add_extra_flags()
	# add the file to compile:
	my_module.add_src_file([
	    'elog/debug.cpp',
	    'elog/log.cpp',
	    'elog/elog.cpp'
	    ])
	
	if "IOs" in target.get_type():
		my_module.add_src_file('elog/logIOs.m')
	
	my_module.add_header_file([
	    'elog/log.hpp',
	    'elog/elog.hpp'
	    ])
	
	if target.get_mode() == "debug":
		# Bor backtrace display :
		if     "Windows" not in target.get_type() \
		   and "MacOs" not in target.get_type() \
		   and "IOs" not in target.get_type():
			# TODO : check if it is really needed ...
			my_module.add_flag('link', [
				'-ldl',
				'-rdynamic'],
				export=True)
		elif "Windows" not in target.get_type():
			my_module.add_flag('link', [
				'-ldl'],
				export=True)
	# build in C++ mode
	my_module.compile_version("c++", 2011)
	my_module.compile_version("c", 1999)
	# add dependency of the generic C++ library:
	my_module.add_depend('cxx')
	my_module.add_optionnal_depend('ethread', ["c++", "-DELOG_BUILD_ETHREAD"])
	
	if "Windows" in target.get_type():
		pass
	elif "Android" in target.get_type():
		my_module.add_depend("SDK")
		pass
	else:
		my_module.add_depend("pthread")
	
	my_module.add_path(".")
	return True


