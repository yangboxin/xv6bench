### unixbench-code-r7

#### 测试项目：

1. ##### arithm test(arith.c)

   测试内容：针对6种不同类型(register int, short, int, long, float, double)数字的算术运算（赋值、加减乘除）。

   usage：arith $duration $test_type：在duration秒内执行第test_type个测试。

   测试指标：test执行次数iter

   xv6实现：可参照原始实现

2. ##### Dhrystone 2 test(dhry1.c, dhry2.c)

   测试内容：测试聚焦在字符串处理，没有浮点运算操作。这个测试用于测试链接器编译、代码优化、内存缓存、等待状态、整数数据类型等，硬件和软件设计都会非常大的影响测试结果。

   usage：dhry $duration

   测试指标：test执行次数iter

   xv6实现：可参照原始实现

3. ##### Hanoi test(hanoi.c)

   测试内容：经典汉诺塔问题。

   usage：hanoi $duration $disk：duration-运行时长，disk-盘子数量

   测试指标：解一次$disk个盘子的hanoi函数的执行次数iter

   xv6实现：可参照原始实现

4. ##### Fstime test(fstime.c)

   测试内容：write test, read test, copy test

   usage：fstime $duration $buffer_size $max_mblock: duration-运行时长，buffer_size-缓冲区大小，max_mblock-最大块数

   测试指标：write_score, read_score, copy_score（与文件读/写/复制的次数、时长有关）

   xv6实现:需实现sync(),lseek()两个系统调用，其中sync()用于将所有缓冲区数据写入文件系统中,lseek()用于重置一个file descriptor的offset。

5. ##### Process creation test(spawn.c)

   测试内容：测试每秒钟一个进程可以创建子进程然后收回子进程的次数（子进程一定立即退出）

   usage：spawn $duration：duration-运行时长

   测试指标：进程在$duration秒内创建子进程然后收回子进程的次数iter

   xv6实现：已有现成的fork()

6. ##### System function call test(syscall.c)

   测试内容：测试进入和离开操作系统内核的代价，即一次系统调用的代价。它利用一个反复地调用 getpid 函数的小程序达到此目的。

   usage：syscall $duration：duration-运行时长

   测试指标：在$duration秒内执行{getpid(), getuid(), umask(022)}的次数iter

   xv6实现：需实现getuid(), umask()；其中:

   - getuid()：returns the real user ID of the calling process
   - umask()：set file mode creation mask

7. ##### Exec shell scripts test(looper.c)

   测试内容：测试一定时间内一个进程可以并发地开始一个 shell 脚本的 n 个拷贝的次数

   usage：looper $duration multi.sh：duration-运行时长，multi.sh-脚本文件

   测试指标：脚本执行的次数iter

   xv6实现：需要对execvp()做一定的处理,execvp()是execve()的front-end,execve()以NULL作为参数列表的结尾（UNIX V7），而xv6实现的exec以UNIX V6为准，参数列表以0作为结尾。

8. ##### Pipe-based context switch test(context.c)

   测试内容：这个测试两个进程（每秒钟）通过一个管道交换一个不断增长的整数的次数。

   usage：context $duration：duration-运行时长

   测试指标：在管道中不断交换并在每个进程中都增长的iter

   xv6实现：可参照原始实现

9. ##### Pipe throughput test(pipe.c)

   测试内容：Pipe throughtput 指的是一段时间内一个进程可以向一个管道写 512 字节数据然后再读回的次数。

   usage：pipe $duration：duration-运行时长

   测试指标：在duration时长内进程向管道写入数据并读回的次数iter

   xv6实现：可参照原始实现

10. ##### Execl test(execl.c)

    测试内容：一段时间内execl()可以执行的次数

    usage：execl $duration：duration-运行时长

    测试指标：execl的执行次数iter

    xv6实现：execl()与execvp()一样，是execve()的众多front-end之一，同样需要对sys_exec()做处理



同时，对于几乎所有测试中都出现的printf(),perror(),exit(),sprint(),signal(),alarm()等非系统调用的库函数也需要特别注意：其在使用时可能会调用系统调用，比如printf()会调用write().