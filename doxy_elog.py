#!/usr/bin/python
import os
import doxy.module as module
import doxy.debug as debug
import doxy.tools as tools

def create(target, module_name):
	my_module = module.Module(__file__, module_name)
	my_module.set_version([0,1,"dev"])
	my_module.set_title("Elog: Etk log interface")
	my_module.set_website("http://atria-soft.github.io/" + module_name)
	my_module.set_website_sources("http://github.com/atria-soft/" + module_name)
	my_module.set_path(os.path.join(tools.get_current_path(__file__), module_name))
	
	return my_module
