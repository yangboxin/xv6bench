#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"

unsigned long iter,flag;

void reporter(){
    printf("%l\n",iter);
    flag=1;
    sigreturn();
}

/* function that implements arithmetic operations on register int variables */
void arithreg()
{
 register int	x, y, z;
 z = 0;
 for (int i = 2; i <= 101; i++)
   {
    x = i;
    y = x*x;
    z += y/(y-1);
   }
   return;
}

/* function that implements arithmetic operations on short variables */
void arithshort()
{
 short	x, y, z;
 z = 0;
 for (int i = 2; i <= 101; i++)
   {
    x = i;
    y = x*x;
    z += y/(y-1);
   }
    return;
}

/* function that implements arithmetic operations on int variables */
void arithint()
{
 int	x, y, z;
 z = 0;
 for (int i = 2; i <= 101; i++)
   {
    x = i;
    y = x*x;
    z += y/(y-1);
   }
   return;
}

/* function that implements arithmetic operations on long variables */
void arithlong()
{
 long	x, y, z;
 z = 0;
 for (int i = 2; i <= 101; i++)
   {
    x = i;
    y = x*x;
    z += y/(y-1);
   }
   return;
}

/* function that implements arithmetic operations on float variables */
void arithfloat()
{
 float	x, y, z;
 z = 0;
 for (int i = 2; i <= 101; i++)
   {
    x = i;
    y = x*x;
    z += y/(y-1);
   }
   return;
}

/* function that implements arithmetic operations on double variables */
void arithdouble()
{
 double	x, y, z;
 z = 0;
 for (int i = 2; i <= 101; i++)
   {
    x = i;
    y = x*x;
    z += y/(y-1);
   }
   return;
}

/* function that implements empty sycle without any arithmetic operations */
void arithoh()
{
 for (int i = 2; i <= 101; i++);
 return;
}

void iterpp(){
  iter++;
}

/* main test function */
int main(int argc, char * argv[])
{
 int	duration, test_type;
 iter = 0;
 flag = 0;
 if (argc != 3)
   {
    printf("Usage: %s duration test_type\n", argv[0]);
    exit(1);
   }
 duration = atoi(argv[1]); /* takes execution time from program argv params */
 test_type = atoi(argv[2]); /* takes test number to execute from program argv params */		
 sigalarm(duration, reporter); /* end test execution in <duration> minutes and betrays management to <report> function*/
 while (flag != 1) /* until report is not executed do */
   {
    iterpp(); /* increase number of test cycles */
    switch (test_type) /* execute test of specified in argv params type */
      {
       case 1:
         arithoh();
       break;
       case 2:
         arithreg();
       break;
       case 3:
         arithshort();
       break;
       case 4:
         arithint();
       break;
       case 5:
         arithlong();
       break;
       case 6:
         arithfloat();
       break;
       case 7:
         arithdouble();
       break;
       default:
         printf("number of function that you chose is invalid\n"); /* if test number you has chosen is not in [1..7] print message */
         return 0;
      }        
   }
   sigalarm(0,0);
 return 0;
}


