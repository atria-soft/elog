#!/usr/bin/python
import lutin.debug as debug
import lutin.tools as tools
import lutin.debug as debug


def get_type():
	return "LIBRARY"

def get_sub_type():
	return "TEST"

def get_desc():
	return "basic debug log for test"

def get_licence():
	return "MPL-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return ["Mr DUPIN Edouard <yui.heero@gmail.com>"]

def configure(target, my_module):
	my_module.add_src_file([
		'test-debug/debug.cpp'
		])
	my_module.add_header_file([
		'test-debug/debug.hpp'
		])
	my_module.add_depend('etk')
	my_module.add_path(".")
	return True


