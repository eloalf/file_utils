
#!/bin/bash
# memalloc2_reg.sh
# Store memory usage of program 'memalloc2' as well as available memory in 1s increments into log file 'memalloc2_plt.dat'
# Used in conjunction with program 'memalloc2' (compiled 'memalloc2.c')
# A. Schmiedl, FAU / Biochemistry Division, 2021-11-17 / 2021-11-18

# Wait for file 'memalloc2_started' marking the start of program 'memalloc2.c' :
echo "Waiting for 'memalloc2.c' to start ..."
until [ -f memalloc2_started ]
do
     sleep 0.1
done

counter=1
PROCNUM=$(ps -ea | awk '/memalloc2/{print $1}')
echo "PID = $PROCNUM found! "

# Used in an earlier version to terminate script when process was not yet started (checking for a valid PID number):
#if ! [[ "$PROCNUM" =~ ^[0-9]+$ ]]
#    then
#        echo "No valid process number found - exiting! "
#        exit -1
#    else
#        echo "PID = $PROCNUM found! "
#fi

echo "Start time of recording = $(date)"
STARTT=$(date +%s)
LASTT=$STARTT
if [ -e memalloc2_mem.dat ]
    then rm memalloc2_mem.dat
fi
while [ -e memalloc2_started ]        # check for marker file if still existent which indicates a running 'memalloc2'
do
    CURRT=$(date +%s)
    if [ $CURRT -gt $LASTT ]
    then    # execute this block once every second:
        echo "$((CURRT-STARTT)) $(pmap $PROCNUM | tail -n 1 | awk '/total/{print $2}') $(awk '/MemAvailable/ {print $2}' /proc/meminfo )" >> memalloc2_mem.dat
        LASTT=$CURRT
            # counter is incrementing in 1s intervals with some little jitter but long term accuracy
        ((counter++))
    fi
    sleep 0.2     # reduces significantly the amount of CPU time needed for this bash script
done

# remove 'K' and display valid lines only into new file memalloc2_plt.dat :
perl -pe 's/K//g' memalloc2_mem.dat | awk 'NF==3{print}' > memalloc2_plt.dat
rm memalloc2_mem.dat
echo "End time of recording   = $(date)"
echo $(wc memalloc2_plt.dat | awk '{print $1, "entries have been recorded into file memalloc2_plt.dat "}')
echo "File memalloc2_plt.dat contains seconds + KB values + free usable RAM, may be plotted with command: "
echo "gnuplot --persist -e \"set grid ; set title 'process memory KB' ;   plot 'memalloc2_plt.dat' using 1:2 with lines\" "
echo "gnuplot --persist -e \"set grid ; set title 'available memory KB' ; plot 'memalloc2_plt.dat' using 1:3 with lines\" "
