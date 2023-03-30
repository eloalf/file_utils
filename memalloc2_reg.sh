
#!/bin/bash
# memalloc2_reg.sh
# Store memory demand of program 'memalloc2' as well as available memory in 1s increments into log file 'memalloc2_plt.dat'
# Revised version, gets rid of intermediate file 'memalloc2_mem.dat'
# Used in conjunction with program 'memalloc2.c' , start without arguments. Sample rate is 1 Hz
# Recognizing start of 'memalloc2.c' by its marker file 'memalloc2_started'
# Since log file 'memalloc2_plt.dat' will be written continuously, live monitoring is a good idea, so start the scripts
# 'memalloc2_proc.gp' and 'memalloc2_free.gp'
# A. Schmiedl, FAU / Biochemistry Division, 2023-03-22 / 2023-03-28

# Wait for file 'memalloc2_started' marking the start of program 'memalloc2.c' :
# (replace it by a keypress or some other thing without such a marker file)
echo "Waiting for 'memalloc2' to start by looking at a marker file ..."
until [ -f memalloc2_started ]
do
     sleep 0.1
done

counter=1
PROCNUM=$(ps -ea | awk '/memalloc2/{print $1}')
echo "PID = $PROCNUM found! "
echo "Start time of recording = $(date)"

STARTT=$(date +%s)
LASTT=$STARTT
if [ -e memalloc2_plt.dat ]           # remove old log file when existing
    then rm memalloc2_plt.dat
fi
while [ -e memalloc2_started ]        # check for marker file which is created in 'memalloc2' to indicate executing
do
    CURRT=$(date +%s)
    if [ $CURRT -gt $LASTT ]
    then    # execute this block once every second:
        echo "$((CURRT-STARTT)) $(pmap $PROCNUM | tail -n 1 | awk '/total/{print $2}') $(awk '/MemAvailable/ {print $2}' /proc/meminfo )" | \
            sed 's/K//' | awk 'NF==3{print}' >> memalloc2_plt.dat
        LASTT=$CURRT
            # counter is incrementing in 1s intervals with some little jitter but good long term accuracy
        ((counter++))
    fi
    sleep 0.2     # reduces significantly the amount of CPU time needed for this bash script
done

echo "End time of recording   = $(date)"
echo $(wc memalloc2_plt.dat | awk '{print $1, "records have been written into file memalloc2_plt.dat "}')
echo "File memalloc2_plt.dat contains 3 columns with: runtime [s] , KB values , free usable RAM ; it may be visualized with gnuplot: "
echo "    gnuplot --persist -e \"set grid ; set title 'process memory KB' ;   plot 'memalloc2_plt.dat' using 1:2 with lines\" "
echo "        use script 'memalloc2_proc.gp' for live plotting, delete log file 'memalloc2_plt.dat' to terminate script "
echo "    gnuplot --persist -e \"set grid ; set title 'available memory KB' ; plot 'memalloc2_plt.dat' using 1:3 with lines\" "
echo "        use script 'memalloc2_free.gp' for live plotting, delete log file 'memalloc2_plt.dat' to terminate script "

