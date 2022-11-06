本周完成context、pipe、syscall、hanoi、spawn五个测试的移植，其实现效果如下：

```bash
$ spawn 5
iteration:34318
$ spawn 10
iteration:56022
$ pipetest 5
44021
$ pipetest 10
63179
$ syscalltest 5
iteration:13362
$ syscalltest 10
iteration:53658
$ hanoi 5 3
iteration:1022706
$ hanoi 10 3
iteration:2449678
$ context 5
24199
$ context 10
64126

```

下周计划在printf中添加“%lu”选项，可以输出unsigned long型数据；画出sigalarm运行流程图；实现looper、execl测试（涉及execvp、execl的实现）。