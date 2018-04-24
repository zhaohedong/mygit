1.Armv7 kernel build
    $ export CROSS_COMPILE=<path/to/cross/compiler/>
      (Path to gcc compiler, without the gcc suffix. e.g. CROSS_COMPILE=/my_compilers/gcc-5.2/bin/aarch64-marvell-linux-gnu-)
    $ export ARCH=arm
    $ make mvebu_v7_lsp_defconfig
    $ make - for build kernel image and device tree images.
    $ make zImage - for build kernel image
2.Download ubuntu-base core image
3./etc/init/ttyS0.conf
4.ref to https://gnu-linux.org/building-ubuntu-rootfs-for-arm.html
4.1 point is, apt-get install qemu-user-static, and copy qemu-arm-static to arm rootfs, and then chroot will work well.


