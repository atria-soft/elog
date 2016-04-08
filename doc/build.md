Build lib & build sample                           {#elog_build}
========================

@tableofcontents

Download:                                          {#elog_build_download}
=========

elog use some tools to manage source and build it:

lutin (build-system):                              {#elog_build_download_lutin}
---------------------

```{.sh}
	pip install lutin --user
	# optionnal dependency of lutin (manage image changing size for application release
	pip install pillow --user
```


dependency:                                        {#elog_build_download_dependency}
-----------

**no dependency**

sources:                                           {#elog_build_download_sources}
--------

```{.sh}
	mkdir framework
	cd framework
	git clone https://github.com/atria-soft/elog.git
	cd ..
```

Build:                                             {#elog_build_build}
======


library:                                           {#elog_build_build_library}
--------

```{.sh}
	lutin -mdebug elog
```

Sample:                                            {#elog_build_build_sample}
-------

```{.sh}
	lutin -mdebug elog-sample
	lutin -mdebug elog-test
```

Or simple:

```{.sh}
	lutin -mdebug elog-*
```

Run sample:                                        {#elog_build_run_sample}
===========

```{.sh}
	lutin -mdebug elog-sample?run
```
