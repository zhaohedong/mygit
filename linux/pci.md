- pci initialization
```
1.pci 如何确定给bridge的资源
2.pci 设备树的创建流程
3.何时reassign
4.request_resources的作用
5. shell 下修改PCI配置空间
    - http://samfreetime.blogspot.com/2011/10/efi-shell-commands-mmmemmmioiopcipcie.html
    - mm 0x0001000030 -PCI -w 4 // 修改为FFF00000，ss=00, bb=01, dd=00, ff=00, rr=30
6. shell 下修改RAID卡所连接的Bridge 设备mmio空间
    - mm 0x0000010020 -PCI -w 4 //修改为df20df10
7. option rom本身是prefetch的，而最后可以使用普通的bridge window
    - 2433 [    0.688661] pci 0000:01:00.0: reg 0x30: [mem 0xfff00000-0xffffffff pref]
8. 父bridge 申请的时mmio，32位
    - 2457 [    0.690975] zhd-debug: pci_read_bridge_mmio() region.start = 0x92200000, region.end = 0x923fffff, res->flags = 0x200
    -  2477 [    0.691080] pci 0000:00:1c.0:   bridge window [mem 0x92200000-0x923fffff]
9. i350
 4240 [    0.716006] pci 0000:16:02.0:   bridge window [mem 0xa6000000-0xa64fffff]
 4348 [    0.718162] pci 0000:1f:00.0: reg 0x1c: [mem 0xa640c000-0xa640ffff
 4369 [    0.718868] pci 0000:1f:00.0: reg 0x30: [mem 0xfff80000-0xffffffff pref]
 14098 [    0.837113] pci 0000:1f:00.0: BAR 6: assigned [mem 0xa6480000-0xa64fffff pref]
  prefetch最后可以借用普通mmio空间
10. efi code 
    - PCI_EXPANSION_ROM_HEADER
    - PciHostBridgeResourceAllocator
    - ProgramBar

关于根本原因：
对于PCI设备option rom的内存空间分配，主要涉及pci设备自身ROM BAR的地址值及pci设备的父设备（pci桥接设备）的bridge window，如果bridge window有足够的空间来容纳rom，那么即使ROM BAR本身是一个非法值，linux内核也从pci桥接设备的bridge window下申请一段内存给ROM BAR使用。

本bug的关键点：
1.pci设备的ROM BAR是一个无效值（0xFFF00000）
2.父设备的bridge window为a6000000-a61fffff，这2MB的空间被该pci设备的BAR0、BAR1所占据，内核无法从这部分空间再申请1MB空间给ROM BAR使用。

经调查，内核再为设备分配内存资源前，还有一个扩大父设备bridge window的逻辑，但是这个扩大bridge window的逻辑，也没有为本bug中的raid成功申请到内存资源，原因如下：

__pci_bus_size_bridges函数内部有三处对pbus_size_mem 的调用，这三处调用分别对应了64bit prefetch bridge window，32bit bridge window，no-prefetch bridge window。如果桥接设备存在64bit prefetch window，那么内存资源的申请扩大处理将在64bit prefetch 完成，同时32位prefetch内存资源的申请和no-prefetch内存资源申请，交由no-prefetch bridge window来处理（*注意，这个逻辑就不会执行32bit prefetch bridge window）

本bug的在可能为桥接设备扩大bridge window的处理逻辑上，就如上面描述经过了64bit prefetch对pbus_size_mem的调用，pbus_size_mem内部处理由于资源flags匹配失败（因为rom申请的内存要求是32bit），并没有为rom扩大内存，

[    0.771107] zhd-debug: pci_assign_unassigned_root_bus_resources() start
[    0.771112] zhd-debug: __pci_bus_size_bridges() start
[    0.771327] zhd-debug: pbus_size_mem() dev->vendor = 0x1000, dev->device = 0x005d, r->start = 0x0, r->end = 0xfffff, i = 0x6
[    0.771331] zhd-debug: pbus_size_mem() continue rflags = 0x2004e200, mask = 0x102200, type = 0x102200, type2 = 0x102200, type3 = 0x102200
****由于上面的rflags对应的内存是32bit（设备rom内存要求），而type对应内存是64bit（bridge window的资源状况），两个内存地址不匹配，因此不会为rom而扩大内存






====================================================================================
2377 [    0.811718] pci 0000:01:00.0: reg 0x14: [mem 0x92300000-0x9230ffff 64bit]
2400 [    0.811855] pci 0000:01:00.0: reg 0x1c: [mem 0x92200000-0x922fffff 64bit]
2431 [    0.812037] pci 0000:01:00.0: reg 0x30: [mem 0xfff00000-0xffffffff pref]

2455 [    0.813727] zhd-debug: pci_read_bridge_mmio() region.start = 0x92200000, region.end = 0x923fffff, res->flags = 0x200


2406 [    0.811902] zhd-debug: __pci_read_bases() l = 0xfff00000, sz = 0xfff00000, pos = 0x30
2431 [    0.812037] pci 0000:01:00.0: reg 0x30: [mem 0xfff00000-0xffffffff pref]

2475 [    0.813832] pci 0000:00:1c.0:   bridge window [mem 0x92200000-0x923fffff]

2481 [    0.813861] zhd-debug: pci_read_bridge_mmio_perf() A2 base64= 0xfff00000, limit64= 0x0
2485 [    0.813883] zhd-debug: pcibios_fixup_device_resources() dev->vendor = 0x1000, dev->device = 0x005d
2486 [    0.813888] zhd-debug: pcibios_fixup_device_resources() rom_r->start = 0xfff00000, rom_r->end = 0xffffffff, rom_r->name = 0000:01:00.0

⭕️6570 [    0.905974] zhd-debug: pcibios_resource_survey() start



6587 [    0.906042] zhd-debug: pci_claim_resource() start
6588 [    0.906045] zhd-debug: pci_claim_resource() dev->vendor = 0x8086, dev->device = 0xa190
6602 [    0.906110] zhd-debug: __request_resource() new->start = 0x92200000, new->end = 0x923fffff, root->start = 0x90000000, root->end = 0xa5ffffff

7164 [    0.908492] zhd-debug: pci_claim_resource() start
7165 [    0.908495] zhd-debug: pci_claim_resource() dev->vendor = 0x1000, dev->device = 0x005d
7171 [    0.908525] zhd-debug: __request_resource() new->start = 0x92300000, new->end = 0x9230ffff, root->start = 0x92200000, root->end = 0x923fffffA

⭕️8547 [    0.915400] zhd-debug: pcibios_resource_survey() end

8687 [    0.933768] zhd-debug: pcibios_assign_resources() start

RAID FAIL 1:
8734 [    0.933994] zhd-debug: pcibios_allocate_rom_resources() start
8735 [    0.933997] zhd-debug: pcibios_allocate_rom_resources() dev->vendor = 0x1000, dev->device = 0x005d
8736 [    0.934003] zhd-debug: pcibios_allocate_dev_rom_resource() start
8737 [    0.934006] zhd-debug: pcibios_allocate_dev_rom_resource() dev->vendor = 0x1000, dev->device = 0x005d
8738 [    0.934011] zhd-debug: pci_claim_resource() start
8739 [    0.934014] zhd-debug: pci_claim_resource() dev->vendor = 0x1000, dev->device = 0x005d
8740 [    0.934019] zhd-debug: pci_find_parent_resource() dev->vendor = 0x1000, dev->device = 0x005d
8741 [    0.934025] zhd-debug: pci_find_parent_resource() r->start = 0x3000, r->end = 0x3fff, i = 0x0
8742 [    0.934031] zhd-debug: pci_find_parent_resource() r->start = 0x92200000, r->end = 0x923fffff, i = 0x1
8743 [    0.934036] zhd-debug: pci_find_parent_resource() r->start = 0x0, r->end = 0x0, i = 0x2
8744 [    0.934041] zhd-debug: pci_find_parent_resource() r->start = 0x0, r->end = 0x0, i = 0x3
8745 [    0.934047] zhd-debug: pci_find_parent_resource() end NO FOUND, i = x04
8746 [    0.934051] pci 0000:01:00.0: can't claim BAR 6 [mem 0xfff00000-0xffffffff pref]: no compatible bridge window

I350 FAIL1:
8828 [    0.934391] zhd-debug: pcibios_allocate_rom_resources() start
8829 [    0.934394] zhd-debug: pcibios_allocate_rom_resources() dev->vendor = 0x8086, dev->device = 0x1521
8830 [    0.934399] zhd-debug: pcibios_allocate_dev_rom_resource() start
8831 [    0.934402] zhd-debug: pcibios_allocate_dev_rom_resource() dev->vendor = 0x8086, dev->device = 0x1521
8832 [    0.934408] zhd-debug: pci_claim_resource() start
8833 [    0.934411] zhd-debug: pci_claim_resource() dev->vendor = 0x8086, dev->device = 0x1521
8834 [    0.934416] zhd-debug: pci_find_parent_resource() dev->vendor = 0x8086, dev->device = 0x1521
8835 [    0.934421] zhd-debug: pci_find_parent_resource() r->start = 0x0, r->end = 0x0, i = 0x0
8836 [    0.934427] zhd-debug: pci_find_parent_resource() r->start = 0xa6000000, r->end = 0xa64fffff, i = 0x1
8837 [    0.934432] zhd-debug: pci_find_parent_resource() r->start = 0x0, r->end = 0x0, i = 0x2
8838 [    0.934437] zhd-debug: pci_find_parent_resource() r->start = 0x0, r->end = 0x0, i = 0x3
8839 [    0.934442] zhd-debug: pci_find_parent_resource() end NO FOUND, i = x04
8840 [    0.934446] pci 0000:1f:00.0: can't claim BAR 6 [mem 0xfff80000-0xffffffff pref]: no compatible bridge window


⭕️9318 [    0.936620] zhd-debug: pci_assign_unassigned_resources() start
⭕️9319 [    0.936623] zhd-debug: pci_assign_unassigned_root_bus_resources() start
⭕️9320 [    0.936626] zhd-debug: pci_assign_unassigned_root_bus_resources() again
⭕️9329 [    0.936695] zhd-debug: pbus_assign_resources_sorted() start
⭕️__dev_sort_resources
⭕️9330 [    0.936701] zhd-debug: __assign_resources_sorted() start
⭕️9331 [    0.936704] zhd-debug: __assign_resources_sorted() requested_and_reassign
⭕️9332 [    0.936707] zhd-debug: assign_requested_resources_sorted() start
⭕️9333 [    0.936711] zhd-debug: assign_requested_resources_sorted() end
⭕️9334 [    0.936714] zhd-debug: reassign_resources_sorted() start
⭕️9335 [    0.936717] zhd-debug: reassign_resources_sorted() end
⭕️9336 [    0.936720] zhd-debug: __assign_resources_sorted() end
⭕️9337 [    0.936723] zhd-debug: pbus_assign_resources_sorted() end
⭕️9338 [    0.936727] zhd-debug: pbus_assign_resources_sorted() start
⭕️9339 [    0.936730] zhd-debug: __assign_resources_sorted() start
⭕️9340 [    0.936733] zhd-debug: __assign_resources_sorted() requested_and_reassign
⭕️9341 [    0.936736] zhd-debug: assign_requested_resources_sorted() start

RAID FAIL 2:
9342 [    0.936739] zhd-debug: pci_assign_resource() start dev->vendor = 0x1000, dev->device = 0x005d, resno = 6
9343 [    0.936745] zhd-debug: __pci_assign_resource() dev->vendor = 0x1000, dev->device = 0x005d
9344 [    0.936750] zhd-debug: __pci_assign_resource() align = 0x100000, size = 0x100000, resno = 0x6
9345 [    0.936756] zhd-debug: pci_bus_alloc_resource() start
9346 [    0.936759] zhd-debug: pci_bus_alloc_from_region() start
9347 [    0.936762] zhd-debug: pci_bus_alloc_from_region() return ENOMEM
9348 [    0.936765] zhd-debug: pci_bus_alloc_resource() start
9349 [    0.936768] zhd-debug: pci_bus_alloc_from_region() start
9350 [    0.936771] zhd-debug: pcibios_resource_to_bus() region_contains occured offset = 0x0
9351 [    0.936777] zhd-debug: pcibios_resource_to_bus() region->start = 0x92200000, region->end = 0x923fffff, offset = 0x0
9352 [    0.936782] zhd-debug: pcibios_resource_to_bus() res->start = 0x92200000, res->end = 0x923fffff, res->name = PCI Bus 0000:01
9353 [    0.936788] zhd-debug: pcibios_bus_to_resource() start
9354 [    0.936791] zhd-debug: pcibios_bus_to_resource() region->start = 0x92200000, region->end = 0x923fffff
9355 [    0.936797] zhd-debug: pcibios_bus_to_resource() window->res->start = 0x0, window->res->end = 0x15, window->offset = 0x0
9356 [    0.936802] zhd-debug: pcibios_bus_to_resource() resource_type(res) != resource_type(window->res) occured
9357 [    0.936808] zhd-debug: pcibios_bus_to_resource() window->res->start = 0x0, window->res->end = 0xcf7, window->offset = 0x0
9358 [    0.936813] zhd-debug: pcibios_bus_to_resource() resource_type(res) != resource_type(window->res) occured
9359 [    0.936819] zhd-debug: pcibios_bus_to_resource() window->res->start = 0x1000, window->res->end = 0x4fff, window->offset = 0x0
9360 [    0.936824] zhd-debug: pcibios_bus_to_resource() resource_type(res) != resource_type(window->res) occured
9361 [    0.936843] zhd-debug: pcibios_bus_to_resource() window->res->start = 0xa0000, window->res->end = 0xbffff, window->offset = 0x0
9362 [    0.936848] zhd-debug: pcibios_bus_to_resource() bus_region.start = 0xa0000, bus_region.end = 0xbffff
9363 [    0.936854] zhd-debug: pcibios_bus_to_resource() window->res->start = 0xc4000, window->res->end = 0xc7fff, window->offset = 0x0
9364 [    0.936859] zhd-debug: pcibios_bus_to_resource() bus_region.start = 0xc4000, bus_region.end = 0xc7fff
9365 [    0.936865] zhd-debug: pcibios_bus_to_resource() window->res->start = 0xfe010000, window->res->end = 0xfe010fff, window->offset = 0x0
9366 [    0.936870] zhd-debug: pcibios_bus_to_resource() bus_region.start = 0xfe010000, bus_region.end = 0xfe010fff
9367 [    0.936876] zhd-debug: pcibios_bus_to_resource() window->res->start = 0x90000000, window->res->end = 0xa5ffffff, window->offset = 0x0
9368 [    0.936882] zhd-debug: pcibios_bus_to_resource() bus_region.start = 0x90000000, bus_region.end = 0xa5ffffff
9369 [    0.936887] zhd-debug: pcibios_bus_to_resource() region_contains sucessfully offset = 0x0
9370 [    0.936892] zhd-debug: pcibios_bus_to_resource() res->start = 0x92200000, res->end = 0x923fffff, res->name = PCI Bus 0000:01
9371 [    0.936898] zhd-debug: pcibios_bus_to_resource() end
9372 [    0.936901] zhd-debug: allocate_resource() start
9373 [    0.936904] zhd-debug: __find_resource() start
9374 [    0.936907] zhd-debug: __find_resource() constraint->min = 0x92200000, constraint->max = 0x923fffff, constraint->align = 0x100000
9375 [    0.936913] zhd-debug: __find_resource() root->start = 0x92200000, root->end = 0x923fffff
9376 [    0.936918] zhd-debug: __find_resource() this->start = 0x92200000, this->end = 0x923fffff
9377 [    0.936924] zhd-debug: __find_resource() before resource_clip tmp.start = 0x92310000, tmp.end = 0x923fffff
9378 [    0.936929] zhd-debug: __find_resource() after resource_clip tmp.start = 0x92310000, tmp.end = 0x923fffff
9379 [    0.936935] zhd-debug: __find_resource() after arch_remove_reservations tmp.start = 0x92310000, tmp.end = 0x923fffff
9380 [    0.936941] zhd-debug: __find_resource() avail.start = 0x92400000, avail.end = 0x923fffff, avail.flags = 0x4e200, tmp.start = 0x92310000, tmp.end = 0      x923fffff
9381 [    0.936948] zhd-debug: pcibios_align_resource() start
9382 [    0.936952] zhd-debug: __find_resource() avail.start = 0x92400000, avail.end = 0x923fffff, alloc.start = 0x92400000, alloc.end = 0x924fffff
9383 [    0.936957] zhd-debug: __find_resource() end EBUSY
9384 [    0.936960] zhd-debug: allocate_resource() end err=-16,
9385 [    0.936963] zhd-debug: pci_bus_alloc_from_region() return ENOMEM
9386 [    0.936967] zhd-debug: __pci_assign_resource() get address A3 align = 0x100000, size = 0x100000, resno = 0x6
9387 [    0.936973] zhd-debug: __pci_assign_resource() res->start = 0x0, res->end = 0xfffff, res->name = 0000:01:00.0, ret = 0xfffffff4
9388 [    0.936978] zhd-debug: pci_assign_resource() dev->vendor = 0x1000, dev->device = 0x005d
9389 [    0.936984] zhd-debug: pci_assign_resource() align = 0x100000, size = 0x100000, ret = 0xfffffff4, resno = 0x6
9390 [    0.936990] pci 0000:01:00.0: BAR 6: no space for [mem size 0x00100000 pref]
9391 [    0.936993] zhd-debug: pcibios_retrieve_fw_addr() dev->vendor = 0x1000, dev->device = 0x005d
9392 [    0.936999] zhd-debug: pcibios_retrieve_fw_addr() pcibios_fw_addr_done = 0x0, idx = 0x6
9393 [    0.937004] zhd-debug: pcibios_retrieve_fw_addr() fw_addr = 0x0
9394 [    0.937008] pci 0000:01:00.0: BAR 6: failed to assign [mem size 0x00100000 pref]

9408 [    0.937073] zhd-debug: pci_setup_bridge_mmio() start
9412 [    0.937093] pci 0000:00:1c.0:   bridge window [mem 0x92200000-0x923fffff]

I350 SUCCESS 1:
9582 [    0.937922] zhd-debug: pbus_size_mem() start
9583 [    0.937925] zhd-debug: pbus_size_mem() EXPANSION_ROM_PATCH VAILD
9584 [    0.937929] zhd-debug: pbus_size_mem() dev->vendor = 0x8086, dev->device = 0x1521, r->start = 0x0, r->end = 0xffffffff, r_size = 0x20000

I350 parent bridge:
=> 9599 [    0.937999] pci 0000:1e:00.0: bridge window [mem 0x00100000-0x000fffff 64bit pref] to [bus 1f] add_size 200000 add_align 100000
=> 9604 [    0.938028] pci 0000:1e:00.0: res[15]=[mem 0x00100000-0x000fffff 64bit pref] res_to_dev_res add_size 200000 min_align 100000
=> 9605 [    0.938034] pci 0000:1e:00.0: res[15]=[mem 0x00100000-0x000fffff 64bit pref] res_to_dev_res add_size 200000 min_align 100000


9793 [    0.938952] zhd-debug: reassign_resources_sorted() add_res->dev->vendor = 0x1dab, add_res->dev->device = 0x8012, res->start = 0x100000, res->end = 0xfffff
9794 [    0.938957] zhd-debug: reassign_resources_sorted() continue A2
9795 [    0.938961] zhd-debug: reassign_resources_sorted() add_res->dev->vendor = 0x8086, add_res->dev->device = 0x1521, res->start = 0x0, res->end = 0xffffffff
9796 [    0.938966] zhd-debug: reassign_resources_sorted() continue A2


I350 parent bridge:
=> 11437 [    0.946610] pci 0000:1e:00.0: res[15]=[mem 0x00100000-0x000fffff 64bit pref] res_to_dev_res add_size 200000 min_align 100000
=> 11438 [    0.946616] pci 0000:1e:00.0: res[15]=[mem 0x00100000-0x002fffff 64bit pref] res_to_dev_res add_size 200000 min_align 100000

=> 11496 [    0.946882] pci 0000:1e:00.0: BAR 15: no space for [mem size 0x00200000 64bit pref]
=> 11500 [    0.946902] pci 0000:1e:00.0: BAR 15: failed to assign [mem size 0x00200000 64bit pref]
=> 11582 [    0.947267] pci 0000:1e:00.0: BAR 15: no space for [mem size 0x00200000 64bit pref]
=> 11586 [    0.947286] pci 0000:1e:00.0: BAR 15: failed to assign [mem size 0x00200000 64bit pref]

⭕️11622 [    0.947451] zhd-debug: reassign_resources_sorted() A3
⭕️11623 [    0.947454] zhd-debug: reassign_resources_sorted() end
⭕️11624 [    0.947457] zhd-debug: __assign_resources_sorted() end
⭕️11625 [    0.947460] zhd-debug: pbus_assign_resources_sorted() end
⭕️11626 [    0.947463] zhd-debug: pbus_assign_resources_sorted() start
⭕️11627 [    0.947468] zhd-debug: __assign_resources_sorted() start

11637 [    0.947525] zhd-debug: pci_assign_resource() start dev->vendor = 0x8086, dev->device = 0x1521, resno = 6
11638 [    0.947530] zhd-debug: __pci_assign_resource() dev->vendor = 0x8086, dev->device = 0x1521
11639 [    0.947536] zhd-debug: __pci_assign_resource() align = 0x80000, size = 0x80000, resno = 0x6
11640 [    0.947541] zhd-debug: pci_bus_alloc_resource() start
11641 [    0.947544] zhd-debug: pci_bus_alloc_from_region() start
11642 [    0.947547] zhd-debug: pci_bus_alloc_from_region() return ENOMEM
11643 [    0.947550] zhd-debug: pci_bus_alloc_resource() start
11644 [    0.947553] zhd-debug: pci_bus_alloc_from_region() start
11645 [    0.947556] zhd-debug: pcibios_resource_to_bus() region_contains occured offset = 0x0
11646 [    0.947561] zhd-debug: pcibios_resource_to_bus() region->start = 0xa6000000, region->end = 0xa64fffff, offset = 0x0
11647 [    0.947567] zhd-debug: pcibios_resource_to_bus() res->start = 0xa6000000, res->end = 0xa64fffff, res->name = PCI Bus 0000:1f
11648 [    0.947572] zhd-debug: pcibios_bus_to_resource() start
11649 [    0.947576] zhd-debug: pcibios_bus_to_resource() region->start = 0xa6000000, region->end = 0xa64fffff
11650 [    0.947581] zhd-debug: pcibios_bus_to_resource() window->res->start = 0x16, window->res->end = 0x61, window->offset = 0x0
11651 [    0.947586] zhd-debug: pcibios_bus_to_resource() resource_type(res) != resource_type(window->res) occured
11652 [    0.947592] zhd-debug: pcibios_bus_to_resource() window->res->start = 0xa6000000, window->res->end = 0xbb7fffff, window->offset = 0x0
11653 [    0.947598] zhd-debug: pcibios_bus_to_resource() bus_region.start = 0xa6000000, bus_region.end = 0xbb7fffff
11654 [    0.947603] zhd-debug: pcibios_bus_to_resource() region_contains sucessfully offset = 0x0
11655 [    0.947608] zhd-debug: pcibios_bus_to_resource() res->start = 0xa6000000, res->end = 0xa64fffff, res->name = PCI Bus 0000:1f
11656 [    0.947614] zhd-debug: pcibios_bus_to_resource() end
11657 [    0.947617] zhd-debug: allocate_resource() start
11658 [    0.947620] zhd-debug: __find_resource() start
11659 [    0.947623] zhd-debug: __find_resource() constraint->min = 0xa6000000, constraint->max = 0xa64fffff, constraint->align = 0x80000
11660 [    0.947628] zhd-debug: __find_resource() root->start = 0xa6000000, root->end = 0xa64fffff
11661 [    0.947634] zhd-debug: __find_resource() this->start = 0xa6000000, this->end = 0xa64fffff
11662 [    0.947639] zhd-debug: __find_resource() before resource_clip tmp.start = 0xa6410000, tmp.end = 0xa64fffff
11663 [    0.947644] zhd-debug: __find_resource() after resource_clip tmp.start = 0xa6410000, tmp.end = 0xa64fffff
11664 [    0.947650] zhd-debug: __find_resource() after arch_remove_reservations tmp.start = 0xa6410000, tmp.end = 0xa64fffff
11665 [    0.947656] zhd-debug: __find_resource() avail.start = 0xa6480000, avail.end = 0xa64fffff, avail.flags = 0x4e200, tmp.start = 0xa6410000, tmp.end = 0      xa64fffff
11666 [    0.947663] zhd-debug: pcibios_align_resource() start
11667 [    0.947667] zhd-debug: __find_resource() avail.start = 0xa6480000, avail.end = 0xa64fffff, alloc.start = 0xa6480000, alloc.end = 0xa64fffff
11668 [    0.947672] zhd-debug: __find_resource() new->start = 0xa6480000, new->end = 0xa64fffff
11669 [    0.947678] zhd-debug: __find_resource() end OK
11670 [    0.947680] zhd-debug: __request_resource() start
11671 [    0.947684] zhd-debug: __request_resource() new->start = 0xa6480000, new->end = 0xa64fffff, root->start = 0xa6000000, root->end = 0xa64fffff
11672 [    0.947689] zhd-debug: __request_resource() end found
11673 [    0.947692] zhd-debug: allocate_resource() end err=0,
11674 [    0.947695] zhd-debug: pci_bus_alloc_from_region() allocate_resource end OK
11628 [    0.947473] pci 0000:1f:00.0: res[7]=[mem 0x00000000-0xffffffffffffffff 64bit pref] res_to_dev_res add_size 20000 min_align 0
11629 [    0.947479] pci 0000:1f:00.0: res[10]=[mem 0x00000000-0xffffffffffffffff 64bit pref] res_to_dev_res add_size 20000 min_align 0
11630 [    0.947485] pci 0000:1f:00.1: res[7]=[mem 0x00000000-0xffffffffffffffff 64bit pref] res_to_dev_res add_size 20000 min_align 0
11631 [    0.947491] pci 0000:1f:00.1: res[10]=[mem 0x00000000-0xffffffffffffffff 64bit pref] res_to_dev_res add_size 20000 min_align 0
11632 [    0.947498] pci 0000:1f:00.2: res[7]=[mem 0x00000000-0xffffffffffffffff 64bit pref] res_to_dev_res add_size 20000 min_align 0
11633 [    0.947504] pci 0000:1f:00.2: res[10]=[mem 0x00000000-0xffffffffffffffff 64bit pref] res_to_dev_res add_size 20000 min_align 0
11634 [    0.947510] pci 0000:1f:00.3: res[7]=[mem 0x00000000-0xffffffffffffffff 64bit pref] res_to_dev_res add_size 20000 min_align 0
11635 [    0.947516] pci 0000:1f:00.3: res[10]=[mem 0x00000000-0xffffffffffffffff 64bit pref] res_to_dev_res add_size 20000 min_align 0
11636 [    0.947522] zhd-debug: assign_requested_resources_sorted() start
11637 [    0.947525] zhd-debug: pci_assign_resource() start dev->vendor = 0x8086, dev->device = 0x1521, resno = 6
11638 [    0.947530] zhd-debug: __pci_assign_resource() dev->vendor = 0x8086, dev->device = 0x1521
11639 [    0.947536] zhd-debug: __pci_assign_resource() align = 0x80000, size = 0x80000, resno = 0x6
11640 [    0.947541] zhd-debug: pci_bus_alloc_resource() start
11641 [    0.947544] zhd-debug: pci_bus_alloc_from_region() start
11642 [    0.947547] zhd-debug: pci_bus_alloc_from_region() return ENOMEM
11643 [    0.947550] zhd-debug: pci_bus_alloc_resource() start
11644 [    0.947553] zhd-debug: pci_bus_alloc_from_region() start
11645 [    0.947556] zhd-debug: pcibios_resource_to_bus() region_contains occured offset = 0x0
11646 [    0.947561] zhd-debug: pcibios_resource_to_bus() region->start = 0xa6000000, region->end = 0xa64fffff, offset = 0x0
11647 [    0.947567] zhd-debug: pcibios_resource_to_bus() res->start = 0xa6000000, res->end = 0xa64fffff, res->name = PCI Bus 0000:1f
11648 [    0.947572] zhd-debug: pcibios_bus_to_resource() start
11649 [    0.947576] zhd-debug: pcibios_bus_to_resource() region->start = 0xa6000000, region->end = 0xa64fffff
11650 [    0.947581] zhd-debug: pcibios_bus_to_resource() window->res->start = 0x16, window->res->end = 0x61, window->offset = 0x0
11651 [    0.947586] zhd-debug: pcibios_bus_to_resource() resource_type(res) != resource_type(window->res) occured
11652 [    0.947592] zhd-debug: pcibios_bus_to_resource() window->res->start = 0xa6000000, window->res->end = 0xbb7fffff, window->offset = 0x0
11653 [    0.947598] zhd-debug: pcibios_bus_to_resource() bus_region.start = 0xa6000000, bus_region.end = 0xbb7fffff
11654 [    0.947603] zhd-debug: pcibios_bus_to_resource() region_contains sucessfully offset = 0x0
11655 [    0.947608] zhd-debug: pcibios_bus_to_resource() res->start = 0xa6000000, res->end = 0xa64fffff, res->name = PCI Bus 0000:1f
11656 [    0.947614] zhd-debug: pcibios_bus_to_resource() end
11657 [    0.947617] zhd-debug: allocate_resource() start
11658 [    0.947620] zhd-debug: __find_resource() start
11659 [    0.947623] zhd-debug: __find_resource() constraint->min = 0xa6000000, constraint->max = 0xa64fffff, constraint->align = 0x80000
11660 [    0.947628] zhd-debug: __find_resource() root->start = 0xa6000000, root->end = 0xa64fffff
11661 [    0.947634] zhd-debug: __find_resource() this->start = 0xa6000000, this->end = 0xa64fffff
11662 [    0.947639] zhd-debug: __find_resource() before resource_clip tmp.start = 0xa6410000, tmp.end = 0xa64fffff
11663 [    0.947644] zhd-debug: __find_resource() after resource_clip tmp.start = 0xa6410000, tmp.end = 0xa64fffff
11664 [    0.947650] zhd-debug: __find_resource() after arch_remove_reservations tmp.start = 0xa6410000, tmp.end = 0xa64fffff
11665 [    0.947656] zhd-debug: __find_resource() avail.start = 0xa6480000, avail.end = 0xa64fffff, avail.flags = 0x4e200, tmp.start = 0xa6410000, tmp.end = 0      xa64fffff
11666 [    0.947663] zhd-debug: pcibios_align_resource() start
11667 [    0.947667] zhd-debug: __find_resource() avail.start = 0xa6480000, avail.end = 0xa64fffff, alloc.start = 0xa6480000, alloc.end = 0xa64fffff
11668 [    0.947672] zhd-debug: __find_resource() new->start = 0xa6480000, new->end = 0xa64fffff
11669 [    0.947678] zhd-debug: __find_resource() end OK
11670 [    0.947680] zhd-debug: __request_resource() start
11671 [    0.947684] zhd-debug: __request_resource() new->start = 0xa6480000, new->end = 0xa64fffff, root->start = 0xa6000000, root->end = 0xa64fffff
11672 [    0.947689] zhd-debug: __request_resource() end found
11673 [    0.947692] zhd-debug: allocate_resource() end err=0,
11674 [    0.947695] zhd-debug: pci_bus_alloc_from_region() allocate_resource end OK
11675 [    0.947699] zhd-debug: __pci_assign_resource() get address A3 align = 0x80000, size = 0x80000, resno = 0x6
11676 [    0.947704] zhd-debug: __pci_assign_resource() res->start = 0xa6480000, res->end = 0xa64fffff, res->name = 0000:1f:00.0, ret = 0x0
11677 [    0.947710] zhd-debug: pci_assign_resource() dev->vendor = 0x8086, dev->device = 0x1521
11678 [    0.947715] zhd-debug: pci_assign_resource() align = 0x80000, size = 0x80000, ret = 0x0, resno = 0x6
11679 [    0.947721] pci 0000:1f:00.0: BAR 6: assigned [mem 0xa6480000-0xa64fffff pref]

I350 parent bridge:
=> 13226 [    0.956624] pci 0000:1e:00.0: PCI bridge to [bus 1f]
=> 13233 [    0.956661] pci 0000:1e:00.0:   bridge window [mem 0xa6000000-0xa64fffff]


⭕️14892 [    0.964295] zhd-debug: pci_assign_unassigned_resources() end
⭕️14893 [    0.964298] zhd-debug: pcibios_assign_resources() end

====================================================================================
device mem
2307 [    0.935635] pci 0000:01:00.0: reg 0x10: [mem 0x92500000-0x925fffff]
2330 [    0.936038] pci 0000:01:00.0: reg 0x1c: [mem 0x9260c000-0x9260ffff]
2361 [    0.936473] pci 0000:01:00.0: reg 0x30: [mem 0xfff80000-0xffffffff pref]

read device parent bridge window
2807 [    0.956708] zhd-debug: pci_read_bridge_mmio() region.start = 0x92200000, region.end = 0x926fffff, res->flags = 0x200
2827 [    0.956788] pci 0000:00:1c.0:   bridge window [mem 0x92200000-0x926fffff]

survey resource stage:(一般BAR空间会在这里申请成功，并且这个阶段不尝试对ROM BAR分配空间)
6616 [    1.009322] zhd-debug: pcibios_resource_survey() start
7481 [    1.012404] zhd-debug: __request_resource() new->start = 0x92500000, new->end = 0x925fffff, root->start = 0x92200000, root->end = 0x926fffff
8407 [    1.015891] zhd-debug: pcibios_resource_survey() end

assign resource stage:(针对ROM BAR空间尝试分配)
8554 [    1.034148] zhd-debug: pcibios_assign_resources() start
8555 [    1.034152] zhd-debug: pcibios_allocate_rom_resources() start
8613 [    1.034367] pci 0000:01:00.0: can't claim BAR 6 [mem 0xfff80000-0xffffffff pref]: no compatible bridge window
9185 [    1.036313] zhd-debug: pci_assign_unassigned_resources() start
9186 [    1.036329] zhd-debug: pci_assign_unassigned_root_bus_resources() start
9207 [    1.036435] zhd-debug: assign_requested_resources_sorted() start
9309 [    1.036790] zhd-debug: assign_requested_resources_sorted() end
9318 [    1.036837] zhd-debug: assign_requested_resources_sorted() start
9366 [    1.036999] pci 0000:01:00.0: BAR 6: assigned [mem 0x92680000-0x926fffff pref]
9427 [    1.037199] pci 0000:01:00.1: BAR 6: no space for [mem size 0x00080000 pref]
9431 [    1.037212] pci 0000:01:00.1: BAR 6: failed to assign [mem size 0x00080000 pref]
9432 [    1.037216] zhd-debug: assign_requested_resources_sorted() EXPANSION_ROM_PATCH VAILD

setup device parent bridge window
10541 [    1.043697] zhd-debug: pci_setup_bridge_mmio() start
10544 [    1.043708] zhd-debug: pcibios_resource_to_bus() res->start = 0x92200000, res->end = 0x926fffff, res->name = PCI Bus 0000:01
10545 [    1.043712] pci 0000:00:1c.0:   bridge window [mem 0x92200000-0x926fffff]

RAID card:
12227 [    1.049851] zhd-debug: __find_resource() avail.start = 0xa6200000, avail.end = 0xa61fffff, avail.flags = 0x4e200, tmp.start = 0xa6110000, tmp.end = 0xa61fffff
12236 [    1.049884] pci 0000:1f:00.0: BAR 6: no space for [mem size 0x00100000 pref]

13727 [    1.055207] zhd-debug: pci_assign_unassigned_root_bus_resources() end
13728 [    1.055209] zhd-debug: pci_assign_unassigned_resources() end
13729 [    1.055212] zhd-debug: pcibios_assign_resources() end

关键log对应代码
[    0.935635] pci 0000:01:00.0: reg 0x10: [mem 0x92500000-0x925fffff]
    => pcibios_fixup_bus
        => pci_read_bridge_bases
            => pci_read_bridge_mmio

[    0.935333] pci 0000:01:00.0: [8086:1521] type 00 class 0x02000
    => pci_setup_device
        => dev_printk(KERN_DEBUG, &dev->dev, "[%04x:%04x] type %02x class %#08x\n",
		   dev->vendor, dev->device, dev->hdr_type, dev->class);

[    0.935635] pci 0000:01:00.0: reg 0x10: [mem 0x92500000-0x925fffff]
    => __pci_read_base
        => dev_printk(KERN_DEBUG, &dev->dev, "reg 0x%x: %pR\n", pos, res)
========================================================================================



在一般pc，Dell PowerEdge T30下，RAID卡和Mellonex网卡都是有rom的，EFI/Legacy都有rom
if (!
((idx == PCI_ROM_RESOURCE) && (!(res->flags & IORESOURCE_ROM_ENABLE)))
)

__pci_bus_assign_resources
    => pbus_assign_resources_sorted
    => pdev_assign_fixed_resources
    => __pci_bus_assign_resources

__pci_bridge_assign_resources
    => pdev_assign_resources_sorted
    => __pci_bus_assign_resources

????    
    =>pci_bus_assign_resources
        => __pci_bus_assign_resources

acpi_pci_root_add / pci_assign_unassigned_resources
pci_assign_unassigned_root_bus_resources
    => __pci_bus_assign_resources

pci_rescan_bus
pci_assign_unassigned_bus_resources
    => __pci_bus_assign_resources

__pci_bus_assign_resources
    => assign_requested_resources_sorted
        => pci_assign_resource
            =>_pci_assign_resource 
                => pci_bus_alloc_resource
                    => pci_bus_alloc_from_region
                        => allocate_resource
                            => find_resource

__pci_read_bases() normal process l64 = 0xfeb00000
==================================================================================
0. e820 map print
[    0.000000] e820: BIOS-provided physical RAM map:
[    0.000000] BIOS-e820: [mem 0x0000000000000000-0x000000000009fbff] usable
[    0.000000] BIOS-e820: [mem 0x000000000009fc00-0x000000000009ffff] reserved
[    0.000000] BIOS-e820: [mem 0x00000000000f0000-0x00000000000fffff] reserved
[    0.000000] BIOS-e820: [mem 0x0000000000100000-0x000000003ffdefff] usable
[    0.000000] BIOS-e820: [mem 0x000000003ffdf000-0x000000003fffffff] reserved
[    0.000000] BIOS-e820: [mem 0x00000000b0000000-0x00000000bfffffff] reserved
[    0.000000] BIOS-e820: [mem 0x00000000feffc000-0x00000000feffffff] reserved
[    0.000000] BIOS-e820: [mem 0x00000000fffc0000-0x00000000ffffffff] reserved


#0  e820_print_map (who=who@entry=0xffffffff81979654 "BIOS-e820") at arch/x86/kernel/e820.c:176
#1  0xffffffff81bfe178 in setup_memory_map () at arch/x86/kernel/e820.c:1644
#2  0xffffffff81bfab5f in setup_arch (cmdline_p=cmdline_p@entry=0xffffffff81a87f50 <init_thread_union+16208>)
    at arch/x86/kernel/setup.c:1061
#3  0xffffffff81bedd29 in start_kernel () at init/main.c:516
#4  0xffffffff81bed5ef in x86_64_start_reservations (
    real_mode_data=real_mode_data@entry=0x14520 <init_tss+1056> <error: Cannot access memory at address 0x14520>)
    at arch/x86/kernel/head64.c:193
#5  0xffffffff81bed740 in x86_64_start_kernel (
    real_mode_data=0x14520 <init_tss+1056> <error: Cannot access memory at address 0x14520>) at arch/x86/kernel/head64.c:182
#6  0x0000000000000000 in ?? ()
==================================================================================


================================================================================
1.first request_resource
[    0.000000] zhd-debug: __request_resource() start
[    0.000000] zhd-debug: __request_resource() new->start = 0xc0000, new->end = 0xc95ff, root->start = 0x0, root->end = 0xffffffff
[    0.000000] zhd-debug: __request_resource() end found
[    0.000000] zhd-debug: __request_resource() start
[    0.000000] zhd-debug: __request_resource() new->start = 0xf0000, new->end = 0xfffff, root->start = 0x0, root->end = 0xffffffff
[    0.000000] zhd-debug: __request_resource() end found
[    0.000000] zhd-debug: __request_resource() start
[    0.000000] zhd-debug: __request_resource() new->start = 0xc9800, new->end = 0xca5ff, root->start = 0x0, root->end = 0xffffffff
[    0.000000] zhd-debug: __request_resource() end found

#0  __request_resource (root=root@entry=0xffffffff81acc3c0 <iomem_resource>, new=new@entry=0xffffffff81ab2380 <video_rom_resource>)
    at kernel/resource.c:209
#1  0xffffffff810f0389 in request_resource_conflict (root=0xffffffff81acc3c0 <iomem_resource>, 
    new=new@entry=0xffffffff81ab2380 <video_rom_resource>) at kernel/resource.c:312
#2  0xffffffff810f03ae in request_resource (root=<optimized out>, new=new@entry=0xffffffff81ab2380 <video_rom_resource>)
    at kernel/resource.c:328
#3  0xffffffff81bfbf3d in probe_roms () at arch/x86/kernel/probe_roms.c:223
#4  0xffffffff81bfae2e in setup_arch (cmdline_p=cmdline_p@entry=0xffffffff81a87f50 <init_thread_union+16208>)
    at arch/x86/kernel/setup.c:1143
#5  0xffffffff81bedd29 in start_kernel () at init/main.c:516
#6  0xffffffff81bed5ef in x86_64_start_reservations (
    real_mode_data=real_mode_data@entry=0x14520 <init_tss+1056> <error: Cannot access memory at address 0x14520>)
    at arch/x86/kernel/head64.c:193
#7  0xffffffff81bed740 in x86_64_start_kernel (
    real_mode_data=0x14520 <init_tss+1056> <error: Cannot access memory at address 0x14520>) at arch/x86/kernel/head64.c:182
#8  0x0000000000000000 in ?? ()
===================================================================================

=====================================================================================
2.second request_resource
[    0.000000] zhd-debug: __request_resource() start
[    0.000000] zhd-debug: __request_resource() new->start = 0x0, new->end = 0xfff, root->start = 0x0, root->end = 0xffffffff
[    0.000000] zhd-debug: __request_resource() end found
[    0.000000] zhd-debug: __request_resource() start
[    0.000000] zhd-debug: __request_resource() new->start = 0x1000, new->end = 0x9fbff, root->start = 0x0, root->end = 0xffffffff
[    0.000000] zhd-debug: __request_resource() end found
[    0.000000] zhd-debug: __request_resource() start
[    0.000000] zhd-debug: __request_resource() new->start = 0x9fc00, new->end = 0x9ffff, root->start = 0x0, root->end = 0xffffffff
[    0.000000] zhd-debug: __request_resource() end found
[    0.000000] zhd-debug: __request_resource() start
[    0.000000] zhd-debug: __request_resource() new->start = 0xf0000, new->end = 0xfffff, root->start = 0x0, root->end = 0xffffffff
[    0.000000] zhd-debug: __request_resource() end unknown tmp->start = 0xf0000, tmp->end = 0xfffff
[    0.000000] zhd-debug: __request_resource() start
[    0.000000] zhd-debug: __request_resource() new->start = 0x100000, new->end = 0x3ffdefff, root->start = 0x0, root->end = 0xffffffff
[    0.000000] zhd-debug: __request_resource() end unknown tmp->start = 0x1000000, tmp->end = 0x172171e
__request_resource() new->start = 0xc0000, new->end = 0xc95ff, root->start = 0x0, root->end = 0xffffffff


#0  __request_resource (root=root@entry=0xffffffff81acc3c0 <iomem_resource>, new=new@entry=0xffff8802bff8d4f8)
    at kernel/resource.c:209
#1  0xffffffff810ef876 in __insert_resource (parent=parent@entry=0xffffffff81acc3c0 <iomem_resource>, 
    new=new@entry=0xffff8802bff8d4f8) at kernel/resource.c:806
#2  0xffffffff810f0ec9 in insert_resource_conflict (parent=0xffffffff81acc3c0 <iomem_resource>, new=new@entry=0xffff8802bff8d4f8)
    at kernel/resource.c:871
#3  0xffffffff810f0eee in insert_resource (parent=<optimized out>, new=new@entry=0xffff8802bff8d4f8) at kernel/resource.c:890
#4  0xffffffff81bfdf76 in e820_reserve_resources () at arch/x86/kernel/e820.c:1531
#5  0xffffffff81bfb711 in setup_arch (cmdline_p=cmdline_p@entry=0xffffffff81a87f50 <init_thread_union+16208>)
    at arch/x86/kernel/setup.c:1366
#6  0xffffffff81bedd29 in start_kernel () at init/main.c:516
#7  0xffffffff81bed5ef in x86_64_start_reservations (
    real_mode_data=real_mode_data@entry=0x14520 <init_tss+1056> <error: Cannot access memory at address 0x14520>)
    at arch/x86/kernel/head64.c:193
#8  0xffffffff81bed740 in x86_64_start_kernel (
    real_mode_data=0x14520 <init_tss+1056> <error: Cannot access memory at address 0x14520>) at arch/x86/kernel/head64.c:182
#9  0x0000000000000000 in ?? ()
===================================================================================


===================================================================================
3.third request_resource

hb kernel/resource.c:209 if new->start == 0x1000

[    0.000000] zhd-debug: __request_resource() start
[    0.000000] zhd-debug: __request_resource() new->start = 0x0, new->end = 0x1f, root->start = 0x0, root->end = 0xffff
[    0.000000] zhd-debug: __request_resource() end found
[    0.000000] zhd-debug: __request_resource() start
[    0.000000] zhd-debug: __request_resource() new->start = 0x20, new->end = 0x21, root->start = 0x0, root->end = 0xffff
[    0.000000] zhd-debug: __request_resource() end found
[    0.000000] zhd-debug: __request_resource() start
[    0.000000] zhd-debug: __request_resource() new->start = 0x40, new->end = 0x43, root->start = 0x0, root->end = 0xffff
[    0.000000] zhd-debug: __request_resource() end found
[    0.000000] zhd-debug: __request_resource() start
[    0.000000] zhd-debug: __request_resource() new->start = 0x50, new->end = 0x53, root->start = 0x0, root->end = 0xffff
[    0.000000] zhd-debug: __request_resource() end found


#0  __request_resource (root=root@entry=0xffffffff81acc400 <ioport_resource>, 
    new=new@entry=0xffffffff81ab1d18 <standard_io_resources+56>) at kernel/resource.c:209
#1  0xffffffff810f0389 in request_resource_conflict (root=0xffffffff81acc400 <ioport_resource>, 
    new=new@entry=0xffffffff81ab1d18 <standard_io_resources+56>) at kernel/resource.c:312
#2  0xffffffff810f03ae in request_resource (root=<optimized out>, new=new@entry=0xffffffff81ab1d18 <standard_io_resources+56>)
    at kernel/resource.c:328
#3  0xffffffff81bfa998 in reserve_standard_io_resources () at arch/x86/kernel/setup.c:706
#4  0xffffffff81bfb723 in setup_arch (cmdline_p=cmdline_p@entry=0xffffffff81a87f50 <init_thread_union+16208>)
    at arch/x86/kernel/setup.c:1369
#5  0xffffffff81bedd29 in start_kernel () at init/main.c:516
#6  0xffffffff81bed5ef in x86_64_start_reservations (
    real_mode_data=real_mode_data@entry=0x14520 <init_tss+1056> <error: Cannot access memory at address 0x14520>)
    at arch/x86/kernel/head64.c:193
#7  0xffffffff81bed740 in x86_64_start_kernel (
    real_mode_data=0x14520 <init_tss+1056> <error: Cannot access memory at address 0x14520>) at arch/x86/kernel/head64.c:182
#8  0x0000000000000000 in ?? ()
=====================================================================================

=====================================================================================
5. e820 mem gap for pcie devices
[    0.000000] e820: [mem 0xc0000000-0xfeffbfff] available for PCI devices

#0  e820_setup_gap () at arch/x86/kernel/e820.c:645
#1  0xffffffff81bfb728 in setup_arch (cmdline_p=cmdline_p@entry=0xffffffff81a87f50 <init_thread_union+16208>)
    at arch/x86/kernel/setup.c:1371
#2  0xffffffff81bedd29 in start_kernel () at init/main.c:516
#3  0xffffffff81bed5ef in x86_64_start_reservations (
    real_mode_data=real_mode_data@entry=0x14520 <init_tss+1056> <error: Cannot access memory at address 0x14520>)
    at arch/x86/kernel/head64.c:193
#4  0xffffffff81bed740 in x86_64_start_kernel (
    real_mode_data=0x14520 <init_tss+1056> <error: Cannot access memory at address 0x14520>) at arch/x86/kernel/head64.c:182
#5  0x0000000000000000 in ?? ()
=====================================================================================


=====================================================================================
6. fourth request_resource for pci root bus

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
7. root bus create call stack:

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

=====================================================================================
8.pcie device read rom address

[    0.295030] zhd-debug: pci_read_bases() calling __pci_read_base pci_bar_mem32 = 0x2, rom = 0x30
[    0.296003] zhd-debug: __pci_read_base() dev->vendor = 0x8086, dev->device = 0x1521
[    0.297031] zhd-debug: __pci_read_bases() pos = 0x30 , type = 0x2, res->flags = 0x4e200, occured start print log
[    0.299096] zhd-debug: __pci_read_bases() l = 0xfeb00000, sz = 0xfff80000, pos = 0x30
[    0.300003] zhd-debug: __pci_read_bases() type != pci_bar_unkonwn l64 = 0xfeb00000, sz64 = 0xfff80000, res->flags = 0x4e200
[    0.306008] zhd-debug: __pci_read_bases() normal process l64 = 0xfeb00000, sz64 = 0x7ffff, res->flags = 0x4e200

#0  __pci_read_base (dev=dev@entry=0xffff8802b1c0c000, 
    type=type@entry=pci_bar_mem32, res=res@entry=0xffff8802b1c0c490, 
    pos=pos@entry=48) at drivers/pci/probe.c:403
#1  0xffffffff813c13cf in pci_read_bases (dev=0xffff8802b1c0c000, howmany=6, 
    rom=48) at drivers/pci/probe.c:480
#2  0xffffffff813c2023 in pci_setup_device (dev=dev@entry=0xffff8802b1c0c000)
    at drivers/pci/probe.c:1381
#3  0xffffffff813c26a2 in pci_scan_device (devfn=16, bus=0xffff8802b1c20800)
    at drivers/pci/probe.c:1780
#4  pci_scan_single_device (bus=0xffff8802b1c20800, devfn=16)
    at drivers/pci/probe.c:1881
#5  0xffffffff813c272a in pci_scan_slot (bus=0xffff8802b1c20800, devfn=16)
    at drivers/pci/probe.c:1959
#6  0xffffffff813c3b6d in pci_scan_child_bus (bus=bus@entry=0xffff8802b1c20800)
    at drivers/pci/probe.c:2144
#7  0xffffffff815c8142 in pci_acpi_scan_root (
    root=root@entry=0xffff8802b23d4d80) at arch/x86/pci/acpi.c:470
#8  0xffffffff8140df38 in acpi_pci_root_add (device=0xffff8802b23f4000, 
    not_used=<optimized out>) at drivers/acpi/pci_root.c:589
#9  0xffffffff814099aa in acpi_scan_attach_handler (device=0xffff8802b23f4000)
    at drivers/acpi/scan.c:1820
#10 acpi_bus_attach (device=device@entry=0xffff8802b23f4000)
    at drivers/acpi/scan.c:1858
--Type <RET> for more, q to quit, c to continue without paging--
#11 0xffffffff81409a22 in acpi_bus_attach (
    device=device@entry=0xffff8802b23f3800) at drivers/acpi/scan.c:1872
#12 0xffffffff81409a22 in acpi_bus_attach (device=0xffff8802b23f3000)
    at drivers/acpi/scan.c:1872
#13 0xffffffff81409b18 in acpi_bus_scan (
    handle=handle@entry=0xffffffffffffffff) at drivers/acpi/scan.c:1901
#14 0xffffffff81c39991 in acpi_scan_init () at drivers/acpi/scan.c:2006
#15 0xffffffff81c397b8 in acpi_init () at drivers/acpi/bus.c:740
#16 0xffffffff810020e8 in do_one_initcall (fn=0xffffffff81c3953f <acpi_init>)
    at init/main.c:772
#17 0xffffffff81bee226 in do_initcall_level (level=<optimized out>)
    at init/main.c:838
#18 do_initcalls () at init/main.c:846
#19 do_basic_setup () at init/main.c:864
#20 kernel_init_freeable () at init/main.c:968
#21 0xffffffff816fa19e in kernel_init (unused=<optimized out>)
    at init/main.c:899
#22 <signal handler called>
#23 0x0000000000000000 in irq_stack_union ()
#24 0x0000000000000000 in ?? ()
====================================================================================
1.__request_resource
setup_arch
    => probe_roms //video rom, system rom, extension rom, adapter roms
        => request_resource
            => request_resource_conflict
                => __request_resource
2.__request_resource
setup_arch
    => e820_reserve_resources //e820, mark busy
        => insert_resource
            => insert_resource_conflict 
                => __insert_resource
                    => __request_resource

3.__request_resource
setup_arch
    => reserve_standard_io_resources  //io resource
        => request_resource
            => request_resource_conflict
                => __request_resource

4.__request_resource
acpi_init
    => acpi_scan_init 
        => acpi_bus_scan
            => acpi_bus_attach
                => acpi_scan_attach_handler
                    => acpi_pci_root_add
                        => pci_acpi_scan_root
                            => add_resources
                                => insert_resource_conflict
                                    => __insert_resource
                                        => __request_resource

pci_scan_bridge
    => pci_add_new_bus
        => pci_alloc_child_bus

subsys_initcall(acpi_init)
acpi_init
    => acpi_scan_init
        => acpi_bus_scan
            => acpi_bus_attach
                => acpi_scan_attach_handler
                    => acpi_pci_root_add
                        => pci_acpi_scan_root
                            => pci_add_resource(&resources, &root->secondary)
                            => add_resources //pci_use_crs
                            => pci_create_root_bus      //创建root bus
                                => pci_alloc_host_bridge
                                => pcibios_add_bus
                                    => acpi_pci_add_bus
                            => pci_scan_child_bus
                                => pci_scan_slot
                                    => pci_scan_single_device
                                        => dev = pci_get_slot(bus, devfn);
                                        => dev = pci_scan_device(bus, devfn)
                                            => pci_setup_device
                                                => pci_read_bases(dev, 6, PCI_ROM_ADDRESS)  //读出所有RESOURCE的设定
                                                    => __pci_read_base (All BAR)
                                        => pci_device_add(dev, bus) 
                                => pcibios_fixup_bus
                                    => pci_read_bridge_bases    //读取Bridge的RESOURCE设定
                                        => pci_read_bridge_io
                                        => pci_read_bridge_mmio
                                        => pci_read_bridge_mmio_pref
                                    => pcibios_fixup_device_resources
                                => pci_scan_bridge
                                    => pci_add_new_bus
                                        => pci_alloc_child_bus //创建child bus
                                            => pcibios_add_bus
                                                => acpi_pci_add_bus
                                    => pci_scan_child_bus
                                        => pci_scan_slot        //同上面的root bus，接下来会扫描Child Bus下的所有设备
                        => pci_bus_add_devices(root->bus)       //循环遍历，为所有bus添加设备
                            => pci_bus_add_device
                                => pcibios_bus_add_device 
                                => pci_fixup_device
                                => pci_create_sysfs_dev_files
                                        

subsys_initcall(pci_subsys_init);
    => pci_subsys_init
        => pcibios_init
            => pcibios_resource_survey
                => pcibios_allocate_bus_resources    //DFS,分配bus resource
                    => pcibios_allocate_bridge_resources
                        => pci_claim_bridge_resource
                            => pci_claim_resource
                                => pci_find_parent_resource
                                => request_resource_conflict
                                    => __request_resource
                => pcibios_allocate_resources(bus, 0);
                    => pcibios_allocate_dev_resources //主要为标准BAR空间分配resource
                        => pci_claim_resource
                            => pci_find_parent_resource
                            => request_resource_conflict
                                => __request_resource
                    => pcibios_allocate_resources
            	=> pcibios_allocate_resources(bus, 1);
                    => pcibios_allocate_dev_resources
                        => pci_claim_resource
                            => pci_find_parent_resource
                            => request_resource_conflict
                                => __request_resource
                    => pcibios_allocate_resources

fs_initcall(pcibios_assign_resources)
    => pcibios_allocate_rom_resources  
        => pcibios_allocate_dev_rom_resource    //先在这里面使用BIOS的设定进行ROM空间分配
            => pci_claim_resource               //Claim Resource for rom
    => pci_assign_unassigned_resources          //如果上面分配失败，在这里对ROM空间分配
        => pci_assign_unassigned_root_bus_resources
            => __pci_bus_size_bridges
                => pbus_size_mem
                    => bridge window %pR to %pR add_size %llx add_align 
            => __pci_bus_assign_resources
                => pbus_assign_resources_sorted
                    => __dev_sort_resources
                    => __assign_resources_sorted
                        => assign_requested_resources_sorted
                            => pci_assign_resource
                                => _pci_assign_resource
                                    => __pci_assign_resource
                                        => pci_bus_alloc_resource //alignf=pcibios_align_resource
                                            => pci_bus_alloc_from_region
                                                => allocate_resource
                                                    => find_resource
                                                        => __find_resource
                                                            => pcibios_align_resource //call alignf
                => pdev_assign_fixed_resources
                => __pci_bus_assign_resources
                => pci_setup_bridge
                    => pci_setup_bridge_io/pci_setup_bridge_mmio/pci_setup_bridge_mmio_pref





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
