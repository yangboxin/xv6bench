#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"


int iter,flag; /* global variables */

/* function that return result */
void reporter() 
{
 printf("%d\n", iter);
 flag=1;
 sigreturn();
}

/* main test function */	
int main(int argc, char * argv[])
{
 int	duration; 
 int	check;
 int	p1[2], p2[2];
 /* set up alarm call */
 iter = 0;flag=0;
 if (argc != 2)
   {
    printf("Usage: %s duration\n", argv[0]);
    exit(1);
   }
 duration = atoi(argv[1]); /* takes execution time from program argv params */
 sigalarm(duration, reporter);  /* end test execution in <duration> minutes and betrays management to <report> function*/
 if (pipe(p1) || pipe(p2)) /* if create of two pipes failed */
   {
    printf("pipe create failed"); /* print erroe message */
    exit(1); /* terminate test execution */
   }
 if (fork()) /* create process if it is parent */
   { /* master, write p1 & read p2 */
    close(p1[0]);
    close(p2[1]);
    while (flag != 1) /* until report is not executed do */
      {
       /* master write through pipes */
       if (write(p1[1], (char *) & iter, sizeof(iter)) != sizeof(iter))
         {
          exit(1);
         }
       /* master read through pipes */
       if (read(p2[0], (char *) & check, sizeof(check)) != sizeof(check))
         {
          exit(1);
         }
       if (check != iter)
         {
          printf("Master sync error: expect %d, got %d\n", iter, check);
          exit(2);
         }
       iter++; /* increase number of test cycles */
      }
   }
 else /* if created process is child */
   {
    int	iter1;
    iter1 = 0;
	/* slave, read p1 & write p2 */
    close(p1[1]);
    close(p2[0]);
    while (flag != 1)
      {
       /* slave read through pipes */
       if (read(p1[0], (char *) & check, sizeof(check)) != sizeof(check))
         {
          exit(1);
         }
       if (check != iter1)
         {
          printf("Slave sync error: expect %d, got %d\n", iter, check);
          exit(2);
         }
       /* slave write through pipes */
       if (write(p2[1], (char *) & iter1, sizeof(iter1)) != sizeof(check))
         {
          exit(1);
         }
       iter1++;
      }
   }
   sigalarm(0,0);
 return(0);
}


