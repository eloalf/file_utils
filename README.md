# file_utils

A small collection of (more or less) useful tools for LINUX to assist daily work with data, files, formats and system resources.

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

### rpi_framebuffer.c
Written in C under PiOS (*Raspios Buster*) on and for a **Raspberry-Pi 4**.
Demonstration of how to use the Pi's framebuffer for plotting graphical elements and text without the use of a graphical desktop environment.<br/>
Read source code to learn how to create a graphical text block, switch to a GUI-less bash terminal with key combination &nbsp; `<Ctrl><Alt><F1>` &nbsp; and run a command like &nbsp; `./rpi_framebuffer 100 10 1820 10 50 50 250` &nbsp; which draws a rectangular block (100 px wide, 10 px high) with it's top left corner at 1820 px from left and 10 px from top and color values 1/4 red 1/4 green 1/1 blue.<br/>
Switch back to graphical desktop with &nbsp; `<Ctrl><Alt><F7>` &nbsp; .<br/>
A graphical block of text in RGB565 16-bit pixel format needs to be prepared in advance using a command sequence (&nbsp;**ImageMagick** and **FFmpeg** are required) like:<br/>
`convert -background black -fill lightgreen -font FreeMono-Bold -pointsize 28 \` <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  `label:"the quick brown fox ...\n0123456789-123456789..." textimg.png` <br/>
`identify textimg.png` <br/>
`display textimg.png` <br/>
`ffmpeg -vcodec png -i textimg.png -vcodec rawvideo -f rawvideo -pix_fmt rgb565 textimg_raw.rgb565` <br/>
The `identify` command prints the pixel dimensions of the image, which need to be entered into the source code, then compile with: <br/>
`gcc -o rpi_framebuffer rpi_framebuffer.c` <br/>

