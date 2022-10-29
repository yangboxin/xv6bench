#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"

int iter,flag;

void reporter(){
    printf("iteration: %d\n",iter);
    flag=1;
    sigreturn();
}

int main(int argc, char * argv[]){
    if (argc != 2)
    {
        printf("Usage: %s duration\n", argv[0]);
        exit(1);
    }
    iter=0,flag=0;
    int duration = atoi(argv[1]);
    sigalarm(duration,reporter);
    while(flag!=1){// end in duration ticks
        getpid();
        getuid();
        iter++;
    }
    sigalarm(0,0);
    return 0;
}