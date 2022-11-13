#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"


unsigned long	iter,flag; /* global variables */

/* function that return result */
void reporter()
{
 printf("iteration:%l\n", iter);
 flag = 1;
 sigreturn();
}
	
/* main test function */
int main(int argc, char * argv[])
{
 int	duration;	
 int	slave;
 //int	status;
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
    if ((slave = fork()) == 0)
      {
       exit(0);
      }
    else
      if (slave < 0)
        {
         printf("Fork failed at iteration %d\n", iter);
         // perror("Reason");
         // exit(2);
         continue;
        }
      else{
        if(wait(0)<0){
            printf("wait stopped early\n");
            exit(1);
        }
      }
    /*if (status != 0)
      {
       printf("Bad wait status: 0x%x\n", status);
       exit(2);
      }*/
    iter++; /* increase number of test cycles */
   }
   sigalarm(0,0);
 return(0);
}


