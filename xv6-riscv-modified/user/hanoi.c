/* hanoi towers test hanoi.c source file */
#define	other(i,j) (6-(i+j))
#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"

void mov(int n, int f, int t);

/* global variables */
unsigned long	iter,flag;
int	num[4];
long	cnt;

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
 int	duration,disk;
 if (argc != 3)
   {
    printf("Usage: %s duration disk\n", argv[0]);
    exit(1);
   }
 duration = atoi(argv[1]); /* takes execution time from program argv params */
 disk = atoi(argv[2]); /* takes number of disks from program argv params */
 num[1] = disk;
 flag=0;
 sigalarm(duration, reporter); /* end test execution in <duration> minutes and betrays management to <report> function*/
 while (flag != 1) /* until report is not executed do */
   {
    mov(disk,1,3);
    iter++; /* increase number of test cycles */
   }
   sigalarm(0,0);
 return(0);
}

/* disk moving function */
void mov(int n, int f, int t)
{
 int	o;
 if (n == 1)
   {
    num[f]--;
    num[t]++;
    return;
   }
 o = other(f,t);
 mov(n-1,f,o);
 mov(1,f,t);
 mov(n-1,o,t);
}


