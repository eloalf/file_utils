# file_utils

A small collection of useful tools mostly for LINUX to assist daily work with various kinds of data files and formats.

### bhist.c
Written in C under LINUX.<br/>
Sums up all equal byte values into an array of 256 bins, then builds, stores and launches a gnuplot script for logarithmic display of the histogram data. Requires an installation of gnuplot. Look up comment lines in source code (in German) for more information.

### memalloc2.c
Written in C under LINUX.<br/>
Compile with: `gcc -o memalloc2 memalloc2.c` <br/>
Simple program using **malloc()** to claim/release large dynamic memory chunks in a timer controlled way.
Used in conjunction with bash script **memalloc2_reg.sh** and for testing methods to limit memory usage of single processes.

### memalloc2_reg.sh
bash script for usage in conjunction with **memalloc2.c** to record memory usage of program **memalloc2** .
