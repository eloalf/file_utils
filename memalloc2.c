
/* memalloc2.c                                                                                          */
/* A. Schmiedl , FAU / Biochemistry Division , 2021-11-18 / 2021-11-30                                  */
/* Using calloc() to claim/release large dynamic memory in big chunks.                                  */
/* malloc() would reserve but not use up memory unless you actually put data into it (lazy allocation), */
/* even calloc() doesn't garantee to use up the RAM, so actually fill it with random data.              */
/* For every next block, store inverted bytes from the previous one, and compare before releasing it,   */
/* which is a kind of poor man's memory tester.                                                         */
/* Use in conjunction with bash script memalloc2_reg.sh                                                 */

#define  BNUM  100000000                         /* number of bytes to allocate per memory block: 100M  */
#include <stdio.h>
#include <stdlib.h>                                      /* for: malloc(), calloc(), realloc(), free()  */
#include <unistd.h>                                                                    /* for: sleep()  */
#include <time.h>                                                             /* for srand() and rand() */

int main () {

    int n;                                                                 /* n = current block number  */
    unsigned long i;                                                           /* i = index into block  */
    unsigned long identerr=0;                                    /* counter for memory cell comparision */
    char *parray[20];                                                  /* array of 20 pointers to char  */

    system("touch memalloc2_started");                     /* mark start of program by creating a file  */
    for (n=0; n<20; n++)  parray[n]=NULL;                           /* initialize all pointers to NULL  */

                                           /* Memory allocation on the heap in a time controlled loop:  */
    printf("--> Allocate 20*%d bytes of memory block by block in 4s time increments: \n", BNUM);
    printf("    Monitoring examples: \n");
    printf("      Get PID of process with:  ps -ea | grep memalloc2  \n");
    printf("      Print memory usage with:  pmap <pid> | tail -n 1 | awk '/total/{print $2}'  \n");
    printf("      Print available memory:   awk '/MemAvailable/ {print $2,$3}' /proc/meminfo  \n");
    printf("    Automated monitoring with script 'memalloc2_reg.sh': start before 'memalloc2'  \n");

    srand(time(0));                                                /* Seed the random number generator  */

    for (n=0; n<20; n++)  {                                             /* Allocation and filling loop  */
        sleep(4);                                              /* wait 4s before next block allocation  */
        parray[n] = (char *) calloc(BNUM, sizeof(char));                 /* Demand BNUM bytes of space  */
        if (parray[n] == NULL) {
            fprintf(stderr, "--> Error! Memory allocation for block %d failed. \n", n);
            return(-(n+1));
        }
        else  {
            if((n & 1) == 0)  {                                                    /* Even block number */
                printf("    even block %2d allocated, starting at location %p , ", n, (void *)parray[n] );
                printf(" fill it with random bytes ... \n" );
                for (i=0; i<BNUM; i++)
                    * (parray[n] + i) = (char)(rand() % 256);     /* Fill even block with random bytes  */
            }
            else  {                                                                 /* Odd block number */
                printf("    odd  block %2d allocated, starting at location %p , ", n, (void *)parray[n] );
                printf(" fill it with inverted bytes from previous block ... \n" );
                for (i=0; i<BNUM; i++)  {
                    *(parray[n] + i) = *(parray[n-1] + i) ^ 0xFF;  /* Fill odd block w. inverted bytes  */
                    if ((n==15) && ((i==55444333) || (i==12345678)))
                        *(parray[n] + i) = 0x5C;         /* In 1 block deliberately introduce 2 errors  */
                }
            }
        }
    }

    printf("--> Now check contents and release the allocated blocks in 2s time steps: \n");
    for (n=19; n>=0; n--)  {
        sleep(2);                                       /* wait 2s before giving each block back to OS  */
        if((n & 1) == 1)  {      /* check content of 2 adjacent blocks which should match when inverted */
            for (i=0; i<BNUM; i++)  {
                if ( ( (char) *(parray[n]+i)) != ( (char) (*(parray[n-1]+i)^0xFF)) )
                    identerr++;
            }            
        }
        free(parray[n]);
        printf("    block %d released. \n", n);
    }

    printf("--> All allocated memory blocks freed - termination in 20s. \n");
    printf("    Content error counts (2 errors expected) = %lu . \n", identerr);
    sleep(18);                             /* wait 18s with all memory released before removing marker  */
    system("rm memalloc2_started");                                              /* remove marker file  */
    sleep(2);                                                            /* wait 2s before termination  */

    return(0);
}
