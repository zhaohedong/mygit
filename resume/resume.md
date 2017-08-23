### ceph on arm
- 作业内容
    - Arm分布式文件存储服务器，作业内容包括硬件选型、系统选型与定制、内核编译与裁剪、ceph的交叉编译、集群的部署、维护、测试、优化。
    - ceph是开源的分布式存储系统，支持块存储、文件存储、对象存储。
- 硬件选型
    - Marvell ARMADA-38x
    - 双核CPU（与或运算加速器）
    - 2G内存（OSD）
    - 双千兆网卡
        - 集群网络
        - 外部网络
    - 双硬盘接口
    - 电源管理
        - ipmitool的使用
- 系统
    - 系统选型
        - Ubuntu 14.04 armhf
    - 内核
        - 内核的编译裁剪>3.13.0
        - 内核模块的追加
    - Bootloader的使用
        - dtb、zimage加载
        - nfs根文件系统加载
    - 系统备份与恢复
- 交叉编译
    - 交叉编译工具链选型
        - gcc4.8 对C++11的支持
        - cmake4.8.0-rc2 升级到最新版
    - 交叉编译环境配置
        - Makefile解析、修改
        - Cmake环境配置
    - Troubleshooting
        - boost库编译
        - 容量削减 strip
        - ceph对交叉编译支持性的改善
            - 缺少库、缺少头文件
        - ceph交叉编译安装支持性的改善
- ceph软件
    - 部署
        - ceph-deploy工具使用，源代码解析、调试
        - Shell脚本编写
        - 自动化运维工具ansible使用
        - ssh工具使用
            - sshkeygen、ssh-copy-id
            - 免密登录
        - ntp部署
        - iptables使用、6789端口开放
    - 维护
    - TroubleShooting
        - linux log的解析
            - /var/log/ceph/*
            - /var/log/dmesg
        - ceph log各级别log的打开与关闭
        - 权限问题
        - 文件的存在性问题
        - 程序自启动
            - /etc/init/*.conf
            - /var/log/upstart解析
        - 硬盘开机自动挂载
            - /etc/fstab
            - /lib/udev/rules.d/*.rules
    - 测试
        - fio测试
            - IOPS
            - 吞吐量
        - dd测试
        - iperf网络测试
        - osd perf集群网络测试
        - 单节点拔出测试
    - 优化
        - 文件系统的选型 ext4/xfs/btrfs
        - 内网、外网隔离
        - pg、pgp调整
        - crushmap调整
            - 故障域的分离
        - ssd
            - Journal
            - Cache Tier
        - 冗余模式纠删码
- 其他
    - vim使用
    - git使用
    - gdb使用
    - tftp服务器搭建与使用
### Intalogic
- 作业内容
    - Intalogci Run端CPU更换
    - Run端本身相当于一个解释器+编译器+执行器，Editor端用户编写ladder程序，Editor端生成中间码，通过串口Editor将中间码传送给Run端，Run端解析中间码，生成对应的机器码，将机器码写入内存的执行代码Section，CPU执行指针从执行Section开始执行机器码。
- 硬件环境
    - 瑞萨H8SX2600->RX630
    - H8SX2600
        - CISC复杂指令集
        - 没有奇数字节指令
    - RX630
        - RISC简单指令集
        - 有奇数字节指令
    - CPU对内存的写入都是以Word为单位
- IDE
    - CubeSuite++
