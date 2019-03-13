CC=gcc
#INCLUDEDIR = /home/mac/Desktop/projects/linux-4.4.120/include/
ROOT=/usr/src/linux-headers-4.8.0-52-generic/
INCLUDEDIR = $(ROOT)/include/
MODCFLAGS :=-Wall -O -DMODULE -D__KERNEL__ -DLINUX -I$(INCLUDEDIR) -I$(ROOT)/arch/x86/include
map_driver.o: map_driver.c 
	$(CC) $(MODCFLAGS) -c map_driver.c
