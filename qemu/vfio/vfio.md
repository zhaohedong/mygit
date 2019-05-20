- option rom
```
qemu:
vfio_rom_read
	=> vfio_pci_load_rom
		=> ioctl(vdev->vbasedev.fd, VFIO_DEVICE_GET_REGION_INFO, &reg_info)
			=> pread(vdev->vbasedev.fd, vdev->rom + off, size, vdev->rom_offset + off);
kernel:
	info.offset = VFIO_PCI_INDEX_TO_OFFSET(info.index);
	info.size = pci_resource_len(pdev, info.index);

clone
	=>start_thread
		=> qemu_kvm_cpu_thread_fn
			=> kvm_cpu_exec
				=> address_space_rw
					=> memory_region_dispatch_read
						=> memory_region_dispatch_read1
							=> access_with_adjusted_size
								=> memory_region_read_accessor
									=> vfio_rom_read
										=> pci_load_rom
main
	=> qemu_opts_foreach
		=> qdev_device_add
			=> object_property_set_bool
				=> object_property_set_qobject
					=> object_property_set
						=> property_set_bool
							=> device_set_realized
								=> pci_qdev_realize
									=> vfio_initfn
										=> vfio_get_device
											=> ioctl(group->fd, VFIO_GROUP_GET_DEVICE_FD, name);
											=> ioctl(fd, VFIO_DEVICE_GET_INFO, &dev_info)
vfio_group_fops_unl_ioctl
	=> VFIO_GROUP_GET_DEVICE_FD
		=> vfio_group_get_device_fd
			=> vfio_device_get_from_name
vfio_pci_ioctl
	=> VFIO_DEVICE_GET_INFO

关于optionrom issue调查状况：
代码角度跟踪pci rom的读取：
qemu:
vfio_rom_read
    => vfio_pci_load_rom
        => ioctl(vdev->vbasedev.fd, VFIO_DEVICE_GET_REGION_INFO, &reg_info)
            => pread(vdev->vbasedev.fd, vdev->rom + off, size, vdev->rom_offset + off);
kernel:
    info.offset = VFIO_PCI_INDEX_TO_OFFSET(info.index);
    info.size = pci_resource_len(pdev, info.index);

关于bug的再现：
mail中提及两个设备（730-8i raid和mellonax），东软没有这两块设备，因此无法再现

关于自动dump pci optionrom:
用户空间可以使用PCI sysfs interface dump optionrom
参考http://smackerelofopinion.blogspot.com/2011/07/using-pci-sysfs-interface-to-dump-video.html
即：写一个用户空间程序，dump所有的pci device optionrom，再该程序启动之后再启动qemu，可能性是存在的，这样qemu启动时候就获得了所有pci device的option rom

接下来的方向：
寻找一块存在option rom的pci设备，来跟踪调试问题

当前的网卡设备在
Emulex 在linux host下有Oprom，在linux guest下有rom，在windows guest下无rom
Boardcom 在linux host下有Oprom，在linux guest下有rom，在windows guest下无rom
I350 在linux host下有Oprom，在linux guest下有rom，在windows guest下无rom
Raid 730-8i 在linux host下有Oprom，在linux guest下有rom，在windows guest下无rom

在qemu使用seabios场景下，guest下有rom
所有设备在windows host下均有rom






```
