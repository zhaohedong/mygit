- pci initialization
```
=====================================================================================
[    0.000000] e820: [mem 0xc0000000-0xfeffbfff] available for PCI devices

setup_arch
    => e820_setup_gap



=====================================================================================
pci resource first request

[    0.140020] zhd-debug: __request_resource() start
[    0.140984] zhd-debug: __request_resource() new->start = 0x0, new->end = 0xcf7, root->start = 0x0, root->end = 0xffff
[    0.142005] zhd-debug: __request_resource() end unknown tmp->start = 0x0, tmp->end = 0x1f
[    0.143003] zhd-debug: __request_resource() start
[    0.143954] zhd-debug: __request_resource() new->start = 0xd00, new->end = 0xffff, root->start = 0x0, root->end = 0xffff
[    0.145003] zhd-debug: __request_resource() end found
[    0.146002] zhd-debug: __request_resource() start
[    0.147002] zhd-debug: __request_resource() new->start = 0xa0000, new->end = 0xbffff, root->start = 0x0, root->end = 0xffffffff
[    0.149003] zhd-debug: __request_resource() end found
[    0.150002] zhd-debug: __request_resource() start
[    0.151002] zhd-debug: __request_resource() new->start = 0xc0000000, new->end = 0xfebfffff, root->start = 0x0, root->end = 0xffffffff

#0  __request_resource (root=root@entry=0xffffffff81acc3c0 <iomem_resource>, new=new@entry=0xffff8802b1c54080)
    at kernel/resource.c:217
#1  0xffffffff810ef876 in __insert_resource (parent=parent@entry=0xffffffff81acc3c0 <iomem_resource>, 
    new=new@entry=0xffff8802b1c54080) at kernel/resource.c:806
#2  0xffffffff810f0ec9 in insert_resource_conflict (parent=parent@entry=0xffffffff81acc3c0 <iomem_resource>, 
    new=new@entry=0xffff8802b1c54080) at kernel/resource.c:871
#3  0xffffffff815c82e5 in add_resources (info=<optimized out>, crs_res=0xffff8802b215bbb0, resources=0xffff8802b215bbc0)
    at arch/x86/pci/acpi.c:312
#4  pci_acpi_scan_root (root=root@entry=0xffff8802b23d4d80) at arch/x86/pci/acpi.c:454
#5  0xffffffff8140df38 in acpi_pci_root_add (device=0xffff8802b23f4000, not_used=<optimized out>) at drivers/acpi/pci_root.c:589
#6  0xffffffff814099aa in acpi_scan_attach_handler (device=0xffff8802b23f4000) at drivers/acpi/scan.c:1820
#7  acpi_bus_attach (device=device@entry=0xffff8802b23f4000) at drivers/acpi/scan.c:1858
#8  0xffffffff81409a22 in acpi_bus_attach (device=device@entry=0xffff8802b23f3800) at drivers/acpi/scan.c:1872
#9  0xffffffff81409a22 in acpi_bus_attach (device=0xffff8802b23f3000) at drivers/acpi/scan.c:1872
#10 0xffffffff81409b18 in acpi_bus_scan (handle=handle@entry=0xffffffffffffffff) at drivers/acpi/scan.c:1901
#11 0xffffffff81c39991 in acpi_scan_init () at drivers/acpi/scan.c:2006
#12 0xffffffff81c397b8 in acpi_init () at drivers/acpi/bus.c:740
#13 0xffffffff810020e8 in do_one_initcall (fn=0xffffffff81c3953f <acpi_init>) at init/main.c:772
#14 0xffffffff81bee226 in do_initcall_level (level=<optimized out>) at init/main.c:838
#15 do_initcalls () at init/main.c:846
#16 do_basic_setup () at init/main.c:864
#17 kernel_init_freeable () at init/main.c:968
#18 0xffffffff816fa19e in kernel_init (unused=<optimized out>) at init/main.c:899
--Type <RET> for more, q to quit, c to continue without paging--
#19 <signal handler called>
#20 0x0000000000000000 in irq_stack_union ()
#21 0x0000000000000000 in ?? ()
==================================================================================


=================================================================================
root bus create call stack:

[    0.288019] pci_bus 0000:00: root bus resource [bus 00-ff]
[    0.322018] pci_bus 0000:00: root bus resource [io  0x0000-0x0cf7 window]
[    0.355021] pci_bus 0000:00: root bus resource [io  0x0d00-0xffff window]
[    0.390014] pci_bus 0000:00: root bus resource [mem 0x000a0000-0x000bffff window]
[    0.412025] pci_bus 0000:00: root bus resource [mem 0xc0000000-0xfebfffff window]

#0  pci_create_root_bus (parent=parent@entry=0x0 <irq_stack_union>, bus=bus@entry=0, ops=0xffffffff81b7c6a0 <pci_root_ops>, 
    sysdata=sysdata@entry=0xffff8802b23d4df8, resources=resources@entry=0xffff8802b215bbc0) at drivers/pci/probe.c:2223
#1  0xffffffff815c812e in pci_acpi_scan_root (root=root@entry=0xffff8802b23d4d80) at arch/x86/pci/acpi.c:466
#2  0xffffffff8140df38 in acpi_pci_root_add (device=0xffff8802b23f4000, not_used=<optimized out>) at drivers/acpi/pci_root.c:589
#3  0xffffffff814099aa in acpi_scan_attach_handler (device=0xffff8802b23f4000) at drivers/acpi/scan.c:1820
#4  acpi_bus_attach (device=device@entry=0xffff8802b23f4000) at drivers/acpi/scan.c:1858
#5  0xffffffff81409a22 in acpi_bus_attach (device=device@entry=0xffff8802b23f3800) at drivers/acpi/scan.c:1872
#6  0xffffffff81409a22 in acpi_bus_attach (device=0xffff8802b23f3000) at drivers/acpi/scan.c:1872
#7  0xffffffff81409b18 in acpi_bus_scan (handle=handle@entry=0xffffffffffffffff) at drivers/acpi/scan.c:1901
#8  0xffffffff81c39991 in acpi_scan_init () at drivers/acpi/scan.c:2006
#9  0xffffffff81c397b8 in acpi_init () at drivers/acpi/bus.c:740
#10 0xffffffff810020e8 in do_one_initcall (fn=0xffffffff81c3953f <acpi_init>) at init/main.c:772
#11 0xffffffff81bee226 in do_initcall_level (level=<optimized out>) at init/main.c:838
#12 do_initcalls () at init/main.c:846
#13 do_basic_setup () at init/main.c:864
#14 kernel_init_freeable () at init/main.c:968
#15 0xffffffff816fa19e in kernel_init (unused=<optimized out>) at init/main.c:899
#16 <signal handler called>
#17 0x0000000000000000 in irq_stack_union ()
#18 0x0000000000000000 in ?? ()
=====================================================================================



pci host bridge call stack:
acpi_init
    => acpi_scan_init 
        => acpi_bus_scan
            => acpi_bus_attach
                => acpi_scan_attach_handler
                    => acpi_pci_root_add
                        => pci_acpi_scan_root
                            => insert_resource_conflict
                                => __insert_resource
                                    => __request_resource

setup_arch
    => probe_rom
        => request_resource
            => request_resource_conflict
                => __request_resource
                    => add_resources
                        => insert_resource_conflict
                            => __insert_resource


pci_scan_bus
    => pci_add_resource(&resources, &ioport_resource)
    => pci_add_resource(&resources, &iomem_resource)
    => pci_add_resource(&resources, &busn_resource);
    => pci_create_root_bus
        => pci_scan_child_bus

subsys_initcall(acpi_init)
    => acpi_init
        => acpi_scan_init
            => acpi_pci_root_init
                => acpi_scan_add_handler_with_hotplug(&pci_root_handler, "pci_root")
                    => acpi_pci_root_add
                        => pci_acpi_scan_root
                            => pci_scan_child_bus
                                => pci_scan_slot
                                    => pci_scan_single_device
                                        => pci_scan_device
                                            => pci_setup_device
                                                => pci_read_bases(dev, 6, PCI_ROM_ADDRESS)  //0x30h
                                                    => __pci_read_base
                                                        =>
                                => pcibios_fixup_bus
                                    => pci_read_bridge_bases
                                        => pci_read_bridge_io
                                        => pci_read_bridge_mmio
                                        => pci_read_bridge_mmio_pref
arch_initcall(pci_arch_init);
    => pci_arch_init
        => pcibios_init
            => pcibios_resource_survey
                => pcibios_allocate_bus_resources    //DFS
                    => pcibios_allocate_bridge_resources
                    => pcibios_allocate_bus_resources   
                        => pci_claim_bridge_resource
                            => pci_claim_resource
                            => pci_bus_clip_resource
                            => pci_setup_bridge_io/pci_setup_bridge_mmio/pci_setup_bridge_mmio_pref
                => pcibios_allocate_resources(bus, 0);
            	=> pcibios_allocate_resources(bus, 1);

__pci_bus_assign_resources
    => pci_setup_bridge
        => __pci_setup_bridge

__pci_bridge_assign_resources
    => pci_setup_bridge
        => __pci_setup_bridge

pci_rescan_bus_bridge_resize
    => pci_assign_unassigned_bridge_resources
        => __pci_bridge_assign_resources
            => pdev_assign_resources_sorted
                => __assign_resources_sorted
                    => assign_requested_resources_sorted
                        => pci_assign_resource
                             => _pci_assign_resource
                                 => __pci_assign_resource
                                     => pci_bus_alloc_resource
                                         => pci_bus_alloc_from_region
                                             => allocate_resource
pci_request_selected_regions
    => __pci_request_selected_regions
        => __pci_request_region
            => __request_region
                => __request_resource


bridge window 申请(pci bridge申请空间，bridge下的设备再从此空间下申请bar空间)
pci_scan_child_bus
    => pci_scan_slot
        => pci_scan_single_device
            => pci_scan_device  //PCI设备的配置寄存器进行读写操作,硬件层面初始化, 创建了pci_dev结构体
                => pci_setup_device
                    => pci_read_bases(dev, 6, PCI_ROM_ADDRESS); //pci_dev resource 参数中的BAR空间的基地址属于存储器域，PCI设备BAR寄存器中存放的基地址属于PCI总线域
                        => __pci_read_base
            => pci_device_add   //软件层面初始化

    => pcibios_fixup_bus
        =>pci_read_bridge_bases
            => pci_read_bridge_io
            => pci_read_bridge_mmio
            => pci_read_bridge_mmio_pref



- 查看设备状态流程
    - 确定设备bdf -> 根据设备的bus找到pci bridge设备 -> 查看pci bridge 申请了多少bridge window， 设备的resource一定会落在pci bridge 的window中
- 查看设备树
    - lspci -tv
- 查看bus 对应的pci bridge设备的方法
    - dmesg | grep "PCI bridge"
- 查看pci bridge 申请了多少bridge window
    - dmesg | grep "bridge window \[mem"| grep "0000:00:1c.0" 
- 查看整体root bus resource状况
    - dmesg | grep -i "root bus resource"
- bridge window 申请函数
    - pci_read_bridge_mmio

- 以server的I350网卡为例
    - lspci查看网卡的bdf
    ```
    01:00.0 Ethernet controller: Intel Corporation I350 Gigabit Network Connection (rev 01)
    01:00.1 Ethernet controller: Intel Corporation I350 Gigabit Network Connection (rev 01)
    01:00.2 Ethernet controller: Intel Corporation I350 Gigabit Network Connection (rev 01)
    01:00.3 Ethernet controller: Intel Corporation I350 Gigabit Network Connection (rev 01)
    ```
    - 找到pci bridge 
        - [    0.646661] pci 0000:00:1c.0: PCI bridge to [bus 01]
    - 查看 0000:00:1c.0 申请了多少bridge window
    ```
        [    0.646703] pci 0000:00:1c.0:   bridge window [mem 0x92200000-0x926fffff]
        [    0.706801] pci 0000:00:1c.0: bridge window [mem 0x00100000-0x000fffff 64bit pref] to [bus 01] add_size 200000 add_align 100000
        [    0.708813] pci 0000:00:1c.0:   bridge window [mem 0x92200000-0x926fffff]
        [    0.708819] pci 0000:00:1c.0:   bridge window [mem 0x380000000000-0x3800001fffff 64bit pref]
    ```
    - 实际状况/proc/iomem
    ```
    92200000-926fffff : PCI Bus 0000:01
        92200000-922fffff : 0000:01:00.3
        92300000-923fffff : 0000:01:00.2
        92400000-924fffff : 0000:01:00.1
        92500000-925fffff : 0000:01:00.0
        92600000-92603fff : 0000:01:00.3
        92604000-92607fff : 0000:01:00.2
        92608000-9260bfff : 0000:01:00.1
        9260c000-9260ffff : 0000:01:00.0
        92680000-926fffff : 0000:01:00.0

    380000000000-3800001fffff : PCI Bus 0000:01
        380000000000-38000001ffff : 0000:01:00.0
        380000020000-38000003ffff : 0000:01:00.0
        380000040000-38000005ffff : 0000:01:00.1
        380000060000-38000007ffff : 0000:01:00.1
        380000080000-38000009ffff : 0000:01:00.2
        3800000a0000-3800000bffff : 0000:01:00.2
        3800000c0000-3800000dffff : 0000:01:00.3
        3800000e0000-3800000fffff : 0000:01:00.3    
    ```
- 以server raid卡为例
    - lspci查看网卡的bdf
        - 1f:00.0 RAID bus controller: LSI Logic / Symbios Logic MegaRAID SAS-3 3108 [Invader] (rev 02)
    - 找到pci bridge
        ```
        [    0.664551] pci 0000:16:02.0: PCI bridge to [bus 1d-1f]
        [    0.664839] pci 0000:1d:00.0: PCI bridge to [bus 1e-1f]
        ```
    - 查看0000:16:02.0申请了多少bridge window
        ```
        [    0.664592] pci 0000:16:02.0:   bridge window [mem 0xa6000000-0xa61fffff]
        [    0.664618] pci 0000:16:02.0:   bridge window [mem 0x387ffff00000-0x387fffffffff 64bit pref]
        [    0.710276] pci 0000:16:02.0:   bridge window [mem 0xa6000000-0xa61fffff]
        [    0.710281] pci 0000:16:02.0:   bridge window [mem 0x387ffff00000-0x387fffffffff 64bit pref]
        ```
    - 查看0000:1d:00.0申请了多少bridge window
        ```
        [    0.664856] pci 0000:1d:00.0:   bridge window [mem 0xa6000000-0xa61fffff]
        [    0.709031] pci 0000:1d:00.0: bridge window [mem 0x00100000-0x000fffff 64bit pref] to [bus 1e-1f] add_size 200000 add_align 100000
        [    0.710259] pci 0000:1d:00.0:   bridge window [mem 0xa6000000-0xa61fffff]
        ```
    - 实际状况/proc/iomem
    ```
    a6000000-bb7fffff : PCI Bus 0000:16
        a6000000-a61fffff : PCI Bus 0000:1d
            a6000000-a61fffff : PCI Bus 0000:1e
                a6000000-a61fffff : PCI Bus 0000:1f
                    a6000000-a60fffff : 0000:1f:00.0
                    a6100000-a610ffff : 0000:1f:00.0
    ```
    - 整体root bus resource 状况   
    ```
    [    0.653322] pci_bus 0000:16: root bus resource [bus 16-61]
    [    0.653324] pci_bus 0000:16: root bus resource [io  0x4000-0x4fff window]
    [    0.653326] pci_bus 0000:16: root bus resource [mem 0xa6000000-0xbb7fffff window]
    [    0.653327] pci_bus 0000:16: root bus resource [mem 0x384000000000-0x387fffffffff window]
    [    0.667467] pci_bus 0000:62: root bus resource [bus 62-ad]
    [    0.667468] pci_bus 0000:62: root bus resource [mem 0xbb800000-0xd0ffffff window]
    [    0.667470] pci_bus 0000:62: root bus resource [mem 0x388000000000-0x38bfffffffff window]
    [    0.674394] pci_bus 0000:ae: root bus resource [bus ae-f9]
    [    0.674396] pci_bus 0000:ae: root bus resource [mem 0xd1000000-0xe67fffff window]
    [    0.674397] pci_bus 0000:ae: root bus resource [mem 0x38c000000000-0x38ffffffffff window]
    [    0.678280] pci_bus 0000:fa: root bus resource [bus fa-ff]
    [    0.678282] pci_bus 0000:fa: root bus resource [io  0x5000-0xffff window]
    [    0.678283] pci_bus 0000:fa: root bus resource [mem 0xe6800000-0xfbffffff window]
    [    0.678285] pci_bus 0000:fa: root bus resource [mem 0x390000000000-0x393fffffffff window]
    ```
对于assigned/unassigned bar空间的理解

```
pcibios_assign_resources
    pcibios_allocate_rom_resources
        => pcibios_allocate_dev_rom_resource
            => pci_claim_resource
                => pci_find_parent_resource
                => request_resource_conflict
                    => __request_resource
    pci_assign_unassigned_resources
    pcibios_fw_addr_list_del
```


- analysis
  - ```
        a6000000-bb7fffff : PCI Bus 0000:16
          a6000000-a61fffff : PCI Bus 0000:1d
            a6000000-a61fffff : PCI Bus 0000:1e
              a6000000-a61fffff : PCI Bus 0000:1f
                a6000000-a60fffff : 0000:1f:00.0
                a6100000-a610ffff : 0000:1f:00.0
    ````

    ```
    [    0.634179] zhd-debug: pci_read_bases() calling __pci_read_base pci_bar_mem32 = 0x2, rom = 0x30
    [    0.634183] zhd-debug: __pci_read_base() dev->vendor = 0x1000, dev->device = 0x005d
    [    0.634187] pci 0000:1f:00.0: my KERN_DEBUG TEST1
    [    0.634189] my KERN_DEBUG TEST2
    [    0.634192] zhd-debug: __pci_read_bases() pos = 0x30 , type = 0x2, res->flags = 0x4e200, occured start print log
    [    0.634202] zhd-debug: __pci_read_bases() l = 0xfff00000, sz = 0xfff00000, pos = 0x30
    [    0.634208] zhd-debug: __pci_read_bases() type != pci_bar_unkonwn l64 = 0xfff00000, sz64 = 0xfff00000, res->flags = 0x4e200
    [    0.634213] zhd-debug: __pci_read_bases() normal process l64 = 0xfff00000, sz64 = 0xfffff, res->flags = 0x4e200
    [    0.634217] zhd-debug: pcibios_bus_to_resource() start
    [    0.634219] zhd-debug: pcibios_bus_to_resource() region->start = 0xfff00000, region->end = 0xffffffff
    [    0.634225] zhd-debug: pcibios_bus_to_resource() window->res->start = 0x16, window->res->end = 0x61, window->offset = 0x0
    [    0.634229] zhd-debug: pcibios_bus_to_resource() resource_type(res) != resource_type(window->res) occured
    [    0.634233] zhd-debug: pcibios_bus_to_resource() window->res->start = 0x4000, window->res->end = 0x4fff, window->offset = 0x0
    [    0.634238] zhd-debug: pcibios_bus_to_resource() resource_type(res) != resource_type(window->res) occured
    [    0.634244] zhd-debug: pcibios_bus_to_resource() window->res->start = 0xa6000000, window->res->end = 0xbb7fffff, window->offset = 0x0
    [    0.634248] zhd-debug: pcibios_bus_to_resource() bus_region.start = 0xa6000000, bus_region.end = 0xbb7fffff
    [    0.634252] zhd-debug: pcibios_bus_to_resource() window->res->start = 0x0, window->res->end = 0xffffffff, window->offset = 0x0
    [    0.634257] zhd-debug: pcibios_bus_to_resource() bus_region.start = 0x0, bus_region.end = 0xffffffff
    [    0.634261] zhd-debug: pcibios_bus_to_resource() res->start = 0xfff00000, res->end = 0xffffffff, res->name = 0000:1f:00.0
    [    0.634265] zhd-debug: pcibios_bus_to_resource() end
    [    0.634268] zhd-debug: pcibios_resource_to_bus() region->start = 0xfff00000, region->end = 0xffffffff, offset = 0x0
    [    0.634272] zhd-debug: pcibios_resource_to_bus() res->start = 0xfff00000, res->end = 0xffffffff, res->name = 0000:1f:00.0
    [    0.634276] pci 0000:1f:00.0: reg 0x30: [mem 0xfff00000-0xffffffff pref]
    ```

    ```
    [    0.689719] zhd-debug: pcibios_allocate_dev_rom_resource() dev->vendor = 0x1000, dev->device = 0x005d
    [    0.689723] zhd-debug: pci_claim_resource() start
    [    0.689725] zhd-debug: pci_claim_resource() dev->vendor = 0x1000, dev->device = 0x005d
    [    0.689729] zhd-debug: pci_find_parent_resource() dev->vendor = 0x1000, dev->device = 0x005d
    [    0.689733] zhd-debug: pci_find_parent_resource() r->start = 0x4000, r->end = 0x4fff, i = 0x0
    [    0.689738] zhd-debug: pci_find_parent_resource() r->start = 0xa6000000, r->end = 0xa61fffff, i = 0x1
    [    0.689742] zhd-debug: pci_find_parent_resource() r->start = 0x0, r->end = 0x0, i = 0x2
    [    0.689746] zhd-debug: pci_find_parent_resource() r->start = 0x0, r->end = 0x0, i = 0x3
    [    0.689749] zhd-debug: pci_find_parent_resource() end NO FOUND, i = x04
    [    0.689753] pci 0000:1f:00.0: can't claim BAR 6 [mem 0xfff00000-0xffffffff pref]: no compatible bridge window
    ```

    ```
    [    0.702541] zhd-debug: allocate_resource() start
    [    0.702543] zhd-debug: __find_resource() start
    [    0.702545] zhd-debug: __find_resource() constraint->min = 0xa6000000, constraint->max = 0xa61fffff, constraint->align = 0x100000
    [    0.702550] zhd-debug: __find_resource() avail.start = 0xa6200000, avail.end = 0xa61fffff, avail.flags = 0x4e200, tmp.start = 0xa6110000, tmp.end = 0xa61fffff
    [    0.702555] zhd-debug: pcibios_align_resource() start
    [    0.702558] zhd-debug: __find_resource() end EBUSY
    [    0.702560] zhd-debug: allocate_resource() end err=-16,
    [    0.702563] zhd-debug: pci_bus_alloc_from_region() return ENOMEM
    [    0.702565] zhd-debug: __pci_assign_resource() get address A3 align = 0x100000, size = 0x100000, resno = 0x6
    [    0.702569] zhd-debug: __pci_assign_resource() res->start = 0x0, res->end = 0xfffff, res->name = 0000:1f:00.0, ret = 0xfffffff4
    [    0.702575] zhd-debug: pci_assign_resource() dev->vendor = 0x1000, dev->device = 0x005d
    [    0.702580] zhd-debug: pci_assign_resource() align = 0x100000, size = 0x100000, ret = 0xfffffff4, resno = 0x6
    [    0.702585] pci 0000:1f:00.0: BAR 6: no space for [mem size 0x00100000 pref]
    [    0.702588] zhd-debug: pcibios_retrieve_fw_addr() dev->vendor = 0x1000, dev->device = 0x005d
    [    0.702592] zhd-debug: pcibios_retrieve_fw_addr() pcibios_fw_addr_done = 0x0, idx = 0x6
    [    0.702596] zhd-debug: pcibios_retrieve_fw_addr() fw_addr = 0x0
    [    0.702599] pci 0000:1f:00.0: BAR 6: failed to assign [mem size 0x00100000 pref]
    ```

    pcibios_allocate_rom_resources() dev->vendor = 0x1000, dev->device = 0x005d
