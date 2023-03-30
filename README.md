# file_utils

A small collection of (more or less) useful tools for LINUX to assist daily work with data, files, object formats, system resources and to explore functions and capabilities of LINUX.

### bhist.c
Written in C under LINUX.<br/>
Sums up all equal byte values into an array of 256 bins, then builds, stores and launches a **gnuplot** script for logarithmic display of the histogram data. Requires an installation of **gnuplot**. Look up comment lines in source code (in German) for more information.

### memalloc2.c
Written in C under LINUX.<br/>
Compile with: `gcc -o memalloc2 memalloc2.c` <br/>
Simple program using **calloc()** to claim/release large dynamic memory chunks in a timed sequence.
Used in conjunction with bash script **memalloc2_reg.sh** to record memory usage. A simple memory tester is included by comparing the contents of every other block.
Useful also to learn about the way the operating system is managing free memory resources and for testing methods to limit memory usage.
Edit two `#define` lines with values `BNUM` and `BLKS` prior to compilation to change amount of allocated memory, preset is a memory consumption
of 4 GB with resulting runtime of 284s .<br/>
Run without arguments: `./memalloc2`

### memalloc2_reg.sh
bash script for use with **memalloc2.c** to record memory usage of program **memalloc2** . <br/>
A data file **memalloc2_plt.dat** will be created holding a time course of memory consumption and free available memory.<br/>
Run without arguments before (!) starting **memalloc2** : `./memalloc2_reg.sh` .<br/>
Live visualization could be performed with **gnuplot** scripts `./memalloc2_proc.gp` and `./memalloc2_free.gp` started after the bash script.
Manual deletion/renaming of the data file allows closing the two visualization scripts.

### rpi_framebuffer.c
Written in C under PiOS (*Raspios Buster*) on and for a **Raspberry-Pi 4**.
Demonstration of how to use the Pi's framebuffer for plotting graphical elements and text without the use of a graphical desktop environment.<br/>
Read source code to learn how to create a graphical text block, switch to a GUI-less bash terminal with key combination &nbsp; `<Ctrl><Alt><F1>` &nbsp; and run a command like &nbsp; `./rpi_framebuffer 100 10 1820 10 50 50 250` &nbsp; which draws a rectangular block (100 px wide, 10 px high) with it's top left corner at 1820 px from left and 10 px from top and color values 25% red + 25% green + 100% blue. Color values range from 0 to 255. <br/>
Switch back to graphical desktop with &nbsp; `<Ctrl><Alt><F7>` &nbsp; .<br/>
A graphical block of text in RGB565 16-bit pixel format needs to be prepared in advance using a command sequence (&nbsp;**ImageMagick** and **FFmpeg** are required) like:<br/>
`convert -background black -fill lightgreen -font FreeMono-Bold -pointsize 28 \` <br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  `label:"the quick brown fox ...\n0123456789-123456789..." textimg.png` <br/>
`identify textimg.png` <br/>
`display textimg.png` <br/>
`ffmpeg -vcodec png -i textimg.png -vcodec rawvideo -f rawvideo -pix_fmt rgb565 textimg_raw.rgb565` <br/>
The `identify` command prints the dimensions (in pixels) of the image, which need to be entered into the source code, then compile with: <br/>
`gcc -o rpi_framebuffer rpi_framebuffer.c` <br/>

