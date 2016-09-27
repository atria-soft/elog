Elog Tutorial: Runtime use                                {#elog_tutorial_02}
==========================

When you build your application you can access some log configuration:

```{.sh}
./yourApplication --help
```

you might have an output for elog:
```{.sh}
[P] elog - help : 
[P]     yourApplication [options]
[P]         --elog-level=            Change the default log level (set all Log level):
[P]             0: debug None (default in release)
[P]             1: debug Critical
[P]             2: debug Error
[P]             3: debug Warning
[P]             4: debug Info (default in debug)
[P]             5: debug Debug
[P]             6: debug Verbose
[P]         --elog-lib=name:X        Set a library specific level:
[P]             name  Name of the library
[P]             X     Log level to set [0..6]
[P]             note: ':' can be replace with '/' or '+'
[P]         --elog-file=pathToFile   File to store the logs: (disable console logs)
[P]         --elog-color             Enable color in log (default in Linux/debug)
[P]         --elog-no-color          Disable color in log (default in Linux/release and Other)
[P]         --elog-config=           Configure the Log interface
[P]             t: diplay time
[P]             T: diplay thread id
[P]             N: diplay thread name
[P]             L: diplay line number
[P]             l: diplay lib name
[P]             f: diplay function name
[P]         -h/--help:               Display this help
[P]     example:
[P]         yourApplication --elog-color --elog-level=2 --elog-lib=etk:5 --elog-lib=appl:6 --elog-config=NLlf
```


Then you can simply select the log level with:

```{.sh}
./yourApplication --elog-level=5
```

Or select a sub-element log level:

```{.sh}
./yourApplication --elog-lib=elog:1 --elog-lib=appl:6
```

Store in a file:
----------------

```{.sh}
./yourApplication --elog-file=log.txt
```


