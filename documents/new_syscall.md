### github仓库地址：

https://github.com/yangboxin/xv6bench



### 在xv6中新增系统调用的注意事项：

1. 在user/usys.pl中添加系统调用的entry条目，usys.pl会生成一个usys.S，它能将用户的系统调用route到内核空间。在user.h中添加声明。
1. 内核空间的系统调用统一通过syscall.c处理，它会处理系统调用的参数以及从进程的a7寄存器中获取系统调用的编码，调用对应的系统调用并将系统调用返回值存储在a0寄存器。在添加系统调用时，需在syscall.c和syscall.h中添加系统调用的声明以及系统调用的对应编码。
1. 在sysproc.c中添加系统调用的相应代码。



### sys_getuid()实现

1. 在proc.h的struct proc中添加int uid
2. 在proc.c的static struct proc* allocproc(void)中分配uid给进程
3. 在sysproc.c中实现sys_getuid(),返回调用getuid的进程的uid
4. 在usys.pl中添加getuid的entry，在user.h中添加getuid的声明
5. 在syscall.h中#define SYS_getuid 22,在syscall.c中添加extern引用sysproc中的getuid原型，在mapping中加入SYS_getuid



#### 测试getuid效果

在user目录下添加testuid.c，调用getuid，将得到的uid打印到stdout中，注意在Makefile中将testuid添加到UPROG下



### alarm

实现sigalarm、sigreturn两个系统调用，用于控制程序运行时长。

sigalarm接受两个参数：

sigalarm(int tick, void (*func)());

**表示在tick后调用func**

#### 实现过程：

1. 在proc.h中添加alarm_interval（过去多少个ticks后调用handler），alarm_passed（过去了多少个tick），alarm_handler（handler的指针），struct trapframe etpfm（用于储存原进程的寄存器）
2. 在proc.c的allocproc()中初始化上述的数据
3. 在sysproc.c中实现sigalarm、sigreturn，sigalarm获取两个参数tick、func，并将调用它的进程的alarm_interval和alarm_handler分别设为tick和func；sigreturn会返回原始的进程
4. 在trap.c中，遇到一个时钟中断时，查看当前进程的alarm_interval是否大于0，若大于0，查看此进程已经经过的tick是否达到预设的interval，若达到，调用handler程序
5. 在syscall.c和syscall.h中添加相应的声明和引用
6. 在uses.pl,user.h中添加entry和声明



#### 测试sigalarm

在testuid中，加入一个**reporter()**作为**sigalarm()**的handler，设置一个iter记录在给定tick下**getuid()**的调用次数，并在**reporter()**中打印iter，最后在**reporter()**中调用**sigreturn()**回到main

