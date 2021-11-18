
/* memalloc2.c                                                                                          */
/* A. Schmiedl , FAU / Biochemistry Division , 2021-11-18                                               */
/* Using calloc() to claim/release large dynamic memory in big chunks                                   */
/* malloc() would reserve but not use up memory unless you actually put data into it (lazy allocation), */
/* even calloc() doesn't garantee to use up the RAM, so actually fill it with random data.              */
/* Use in conjunction with bash script memalloc2_reg.sh                                                 */

#define  BNUM  100000000                         /* number of bytes to allocate per memory block: 100M  */
#include <stdio.h>
#include <stdlib.h>                                      /* for: malloc(), calloc(), realloc(), free()  */
#include <unistd.h>                                                                    /* for: sleep()  */
#include <time.h>                                                             /* for srand() and rand() */

int main () {

    int n;                                                                 /* n = current block number  */
    unsigned long i;                                                           /* i = index into block  */
    char *parray[20];                                                  /* array of 20 pointers to char  */

    system("touch memalloc2_started");                     /* mark start of program by creating a file  */
    for (n=0; n<20; n++)  parray[n]=NULL;                           /* initialize all pointers to NULL  */

                                           /* Memory allocation on the heap in a time controlled loop:  */
    printf("--> Allocate 20*%d bytes of memory block by block in 4s time increments: \n", BNUM);
    printf("    Monitoring examples: \n");
    printf("      Get PID of process with:  ps -ea | grep memalloc2  \n");
    printf("      Print memory usage with:  pmap <pid> | tail -n 1 | awk '/total/{print $2}'  \n");
    printf("      Print available memory:   awk '/MemAvailable/ {print $2,$3}' /proc/meminfo  \n");

    srand(time(0));                                                /* Seed the random number generator  */

    for (n=0; n<20; n++)  {                                             /* Allocation and filling loop  */
        sleep(4);                                          /* wait 4s before each new block allocation  */
        parray[n] = (char *) calloc(BNUM, sizeof(char));                    /* Get BNUM bytes of space  */
        if (parray[n] == NULL) {
            fprintf(stderr, "--> Error! Memory allocation for block %d failed. \n", n);
            return(-(n+1));
        }
        else  {
            printf("    block %2d allocated, starting at location %p , ", n+1, (void *)parray[n] );
            printf(" fill it with random bytes ... \n" );
            for (i=0; i<BNUM; i++)  {                                           /* Fill each block ...  */
                * (parray[n] + i) = (char)(rand() % 256);                     /* ... with random bytes  */
            }
        }
    }

    printf("--> Now release the allocated memory blocks in 2s time steps: \n");
    for (n=19; n>=0; n--)  {
        sleep(2);                                       /* wait 2s before giving each block back to OS  */
        free(parray[n]);
        printf("    block %d freed. \n", n+1);
    }

    printf("--> All allocated memory blocks released - termination in 20s. \n");
    sleep(18);                             /* wait 18s with all memory released before removing marker  */
    system("rm memalloc2_started");                                              /* remove marker file  */
    sleep(2);                                                            /* wait 2s before termination  */

    return(0);
}
