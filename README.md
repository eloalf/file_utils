# file_utils

A small collection of useful tools for LINUX to assist daily work with data, files, formats and system resources.

### bhist.c
Written in C under LINUX.<br/>
Sums up all equal byte values into an array of 256 bins, then builds, stores and launches a gnuplot script for logarithmic display of the histogram data. Requires an installation of gnuplot. Look up comment lines in source code (in German) for more information.

### memalloc2.c
Written in C under LINUX.<br/>
Compile with: `gcc -o memalloc2 memalloc2.c` <br/>
Simple program using **calloc()** to claim/release large dynamic memory chunks in a timer controlled way.
Used in conjunction with bash script **memalloc2_reg.sh** to record memory usage. A simple memory tester is included by comparing the contents of 2 adjacent blocks.
Useful also to learn about the way the Operating system is managing free memory resources and for testing methods to limit memory usage.
Max. memory consumption is 2 GB, runtime is 140s .<br/>
Run without arguments: `./memalloc2`

### memalloc2_reg.sh
bash script for usage in conjunction with **memalloc2.c** to record memory usage of program **memalloc2** <br/>
File **memalloc2_plt.dat** will be written holding variation in time of memory consumption and free available memory.<br/>
Run without arguments before starting **memalloc2** : `./memalloc2_reg.sh`
