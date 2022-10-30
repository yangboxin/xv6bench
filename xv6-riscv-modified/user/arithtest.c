#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"

int iter,flag;

void reporter(){
    printf("%d\n",iter);
    flag=1;
    sigreturn();
}

void arithint1()
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

void iterpp(){
    iter++;
}

int main(int argc, char * argv[]){
    if (argc != 3)
    {
        printf("Usage: %s duration test_type\n", argv[0]);
        exit(1);
    }
    iter=0,flag=0;
    int duration=atoi(argv[1]);
    int test_type=atoi(argv[2]);
    printf("testtype: %d\n",test_type);
    sigalarm(duration,reporter);
    while(flag!=1){
        arithint1();
    // staight iter++ won't work and i have no idea why.
    // This was supposed to be a debug hook but it worked       
        iterpp(); 
        //iter=iter+1; 
    }
    sigalarm(0,0);
    return 0;
}