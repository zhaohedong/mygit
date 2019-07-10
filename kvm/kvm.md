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
- vcpu
```
type_initialize
	=> skymdw_x86_cpu_class_init

main
	=> pc_init_v2_5
		=> pc_q35_init
			=> pc_cpus_init
				=> pc_new_cpu
					=> cpu_x86_create
						=> oc = x86_cpu_class_by_name(name)
						=> xcc = X86_CPU_CLASS(oc)
						=> cpu = X86_CPU(object_new(object_class_get_name(oc)));
							=> x86_cpu_initfn //先调用parent的init函数
							=> skymdw_x86_cpu_initfn  //这里面只获得cpuid limit
								=> ret = kvm_arch_get_supported_cpuid  //kernel实际的返回值保存在ret里面
									=> cpuid = get_supported_cpuid(s)
										=> try_get_cpuid   //fill 
											=> kvm_ioctl(s, KVM_GET_SUPPORTED_CPUID, cpuid);

											kernel:
												=> kvm_arch_dev_ioctl
													=> copy_from_user(&cpuid, cpuid_arg, sizeof cpuid)
													=> kvm_dev_ioctl_get_cpuid
														=> do_cpuid_ent
															=> __do_cpuid_ent
																=> do_cpuid_1_ent
																	=> cpuid_count
																		=> __cpuid
																			=> native_cpuid
																				=> asm cpuid instruction
														=> copy_to_user(entries, cpuid_entries, nent * sizeof(struct kvm_cpuid_entry2)  //fill entries
													=> copy_to_user(cpuid_arg, &cpuid, sizeof cpuid)  //fill headers
									=> g_free(cpuid)
						=> x86_cpu_parse_featurestr  //这里获得features
							=> x86_cpu_get_supported_feature_word
								=> kvm_arch_get_supported_cpuid
									=> get_supported_cpuid
										=> try_get_cpuid

					=> ...
						=> x86_cpu_realizefn
							=> x86_cpu_filter_features
								=> x86_cpu_get_supported_feature_word
									=> kvm_arch_get_supported_cpuid
									=> if (migratable_only) r &= x86_cpu_get_migratable_flags(w);  //这个会过滤掉很多属性，feature name 为空的feature(如cpuid_6_feature_name 中的NULL)，以及unmigratable feature（invtsc）
								=> report_unavailable_features

	=> qemu_kvm_cpu_thread_fn
		=> kvm_init_vcpu
			=> kvm_arch_init_vcpu
				=> cpu_x86_cpuid
					=> kvm_vcpu_ioctl(cs, KVM_SET_CPUID2, &cpuid_data);

						kernel:
						=> copy_from_user(&cpuid, cpuid_arg, sizeof cpuid)
							=> kvm_vcpu_ioctl_set_cpuid2(vcpu, &cpuid,cpuid_arg->entries);
								=> copy_from_user(&vcpu->arch.cpuid_entries, entries,cpuid->nent * sizeof(struct kvm_cpuid_entry2)) //fill cpuid in kvm
									=> kvm_x86_ops->cpuid_update(vcpu)
									=> kvm_update_cpuid(vcpu)
	kernel:
	EXIT_REASON_CPUID
		=> handle_cpuid
			=> kvm_emulate_cpuid
				=> kvm_cpuid
					=> kvm_find_cpuid_entry
						=> vcpu->arch.cpuid_entries[i]
```
