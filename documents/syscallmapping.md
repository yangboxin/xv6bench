| Unixbench-syscall                   | 所属文件         | Xv6-syscall   |
| ----------------------------------- | ---------------- | ------------- |
| exit()                              | *                | sys_exit()    |
| pipe()                              | context.c,pipe.c | sys_pipe()    |
| fork()                              | context.c        | sys_fork()    |
| close()                             | context.c        | sys_close()   |
| write()                             | context.c        | sys_write()   |
| read()                              | context.c,pipe.c | sys_read()    |
| execl()                             | execl.c          | sys_exec()    |
| getpid()                            | fstime.c         | sys_getpid()  |
| sleep()                             | fstime.c         | sys_sleep()   |
| lseek()#seek within a file          | fstime.c         | 无对应syscall |
| sync()#commit data to buf           | fstime.c         | 无对应syscall |
| unlink()                            | fstime.c         | sys_unlink()  |
| wait()                              | looper.c         | sys_wait()    |
| execvp()                            | looper.c         | sys_exec()    |
| umask()#set file mode creation mask | syscall.c        | 无对应syscall |
| getuid()#user identity              | syscall.c        | 无对应syscall |

##### Reference:https://linux.die.net/man/