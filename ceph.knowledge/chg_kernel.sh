#!/bin/bash

set -o errexit

HOSTS_NAME=$1
USER="mnvadmin"

ansible $HOSTS_NAME -s -m copy -a "src=zImage.4.4.120_A1 dest=~/ owner=$USER group=$USER backup=no" -u $USER --sudo
ansible $1 -m shell -a "umount /mnt||/bin/true" -umnvadmin --sudo
ansible $1 -m shell -a "mount /dev/mmcblk0p1 /mnt" -umnvadmin --sudo
ansible $1 -m shell -a "cp /mnt/zImage /mnt/zImage.last " -umnvadmin --sudo
ansible $1 -m shell -a "cp ~/zImage.4.4.120_A1 /mnt/zImage" -umnvadmin --sudo
ansible $1 -m shell -a "ls -lh /mnt" -umnvadmin --sudo
ansible $1 -m shell -a "umount /mnt" -umnvadmin --sudo

