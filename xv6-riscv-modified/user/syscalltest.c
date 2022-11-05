#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"


int	iter,flag; /* global variables */

/* function that return result */
void reporter()
{
 printf("iteration:%d\n", iter);
 flag = 1;
 sigreturn();
}

/* main test function */
int main(int argc, char * argv[])
{
 int	duration;
 iter = 0;
 flag = 0;
 if (argc != 2)
   {
    printf("Usage: %s duration\n", argv[0]);
    exit(1);
   }
 duration = atoi(argv[1]); /* takes execution time from program argv params */
 sigalarm(duration, reporter); /* end test execution in <duration> minutes and betrays management to <report> function*/
 while (flag != 1) /* until report is not executed do */
   {
    close(dup(0));
    getpid();
    getuid();
    getpid();
    //umask(022);
    iter++; /* increase number of test cycles */
   }
 return(0);
}

