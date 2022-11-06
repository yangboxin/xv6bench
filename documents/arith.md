### 10.30

上个版本的sigalarm实现存在一个参数bug，无法正确读取handler的地址，经验证是argaddr没有正确设置，修复了此bug；将proc里用于暂存trapframe的alarm_trapframe修改为动态分配空间；将sigalarm和sigreturn的具体实现移到了kernel/trap.c中。

```diff
diff --git a/xv6-riscv-modified/Makefile b/xv6-riscv-modified/Makefile
index 3448676..add4c93 100644
--- a/xv6-riscv-modified/Makefile
+++ b/xv6-riscv-modified/Makefile
@@ -134,6 +134,8 @@ UPROGS=\
 	$U/_wc\
 	$U/_zombie\
 	$U/_testuid\
+	$U/_arith\
+	$U/_arithtest\
 
 fs.img: mkfs/mkfs README $(UPROGS)
 	mkfs/mkfs fs.img README $(UPROGS)
diff --git a/xv6-riscv-modified/kernel/defs.h b/xv6-riscv-modified/kernel/defs.h
index a3c962b..80c998d 100644
--- a/xv6-riscv-modified/kernel/defs.h
+++ b/xv6-riscv-modified/kernel/defs.h
@@ -147,6 +147,8 @@ void            trapinit(void);
 void            trapinithart(void);
 extern struct spinlock tickslock;
 void            usertrapret(void);
+int             sigalarm(int,void(*)());
+int             sigreturn(void);
 
 // uart.c
 void            uartinit(void);
diff --git a/xv6-riscv-modified/kernel/proc.c b/xv6-riscv-modified/kernel/proc.c
index 96b8b3d..b40fd4d 100644
--- a/xv6-riscv-modified/kernel/proc.c
+++ b/xv6-riscv-modified/kernel/proc.c
@@ -131,7 +131,11 @@ found:
     release(&p->lock);
     return 0;
   }
-
+  
+  if((p->alarm_trapframe=(struct trapframe *)kalloc())==0){
+    release(&p->lock);
+    return 0;
+  }
   // An empty user page table.
   p->pagetable = proc_pagetable(p);
   if(p->pagetable == 0){
@@ -148,8 +152,9 @@ found:
 
   // sigalarm init
   p->alarm_interval = 0;
-  p->alarm_passed = 0;
+  p->alarm_ticks = 0;
   p->alarm_handler = 0;
+  p->alarm_goingoff=0;
   
   return p;
 }
@@ -173,6 +178,13 @@ freeproc(struct proc *p)
   p->chan = 0;
   p->killed = 0;
   p->xstate = 0;
+  if(p->alarm_trapframe)
+    kfree((void*)p->alarm_trapframe);
+  p->alarm_trapframe=0;
+  p->alarm_interval=0;
+  p->alarm_handler=0;
+  p->alarm_ticks=0;
+  p->alarm_goingoff=0;
   p->state = UNUSED;
 }
 
diff --git a/xv6-riscv-modified/kernel/proc.h b/xv6-riscv-modified/kernel/proc.h
index df58fa0..f17b699 100644
--- a/xv6-riscv-modified/kernel/proc.h
+++ b/xv6-riscv-modified/kernel/proc.h
@@ -107,7 +107,8 @@ struct proc {
 
   // sigalarm related
   int alarm_interval;          // alarm interval (ticks)
-  int alarm_passed;            // ticks passed since last call
-  uint64 alarm_handler;        // pointer to the alarm handler function
+  int alarm_ticks;            // ticks passed since last call
+  void (*alarm_handler)();        // alarm handler function
+  int alarm_goingoff;             // prevent re-entrance
   struct trapframe *alarm_trapframe;      // trapframe to resume after return
 };
diff --git a/xv6-riscv-modified/kernel/sysproc.c b/xv6-riscv-modified/kernel/sysproc.c
index bd4c0fc..d7de2ee 100644
--- a/xv6-riscv-modified/kernel/sysproc.c
+++ b/xv6-riscv-modified/kernel/sysproc.c
@@ -107,20 +107,11 @@ sys_sigalarm(void)
   argint(0, &interval);
   argaddr(1, &handler);
 
-  struct proc *p=myproc();
-
-  p->alarm_interval=interval;
-  p->alarm_handler=handler;
-  return 0;
+  return sigalarm(interval, (void(*)())(handler));
 }
 
 uint64
 sys_sigreturn(void)
 {
-  struct proc *p=myproc();
-  memmove(p->trapframe,p->alarm_trapframe,sizeof(struct trapframe));
-  p->alarm_passed=0;
-  p->alarm_interval=0;
-  p->alarm_on=0;
-  return 0;
+  return sigreturn();
 }
\ No newline at end of file
diff --git a/xv6-riscv-modified/kernel/trap.c b/xv6-riscv-modified/kernel/trap.c
index 8108c96..059d780 100644
--- a/xv6-riscv-modified/kernel/trap.c
+++ b/xv6-riscv-modified/kernel/trap.c
@@ -79,8 +79,15 @@ usertrap(void)
   // give up the CPU if this is a timer interrupt.
   if(which_dev == 2){
     // printf("%d\n",r_scause());
-      if(p->alarm_interval>0){
-        
+      if(p->alarm_interval!=0){
+        if(--p->alarm_ticks<=0){
+          if(!p->alarm_goingoff){
+            p->alarm_ticks=p->alarm_interval;
+            *p->alarm_trapframe=*p->trapframe;
+            p->trapframe->epc=(uint64)p->alarm_handler;
+            p->alarm_goingoff=1;
+          }
+        }
       }
       yield();
   }
@@ -224,3 +231,17 @@ devintr()
   }
 }
 
+int sigalarm(int ticks,void(*handler)()){
+  struct proc *p=myproc();
+  p->alarm_interval=ticks;
+  p->alarm_handler=handler;
+  p->alarm_ticks=ticks;
+  return 0;
+}
+
+int sigreturn(){
+  struct proc* p=myproc();
+  *p->trapframe=*p->alarm_trapframe;
+  p->alarm_goingoff=0;
+  return 0;
+}
\ No newline at end of file
```





参照unix-bench实现的arith测试。

在main的while循环中，当使用iter++语句进行计数时，存在未知bug，导致程序陷入向console不断写入0的循环，在调试的过程中，发现程序无法正常进入while，在sigalarm后便陷入printf("%d",iter)的死循环，但若将iter++改为一个函数，则可以正常运行，原因未知。

```diff
diff --git a/xv6-riscv-modified/user/arith.c b/xv6-riscv-modified/user/arith.c
new file mode 100644
index 0000000..025d430
--- /dev/null
+++ b/xv6-riscv-modified/user/arith.c
@@ -0,0 +1,159 @@
+#include "kernel/param.h"
+#include "kernel/types.h"
+#include "kernel/stat.h"
+#include "kernel/riscv.h"
+#include "user/user.h"
+
+long iter,flag;
+
+void reporter(){
+    printf("%d\n",iter);
+    flag=1;
+    sigreturn();
+}
+
+/* function that implements arithmetic operations on register int variables */
+void arithreg()
+{
+ register int	x, y, z;
+ z = 0;
+ for (int i = 2; i <= 101; i++)
+   {
+    x = i;
+    y = x*x;
+    z += y/(y-1);
+   }
+   return;
+}
+
+/* function that implements arithmetic operations on short variables */
+void arithshort()
+{
+ short	x, y, z;
+ z = 0;
+ for (int i = 2; i <= 101; i++)
+   {
+    x = i;
+    y = x*x;
+    z += y/(y-1);
+   }
+    return;
+}
+
+/* function that implements arithmetic operations on int variables */
+void arithint()
+{
+ int	x, y, z;
+ z = 0;
+ for (int i = 2; i <= 101; i++)
+   {
+    x = i;
+    y = x*x;
+    z += y/(y-1);
+   }
+   return;
+}
+
+/* function that implements arithmetic operations on long variables */
+void arithlong()
+{
+ long	x, y, z;
+ z = 0;
+ for (int i = 2; i <= 101; i++)
+   {
+    x = i;
+    y = x*x;
+    z += y/(y-1);
+   }
+   return;
+}
+
+/* function that implements arithmetic operations on float variables */
+void arithfloat()
+{
+ float	x, y, z;
+ z = 0;
+ for (int i = 2; i <= 101; i++)
+   {
+    x = i;
+    y = x*x;
+    z += y/(y-1);
+   }
+   return;
+}
+
+/* function that implements arithmetic operations on double variables */
+void arithdouble()
+{
+ double	x, y, z;
+ z = 0;
+ for (int i = 2; i <= 101; i++)
+   {
+    x = i;
+    y = x*x;
+    z += y/(y-1);
+   }
+   return;
+}
+
+/* function that implements empty sycle without any arithmetic operations */
+void arithoh()
+{
+ for (int i = 2; i <= 101; i++);
+ return;
+}
+
+void iterpp(){
+  iter++;
+}
+
+/* main test function */
+int main(int argc, char * argv[])
+{
+ int	duration, test_type;
+ iter = 0;
+ flag = 0;
+ if (argc != 3)
+   {
+    printf("Usage: %s duration test_type\n", argv[0]);
+    exit(1);
+   }
+ duration = atoi(argv[1]); /* takes execution time from program argv params */
+ test_type = atoi(argv[2]); /* takes test number to execute from program argv params */		
+ sigalarm(duration, reporter); /* end test execution in <duration> minutes and betrays management to <report> function*/
+ while (flag != 1) /* until report is not executed do */
+   {
+    iterpp(); /* increase number of test cycles */
+    switch (test_type) /* execute test of specified in argv params type */
+      {
+       case 1:
+         arithoh();
+       break;
+       case 2:
+         arithreg();
+       break;
+       case 3:
+         arithshort();
+       break;
+       case 4:
+         arithint();
+       break;
+       case 5:
+         arithlong();
+       break;
+       case 6:
+         arithfloat();
+       break;
+       case 7:
+         arithdouble();
+       break;
+       default:
+         printf("number of function that you chose is invalid\n"); /* if test number you has chosen is not in [1..7] print message */
+         return 0;
+      }        
+   }
+   sigalarm(0,0);
+ return 0;
+}
```



arith duration test_type

其中duration表示n个tick，在xv6中“A tick is a fairly arbitrary unit of time in xv6, determined by how often a hardware timer generates interrupts.”而这个hardware timer为100ms。

在arith中不同test_type分别代表：

- 1-空循环
- 2-register int型数计算
- 3-short型数计算
- 4-int型数计算
- 5-long型数计算
- 6-float型数计算
- 7-double型数计算



以下是arith运行的效果

```
xv6 kernel is booting

hart 2 starting
hart 1 starting
init: starting sh
$ arith 10 1
38017007
$ arith 10 2
36641114
$ arith 10 3
35732178
$ arith 10 4
38146397
$ arith 10 5
35315179
$ arith 10 6
37349412
$ arith 10 7 
35580364
$ arith 10 8
number of function that you chose is invalid
```