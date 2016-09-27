Elog Tutorial: Add some Log (using)                                {#elog_tutorial_01}
===================================

@tableofcontents

You might not use the log directly.
The reson is simple:
- It is designed to be replaced by an other log library.

This permit you to use custom log library just replacing Macro and basic functions


Declaring the list of macro                                         {#elog_tutorial_01_declaration}
===========================


**debug.h**

@include sample/debug.h

**debug.cpp**

@include sample/debug.cpp


- on your main application:

@snippet sample/main.cpp elog_sample_main_include
@snippet sample/main.cpp elog_sample_main_base

Using it                                                      {#elog_tutorial_01_using_it}
========

You just need to call the macro whe you want to add debug log:

@snippet sample/main.cpp elog_sample_main_log


Specification of logs
---------------------

  - *_CRITICAL(***); This will log the data and asert just after (display backtrace if possible)
  - *_PRINT(***); display on console (can not be removed with the log-level)




Log in an external logger                                     {#elog_tutorial_01_external}
=========================

You must specify an external function that is receiving the logs:

@snippet sample/main.cpp elog_sample_main_callback_DECLARATION

Now you must connect it on the elog backend:

@snippet sample/main.cpp elog_sample_main_callback_link

```{.cpp}
	elog::setCallbackLog(&myExternalLogCallback);
```

The full code of the callback:
@snippet sample/main.cpp elog_sample_main_callback

you can test the program:

```{.sh}
lutin -C -P -mdebug elog-sample?build?run:--elog-level=2
```
