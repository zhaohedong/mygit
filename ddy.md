- 关于双网卡使用某一网卡访问网页
    - nslookup www.xxx.com 取得ip地址 x.x.x.x和网关地址 y.y.y.y
    - sudo route add -net x.x.x.x/24 gw y.y.y.y eth0
- 当4m顺序写吞吐量1000MB/s时候，iostat 工具显示250，ceph -s 显示500，grafana显示1000
- Performance
    - pg log
- NAS
    - Network Attached Storge
    - 本质上相当于File Server，通常支持TCP/IP，提供NFS、SAMBA、FTP等常见通讯协议，让用户获取档案系统。
    - 有Web管理界面，管理磁盘网络等
- SAN
    - Storge Area Network
    - 可看作外接式存储设备，提供磁盘给主机使用
- 制作patch和使用patch
    - 制作
        - diff -Naur kernel/    kernel_new/ > zwx-lcd.patch
    - 使用
        - patch -p1<../zwx-lcd.patch
        - p1表示忽略第一个“/”前的路径，因为本地文件夹名字通常与制作patch时候的文件夹名不相同，所以使用-p参数
- 机架式服务器
    - 1U12 1U指的是1U=1.75英寸=4.445cm高,12盘位
- umount 报出device is busy
    - fuser -km /mnt
    - umount /mnt
- fs_commit_latency
    - plb.add_time_avg(l_os_j_lat, "journal_latency", "Average journal queue completing latency");
- fs_apply_latency
    - plb.add_time_avg(l_os_apply_lat, "apply_latency", "Apply latency");
- ```
    ceph osd pool create xfs_rbd_pool 1024 1024 replicated
    rbd pool init xfs_rbd_pool
    rbd create xfs_rbd_pool/foo1 --size 50T
    rbd map xfs_rbd_pool/foo1 --name client.admin -m mncx86001,mncx86002,mncx86003 -k /etc/ceph/ceph.client.admin.keyring 
    mkdir /mnt/rbd_benchmark_xfs
    mount /dev/rbd/xfs_rbd_pool/foo1 /mnt/rbd_benchmark_xfs

    ceph osd pool rm xfs_rbd_pool xfs_rbd_pool --yes-i-really-really-mean-it



    rbd create --size 6000 image1 -p rbd_pool
    rbd create --size 6000 test_image -p test_pool

    ```

- ```
1.uuid gen
uuidgen
=>23914b70-5683-45ff-8003-ad79759a1750(deploy node)

2.Create an osd entry to Ceph cluster
ceph osd create 23914b70-5683-45ff-8003-ad79759a1750(deploy node)
=>138

2.push admin keyring
ceph-deploy admin arms006-10(Delploy node)

3.zap disk
ceph-disk zap /dev/sda
=>
/usr/local/lib/python2.7/dist-packages/ceph_disk-1.0.0-py2.7.egg/ceph_disk/main.py:5653: UserWarning: 
*******************************************************************************
This tool is now deprecated in favor of ceph-volume.
It is recommended to use ceph-volume for OSD deployments. For details see:

    http://docs.ceph.com/docs/master/ceph-volume/#migrating

*******************************************************************************

Creating new GPT entries.
GPT data structures destroyed! You may now partition the disk using fdisk or
other utilities.
Creating new GPT entries.
The operation has completed successfully.
/usr/local/lib/python2.7/dist-packages/ceph_disk-1.0.0-py2.7.egg/ceph_disk/main.py:5685: UserWarning: 
*******************************************************************************
This tool is now deprecated in favor of ceph-volume.
It is recommended to use ceph-volume for OSD deployments. For details see:

    http://docs.ceph.com/docs/master/ceph-volume/#migrating

*******************************************************************************
<=

4.create sda1&sda2
/sbin/sgdisk --new=2:0:+5120M --partition-guid=2:a7eb3f90-bf83-4ab8-b913-bc461a2044c3 --typecode=2:45b0969e-9b03-4f30-b4c6-b4b80ceff106 --mbrtogpt -- /dev/sda
/sbin/sgdisk  -c 2:"ceph journal" /dev/sda
/sbin/sgdisk --typecode=2:45b0969e-9b03-4f30-b4c6-b4b80ceff106 -- /dev/sda

/sbin/sgdisk --largest-new=1 --partition-guid=1:f93553ca-b1d8-4b39-a0c5-5a1cea66c1d4 --typecode=1:89c57f98-2fe5-4dc0-89c1-f3ad0ceff2be --mbrtogpt -- /dev/sda
/sbin/sgdisk -c 1:"ceph data" /dev/sda
/sbin/sgdisk --typecode=1:4fbd7e29-9d25-41b8-afd0-062c0ceff05d -- /dev/sda

/usr/bin/flock -s /dev/sda /sbin/partprobe /dev/sda
dmesg
=>
[ 4235.657702]  sda:
[ 4241.703065]  sda:
[ 4264.668064]  sda: sda2
[ 4265.719758]  sda: sda2
[ 4266.778243]  sda: sda2
[ 4267.835808]  sda: sda1 sda2
[ 4268.880422]  sda: sda1 sda2
[ 4269.930828]  sda: sda1 sda2
<=
ls -l /dev/disk/by-partuuid/
=>
lrwxrwxrwx 1 root root 10 Mar  7 15:37 a7eb3f90-bf83-4ab8-b913-bc461a2044c3 -> ../../sda2
lrwxrwxrwx 1 root root 10 Mar  7 15:37 f93553ca-b1d8-4b39-a0c5-5a1cea66c1d4 -> ../../sda1
<=

5.mkfs
mkdir /var/lib/ceph/osd/ceph-138
mkfs.xfs -f -i size=2048 /dev/sda1
=>
meta-data=/dev/sda1              isize=2048   agcount=32, agsize=76259320 blks
         =                       sectsz=4096  attr=2, projid32bit=0
data     =                       bsize=4096   blocks=2440298235, imaxpct=5
         =                       sunit=0      swidth=0 blks
naming   =version 2              bsize=4096   ascii-ci=0
log      =internal log           bsize=4096   blocks=521728, version=2
         =                       sectsz=4096  sunit=1 blks, lazy-count=1
realtime =none                   extsz=4096   blocks=0, rtextents=0
<=

6.mount
mount -o rw,noatime,inode64 /dev/sda1 /var/lib/ceph/osd/ceph-138
dmesg
=>[ 4365.361426] XFS (sda1): Mounting V4 Filesystem
=>[ 4365.428959] XFS (sda1): Ending clean mount

7.mkkey&mkjournal
chown -R ceph:ceph /var/lib/ceph/osd/

cd /var/lib/ceph/osd/ceph-138
sudo -u ceph ln -s /dev/disk/by-partuuid/a7eb3f90-bf83-4ab8-b913-bc461a2044c3 /var/lib/ceph/osd/ceph-138/journal
sudo -u ceph bash -c "echo 'a7eb3f90-bf83-4ab8-b913-bc461a2044c3' > /var/lib/ceph/osd/ceph-138/journal_uuid"

sudo -u ceph ceph-osd -i 138 --mkfs --mkkey --mkjournal

sudo -u ceph ceph-osd -i 138 --mkfs --mkkey --mkjournal --osd-uuid 868811f8-7067-47f3-b82e-083185155ca5
=>
2018-03-07 15:41:07.032148 b6750000 -1 asok(0x850a42a0) AdminSocketConfigObs::init: failed: AdminSocket::bind_and_listen: failed to bind the UNIX domain socket to '/var/run/ceph/ceph-osd.138.asok': (13) Permission denied
2018-03-07 15:41:07.032203 b6750000 -1 auth: error reading file: /var/lib/ceph/osd/ceph-138/keyring: can't open /var/lib/ceph/osd/ceph-138/keyring: (2) No such file or directory
2018-03-07 15:41:07.033147 b6750000 -1 created new key in keyring /var/lib/ceph/osd/ceph-138/keyring
2018-03-07 15:41:07.340110 b6750000 -1 journal check: ondisk fsid 00000000-0000-0000-0000-000000000000 doesn't match expected 868811f8-7067-47f3-b82e-083185155ca5, invalid (someone else's?) journal
2018-03-07 15:41:07.558682 b6750000 -1 journal do_read_entry(4096): bad header magic
2018-03-07 15:41:07.558713 b6750000 -1 journal do_read_entry(4096): bad header magic
2018-03-07 15:41:07.559646 b6750000 -1 read_settings error reading settings: (2) No such file or directory
2018-03-07 15:41:07.739664 b6750000 -1 key 
2018-03-07 15:41:07.877829 b6750000 -1 created object store /var/lib/ceph/osd/ceph-138 for osd.138 fsid 1832ac04-c1cc-412a-bd73-1df94a2523ca

<=
ls -l
=>
-rw-r--r-- 1 ceph ceph 37 Mar  7 15:41 ceph_fsid
drwxr-xr-x 4 ceph ceph 61 Mar  7 15:41 current
-rw-r--r-- 1 ceph ceph 37 Mar  7 15:41 fsid
lrwxrwxrwx 1 ceph ceph 58 Mar  7 15:40 journal -> /dev/disk/by-partuuid/a7eb3f90-bf83-4ab8-b913-bc461a2044c3
-rw------- 1 ceph ceph 58 Mar  7 15:41 keyring
-rw-r--r-- 1 ceph ceph 21 Mar  7 15:41 magic
-rw-r--r-- 1 ceph ceph  6 Mar  7 15:41 ready
-rw-r--r-- 1 ceph ceph  4 Mar  7 15:41 store_version
-rw-r--r-- 1 ceph ceph 53 Mar  7 15:41 superblock
-rw-r--r-- 1 ceph ceph 10 Mar  7 15:41 type
-rw-r--r-- 1 ceph ceph  4 Mar  7 15:41 whoami
<=


8.auth add

ceph auth add osd.138 osd 'allow *' mon 'allow profile osd' -i /var/lib/ceph/osd/ceph-138/keyring


ceph auth add osd.138 osd 'allow *' mon 'allow rwx' -i /var/lib/ceph/osd/ceph-138/keyring
=>added key for osd.138

9.crush add
ceph osd crush add 138 9 root=default host=arms006-10
=>add item id 138 name 'osd.138' weight 9 at location {host=arms006-10,root=default} to crush map

10.start osd daemon
start ceph-osd id=138
=>
[ 4887.142043] XFS (sda1): Metadata corruption detected at xfs_dir3_data_read_verify+0x58/0xd0, xfs_dir3_data block 0x48b9ff80
[ 4887.153223] XFS (sda1): Unmount and run xfs_repair
[ 4887.158040] XFS (sda1): First 64 bytes of corrupted metadata buffer:
[ 4887.164415] edbb0000: 58 46 53 42 00 00 10 00 00 00 00 00 91 73 fe fb  XFSB.........s..
[ 4887.172466] edbb0010: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
[ 4887.180505] edbb0020: 60 4f e4 41 e0 e2 43 ab 8c 01 8b ee df 0b 74 13  `O.A..C.......t.
[ 4887.188542] edbb0030: 00 00 00 00 80 00 00 07 ff ff ff ff ff ff ff ff  ................
[ 4887.196606] XFS (sda1): metadata I/O error: block 0x48b9ff80 ("xfs_trans_read_buf_map") error 117 numblks 8
=>

```

- ```
ceph-disk zap /dev/sda





/sbin/sgdisk --new=2:0:+5120M --partition-guid=2:a7eb3f90-bf83-4ab8-b913-bc461a2044c3 --typecode=2:45b0969e-9b03-4f30-b4c6-b4b80ceff106 --mbrtogpt -- /dev/sda
/sbin/sgdisk  -c 2:"ceph journal" /dev/sda

/sbin/sgdisk --typecode=2:45b0969e-9b03-4f30-b4c6-b4b80ceff106 -- /dev/sda

/sbin/sgdisk --largest-new=1 --partition-guid=1:f93553ca-b1d8-4b39-a0c5-5a1cea66c1d4 --typecode=1:89c57f98-2fe5-4dc0-89c1-f3ad0ceff2be --mbrtogpt -- /dev/sda
/sbin/sgdisk -c 1:"ceph data" /dev/sda

/sbin/mkfs -t xfs -f -i size=2048 -- /dev/sda1

/bin/mount -t xfs -o noatime,inode64 -- /dev/sda1 /var/lib/ceph/tmp/mnt.62fw0Q

adjust_symlink: Creating symlink /var/lib/ceph/tmp/mnt.62fw0Q/journal -> /dev/disk/by-partuuid/a7eb3f90-bf83-4ab8-b913-bc461a2044c3

ln -s /dev/disk/by-partuuid/a7eb3f90-bf83-4ab8-b913-bc461a2044c3 journal 
/bin/umount -- /var/lib/ceph/tmp/mnt.62fw0Q

/sbin/sgdisk --typecode=1:4fbd7e29-9d25-41b8-afd0-062c0ceff05d -- /dev/sda

ceph-deploy disk zap arms006-08:sda
ceph-deploy osd prepare --filestore arms006-08:sda
ceph-deploy osd activate arms006-08:sda1

ceph-deploy disk zap arms006-08:sdb
ceph-deploy osd prepare --filestore arms006-08:sdb
ceph-deploy osd activate arms006-08:sdb1

ceph-deploy disk zap arms006-10:sda
ceph-deploy osd prepare --filestore arms006-10:sda
ceph-deploy osd activate arms006-10:sda1

ceph-deploy disk zap arms006-10:sdb
ceph-deploy osd prepare --filestore arms006-10:sdb
ceph-deploy osd activate arms006-10:sdb1



ceph-deploy osd create --filestore arms006:sda --zap-disk

/sbin/mkfs -t xfs -m crc=1 -f -i size=2048 -l internal,size=2000b -f -- /dev/sda1
/bin/mount -t xfs -o noatime,inode64 -- /dev/sda1 /mnt
fio -filename=/mnt -direct=1 -iodepth 1 -thread -rw=write -ioengine=libaio -bs=4m -size=10G -numjobs=10 -runtime=100 -group_reporting -name=writetest
fio -filename=/dev/sda1 -direct=1 -iodepth 1 -thread -rw=write -ioengine=libaio -bs=4k -size=10G -numjobs=10 -runtime=100 -group_reporting -name=writetest
fio -filename=/dev/sda1 -direct=1 -iodepth 1 -thread -rw=randrw -rwmixread=70 -ioengine=psync -bs=4m -size=5G -numjobs=10 -runtime=60 -group_reporting -name=mytest -ioscheduler=noop
fio -filename=/mnt -direct=1 -iodepth 1 -thread -rw=randrw -ioengine=psync -bs=16k -size 2G -numjobs=10 -runtime=30 -group_reporting -name=mytest13 

 time dd if=/dev/zero of=/mnt/testw.dbf bs=4k count=100000

 https://mirrors.aliyun.com/ceph/debian-luminous/pool/main/c/ceph-deploy/ceph-deploy_1.5.39_all.deb 


 # ceph-osd -i 138 --mkfs --mkkey
2018-03-07 13:36:37.355494 b66e5000 -1 auth: error reading file: /var/lib/ceph/osd/ceph-138/keyring: can't open /var/lib/ceph/osd/ceph-138/keyring: (2) No such file or directory
2018-03-07 13:36:37.356720 b66e5000 -1 created new key in keyring /var/lib/ceph/osd/ceph-138/keyring
2018-03-07 13:36:37.617760 b66e5000 -1 journal FileJournal::_open: disabling aio for non-block journal.  Use journal_force_aio to force use of aio anyway
2018-03-07 13:36:37.852886 b66e5000 -1 journal FileJournal::_open: disabling aio for non-block journal.  Use journal_force_aio to force use of aio anyway
2018-03-07 13:36:37.853240 b66e5000 -1 journal do_read_entry(4096): bad header magic
2018-03-07 13:36:37.853276 b66e5000 -1 journal do_read_entry(4096): bad header magic
2018-03-07 13:36:37.853951 b66e5000 -1 read_settings error reading settings: (2) No such file or directory
2018-03-07 13:36:38.051093 b66e5000 -1 key 
2018-03-07 13:36:38.207574 b66e5000 -1 created object store /var/lib/ceph/osd/ceph-138 for osd.138 fsid 1832ac04-c1cc-412a-bd73-1df94a2523ca

```

export ARMGCC_DIR=/home/mac/projects/gcc-arm-none-eabi-7-2017-q4-major/
PATH="/Applications/CMake.app/Contents/bin":"$PATH"
PATH="/Users/zhaohedong/Desktop/projects/gcc-arm-none-eabi-7-2017-q4-major/bin":"$PATH"
