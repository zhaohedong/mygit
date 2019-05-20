- kvm boot
	- ```
	  x86_64_start_kernel
	  => x86_64_start_reservations
		=> start_kernel
			=> setup_arch
				=> init_hypervisor_platform
					=> detect_hypervisor_vendor
						=> x86_hyper=h
				=> kvmclock_init
					=> kvm_register_clock
				=> kvm_guest_init
					=> smp_ops.smp_prepare_boot_cpu = kvm_smp_prepare_boot_cpu;
				=> x86_init.oem.banner
				=> register_refined_jiffies
					=> clocksource_register
						=> clocksource_select
							=> __clocksource_select
			=> smp_prepare_boot_cpu(kvm_smp_prepare_boot_cpu)
				=> kvm_guest_cpu_init
					=> kvm_register_steal_time
				=> native_smp_prepare_boot_cpu


			=> rest_init
				=> kernel_init
					=> kernel_init_freeable
						=> smp_prepare_cpus
						=> smp_init
						=> do_basic_setup
							=> driver_init
							=> do_initcalls

	  ```
