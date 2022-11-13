#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"


/* global variables */
unsigned long	iter,flag;

/* function that return result */
void reporter()
{
 printf("%l\n", iter);
 flag = 1;
 sigreturn();
}

/* main test function */
int main(int argc, char * argv[])
{
 int	duration;
 char	buf[512];
 int	pvec[2];
 if (argc != 2)
 {
  printf("Usage: %s duration\n", argv[0]);
  exit(1);
 }	
 duration = atoi(argv[1]); /* takes execution time from program argv params */
 pipe(pvec);
 flag = 0;
 sigalarm(duration, reporter); /* end test execution in <duration> minutes and betrays management to <report> function*/
 iter = 0;
 while (flag != 1) /* until report is not executed do */
   {
    if (write(pvec[1], buf, sizeof(buf)) != sizeof(buf))
      {
       //printf("write file error\n");
       break;
      }
    if (read(pvec[0], buf, sizeof(buf)) != sizeof(buf))
      {
       //printf("read file error\n");
       break;
      }
    iter++; /* increase number of test cycles */
   }
   sigalarm(0,0);
 return(0);
}

