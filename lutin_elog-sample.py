#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools


def get_type():
	return "BINARY"

def get_sub_type():
	return "SAMPLE"

def get_desc():
	return "elog sample"

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

def create(target, module_name):
	my_module = module.Module(__file__, module_name, get_type())
	my_module.add_extra_flags()
	# add the file to compile:
	my_module.add_src_file([
		'sample/debug.cpp',
		'sample/main.cpp'
		])
	# build in C++ mode
	my_module.compile_version("c++", 2011)
	my_module.add_depend('elog')
	my_module.add_path('sample')
	return my_module

