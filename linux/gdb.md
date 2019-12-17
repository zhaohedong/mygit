- gdb调试技巧
	- info b 查看所有断点
	- set print pretty 设置结构体显示
	- b file:line 断点
	- list file:line
	- list function
	- set remote debug 1 //设置调试模式，输出gdb信息
	- b file:line if xxx == xxx
    - disassemble 技巧
    	```
		gdb vmlinux
  		disassemble /sr function
		```
- linux crash
	- call stack 形如function+offset/length
	- kdump服务开启
    	- config
		```
		CONFIG_DEBUG_INFO=y
		CONFIG_CRASH_DUMP=y
		CONFIG_PROC_VMCORE=y
		```
		- grub
    		- crashkernel=auto / crashkernel=128M
    	- ref
        	- https://wiki.archlinux.org/index.php/Kdump
    	- enable kdump
        	- systemctl enable kdump
    	- check status 
        	- cat /sys/kernel/kexec_crash_loaded
      	- test crash
        	- echo c > /proc/sysrq-trigger
      	- analyze crash
        	- crash vmlinux /var/crash/xxx/vmcore
        	- vi /var/crash/xxx/vmcore-dmesg.log