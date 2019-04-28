CC = g++
CFLAGS = --std=c++11 -lstdc++
BUILD_DIR = /home/mac/projects/rootfs.16.04.armhf/ceph/build
INC = -I/home/mac/projects/rootfs.16.04.armhf/ceph/src -I/home/mac/projects/rootfs.16.04.armhf/ceph/build/include -I/home/mac/projects/rootfs.16.04.armhf/ceph/src/dmclock/support/src/ -I/home/mac/projects/rootfs.16.04.armhf/ceph/src/fmt/test/
INC2 = -I/home/mac/projects/rootfs.16.04.armhf/ceph/src/googletest/googlemock/include -I/home/mac/projects/rootfs.16.04.armhf/ceph/build/src/googletest/googlemock/include -I/home/mac/projects/rootfs.16.04.armhf/ceph/src/googletest/googletest/include -I/home/mac/projects/rootfs.16.04.armhf/ceph/build/src/googletest/googletest/include
LIBDIR = -L/home/mac/projects/rootfs.16.04.armhf/ceph/build/lib -L/usr/local/lib -L/usr/lib -L/lib -L/home/mac/projects/rootfs.16.04.armhf/ceph/src
LIB_CEPH_OSD = $(BUILD_DIR)/lib/libosd.a $(BUILD_DIR)/lib/libos.a $(BUILD_DIR)/lib/libglobal-static.a $(BUILD_DIR)/lib/libcommon.a -lblkid -libverbs -lfuse $(BUILD_DIR)/lib/libdmclock.a -laio -lfuse $(BUILD_DIR)/lib/libkv.a -lleveldb -ltcmalloc /home/mac/projects/rootfs.16.04.armhf/ceph/src/rocksdb/librocksdb.a -lsnappy -lz $(BUILD_DIR)/lib/libjson_spirit.a $(BUILD_DIR)/lib/libcommon_utf8.a $(BUILD_DIR)/lib/liberasure_code.a -lrt -lresolv -lblkid /usr/local/lib/libboost_thread.so /usr/local/lib/libboost_chrono.so /usr/local/lib/libboost_atomic.so /usr/local/lib/libboost_random.so /usr/local/lib/libboost_system.so /usr/local/lib/libboost_program_options.so /usr/local/lib/libboost_date_time.so /usr/local/lib/libboost_iostreams.so /usr/local/lib/libboost_regex.so -lssl3 -lsmime3 -lnss3 -lnssutil3 -lplds4 -lplc4 -lnspr4 -libverbs -ldl -lpthread 
LIB_CONFIG_TEST = $(BUILD_DIR)/lib/libgmock_main.a $(BUILD_DIR)/lib/libgmock.a $(BUILD_DIR)/lib/libgtest.a -lpthread -ldl $(BUILD_DIR)/lib/libglobal.a $(BUILD_DIR)/lib/libceph-common.so.0 $(BUILD_DIR)/lib/libjson_spirit.a $(BUILD_DIR)/lib/libcommon_utf8.a $(BUILD_DIR)/lib/liberasure_code.a /usr/local/lib/libboost_thread.so /usr/local/lib/libboost_chrono.so /usr/local/lib/libboost_atomic.so /usr/local/lib/libboost_random.so /usr/local/lib/libboost_system.so /usr/local/lib/libboost_program_options.so /usr/local/lib/libboost_date_time.so /usr/local/lib/libboost_iostreams.so /usr/local/lib/libboost_regex.so -lblkid -lssl3 -lsmime3 -lnss3 -lnssutil3 -lplds4 -lplc4 -lnspr4 -lpthread -lpthread -lrt -ldl -lresolv -libverbs 

objects = test.o #str_list.o Confutils.o 
test : $(objects)
	$(CC) -o test $(objects) $(CFLAGS) $(INC2) $(INC) $(LIBDIR) $(LIB_CEPH_OSD) $(LIB_CONFIG_TEST)
#Confutils.o : Confutils.cc 
#	$(CC) -c Confutils.cc $(INC) $(CFLAGS)
#str_list.o : str_list.cc 
#	$(CC) -c str_list.cc $(INC) $(CFLAGS)
test.o : test.cc
	$(CC) -c test.cc $(INC2) $(INC) $(CFLAGS)
PHONY : clean
clean :
	rm test $(objects)
