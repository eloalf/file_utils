#!/usr/bin/gnuplot -persist
# memalloc2_proc.gp
# A.Schmiedl , 2023-03-28
# Use with 'memalloc2.c' and 'memalloc2_reg.sh'
# for live displaying memory allocation for 'memalloc2'
# after start of 'memalloc2' and 'memalloc2_reg.sh'
# Can be closed after manually deleting 'memalloc2_plt.dat'

set grid
set title 'process memory KB'
plot 'memalloc2_plt.dat' using 1:2 with lines
while (1) {
    replot
    pause 1
}
