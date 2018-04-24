# $1=osd number
#!/bin/sh
ceph osd down osd.$1
ceph osd out osd.$1
ceph osd crush remove osd.$1
ceph osd rm $1
ceph auth del osd.$1
