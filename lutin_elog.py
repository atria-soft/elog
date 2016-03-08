#!/usr/bin/python
import lutin.module as module
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
	return ["Mr DUPIN Edouard <yui.heero@gmail.com>"]

def get_version():
	return [0,1,"dev"]

def create(target, module_name):
	my_module = module.Module(__file__, module_name, get_type())
	my_module.add_extra_compile_flags()
	# add the file to compile:
	my_module.add_src_file([
		'elog/debug.cpp',
		'elog/log.cpp'
		])
	
	if target.name=="IOs":
		my_module.add_src_file('etk/logIOs.m')
	
	my_module.add_header_file([
		'elog/log.h',
		])
	
	if target.config["mode"] == "debug":
		# Bor backtrace display :
		if     target.name != "Windows" \
		   and target.name != "MacOs" \
		   and target.name != "IOs":
			# TODO : check if it is really needed ...
			my_module.add_export_flag('link', [
				'-ldl',
				'-rdynamic'])
		elif target.name != "Windows":
			my_module.add_export_flag('link', [
				'-ldl'])
	# build in C++ mode
	my_module.compile_version("c++", 2011)
	# add dependency of the generic C++ library:
	my_module.add_module_depend('cxx')
	my_module.add_optionnal_module_depend('ethread', ["c++", "-DELOG_BUILD_ETHREAD"])
	
	if target.name=="Windows":
		pass
	elif target.name=="Android":
		my_module.add_module_depend("SDK")
		pass
	else:
		#TODO : Set it in a generic include system
		my_module.add_export_flag('link-lib', "pthread")
	
	my_module.add_path(tools.get_current_path(__file__))
	return my_module


