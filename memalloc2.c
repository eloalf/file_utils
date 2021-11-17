
/* memalloc2.c                                                                        */
/* Using malloc() to claim/release large dynamic memory in chunks                     */
/* A. Schmiedl , FAU / Biochemistry Division , 2021-11-16 / 2021-11-17                */
/* Used in conjunction with bash script memalloc2_reg.sh                              */

#define  BNUM  100000000       /* number of bytes to allocate per memory block: 100M  */
#include <stdio.h>
#include <stdlib.h>                    /* for: malloc(), calloc(), realloc(), free()  */
#include <unistd.h>                                                  /* for: sleep()  */

int main () {

    int i, n;
    char *parray[20];                                /* array of 20 pointers to char  */

    system("touch memalloc2_started");            /* mark start of program with file  */
    for (n=0; n<20; n++)  parray[i]=NULL;             /* initialize pointers to NULL  */

                         /* Memory allocation on the heap in a time controlled loop:  */
    printf("--> Claim 20*%d bytes of memory block by block in 4s time increments: \n", BNUM);
    printf("    Monitoring method: \n");
    printf("    Get PID of process with:  ps -ea | grep memalloc2  \n");
    printf("    Print memory usage with:  sudo pmap <pid> | tail -n 1 | awk '/total/{print $2}'  \n");

    for (n=0; n<20; n++)  {
        sleep(4);                       /* wait 10s before each new block allocation  */
        parray[n] = (char *) malloc(BNUM);           /* claiming BNUM bytes of space  */
        if (parray[n] == NULL) {
            fprintf(stderr, "--> Error! Memory allocation for block %d failed. \n", n);
            return(-(n+1));
        } else
            printf("    block %d allocated. \n", n+1);
    }

    printf("--> Now release the allocated memory blocks in 2s time steps: \n");
    for (n=19; n>=0; n--)  {
        sleep(2);                     /* wait 2s before giving each block back to OS  */
        free(parray[n]);
        printf("    block %d freed. \n", n+1);
    }

    printf("--> All allocated memory blocks released - termination in 30s. \n");
    sleep(28);           /* wait 28s with all memory released before removing marker  */
    system("rm memalloc2_started");                            /* remove marker file  */
    sleep(2);                                          /* wait 2s before termination  */

    return(0);
}
