# 内核调试方法

## 前言
开发者认为内核发生错误时，就不应该继续往下执行，内核的行为被判定成系统崩溃，需要机器重启。

## 内核的调试方法
- 使用printk打印调试信息
- 使用内核转存机制，保留发生问题时的内核执行状态，进行事后分析
- 使用单步调试方法

## Kdump的定义
Kdump 是一种基于 kexec 的内存转储工具，目前它已经被内核主线接收，成为了内核的一部分，它也由此获得了绝大多数 Linux 发行版的支持。与传统的内存转储机制不同不同，基于 Kdump 的系统工作的时候需要两个内核，一个称为系统内核，即系统正常工作时运行的内核；另外一个称为捕获内核，即正常内核崩溃时，用来进行内存转储的内核。 在本文稍后的内容中，将会介绍如何设置 kump。
## Kdump依赖的CONFIG设定
```
CONFIG_KEXEC=y 
CONFIG_SYSFS=y 
CONFIG_DEBUG_INFO=y
CONFIG_CRASH_DUMP=y 
CONFIG_PROC_VMCORE=y 
```
## Kdump如何生成vmcore
启动参数指定crashkernel=auto
```
linux16 /vmlinuz-4.18.0-147.3.1.el8_1.x86_64 root=/dev/mapper/centos-root ro crashkernel=auto rd.lvm.lv=centos/root rd.lvm.lv=centos/swap rhgb quiet LANG=en_US.UTF-8
```
## 内核转存使用
需要安装的rpm包
```
sudo rpm -ivh kernel-debuginfo-common-x86_64-4.18.0-147.3.1.el8_1.x86_64.rpm
sudo rpm -ivh kernel-debuginfo-4.18.0-147.3.1.el8_1.x86_64.rpm
```
使用最新的crash 7.2.8++
```
 sudo /opt/usr/bin/crash /usr/lib/debug/usr/lib/modules/4.18.0-147.3.1.el8_1.x86_64/vmlinux vmcore
```

## 一般应用如何使用coredump
- ref:https://www.cnblogs.com/zlcxbb/p/6431191.html
- 检查内核转存是否有效
    - ulimit -c
- 改为1G
    - ulimit -c 1073741824
- 改为无限制
    - ulimit -c unlimited
- 测试程序
    ```
    #include <stdio.h>
    int main(void)
    {
        int *a = NULL;
        *a = 0x1;
        return 0;
    }
    ```

- 调试转存文件
    ```
    gdb -c ./*.core ./a.out
    ```
## 关于Panic是如何产生的
panic()是主动产生的，在内核执行发生错误时主动调用panic()函数，另系统宕机重启
## 手动触发panic的方法
先取得panic函数的虚拟地址
```
cat /proc/kallsyms | grep -w panic
ffffffffb2d5b3e8 T panic
```
之后在内核模块中直接调用panic函数
```
void (*func)(void) = 0xffffffffb2d5b3e8;
func();
```
## 通过内存文件系统触发panic
```
echo c > /proc/sysrq-trigger
```
## 如何单步调试内核
目标机内核参数
```
kgdboc=kbd,ttyS0,9600 kgdbwait
```
目标机CONFIG设定
```
CONFIG_KGDB = y
CONFIG_KGDB_SERIAL_CONSOLE = y
```
调试机文件要求
```
linux内核源码及vmlinux
```
调试机命令
```
gdb vmlinux
set remotebaud 9600 
target remote /dev/ttyUSBS0
```
## 串口的测试方法
dmesg查看哪个串口存在
发送端
```
echo 111 > /dev/ttyS0
```
接收端
```
cat /dev/ttyUSB0
```



