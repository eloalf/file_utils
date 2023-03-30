#!/usr/bin/gnuplot -persist
# memalloc2_free.gp
# A.Schmiedl , 2023-03-28
# Use with 'memalloc2.c' and 'memalloc2_reg.sh'
# for live displaying free system memory
# after start of 'memalloc2' and 'memalloc2_reg.sh'
# Can be closed after manually deleting 'memalloc2_plt.dat'

set grid
set title 'available memory KB'
plot 'memalloc2_plt.dat' using 1:3 with lines
while (1) {
    replot
    pause 1
}
