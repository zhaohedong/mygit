- 
```
#define KVM_GET_MSRS              _IOWR(KVMIO, 0x88, struct kvm_msrs)
#define KVM_SET_MSRS              _IOW(KVMIO,  0x89, struct kvm_msrs)

static u32 msrs_to_save[] = {
	MSR_IA32_SYSENTER_CS, MSR_IA32_SYSENTER_ESP, MSR_IA32_SYSENTER_EIP,
	MSR_STAR,
#ifdef CONFIG_X86_64
	MSR_CSTAR, MSR_KERNEL_GS_BASE, MSR_SYSCALL_MASK, MSR_LSTAR,
#endif
	MSR_IA32_TSC, MSR_IA32_CR_PAT, MSR_VM_HSAVE_PA,
	MSR_IA32_FEATURE_CONTROL, MSR_IA32_BNDCFGS,
#ifndef CONFIG_KVM_CICADA
	MSR_TSC_AUX,
#endif
};

有的msr地址只透传了read，没有透传write 如 MSR_IA32_TSCDEADLINE

Kernel:
handle_rdmsr
    vmx_get_msr
        rdmsrl_safe

QEMU:
kvm_get_supported_msrs
    kvm_ioctl(s, KVM_GET_MSR_INDEX_LIST, &msr_list);
    kvm_ioctl(s, KVM_GET_MSR_INDEX_LIST, kvm_msr_list)

Kernel:
static u32 msrs_to_save[]
static u32 emulated_msrs[] 

kvm_init_msr_list
    rdmsr_safe(msrs_to_save[i], &dummy[0], &dummy[1])

kvm_arch_dev_ioctl
    copy_to_user(user_msr_list->indices, &msrs_to_save, num_msrs_to_save * sizeof(u32))
    copy_to_user(user_msr_list->indices + num_msrs_to_save, &emulated_msrs, num_emulated_msrs * sizeof(u32))

QEMU:
kvm_arch_put_registers
    kvm_put_msrs
        kvm_vcpu_ioctl(CPU(cpu), KVM_SET_MSRS, &msr_data)

Kernel:
kvm_arch_vcpu_ioctl
    msr_io(vcpu, argp, do_set_msr, 0)

QEMU:
kvm_arch_get_registers
    kvm_get_msrs
        kvm_vcpu_ioctl(CPU(cpu), KVM_GET_MSRS, &msr_data);

Kernel:
kvm_arch_vcpu_ioctl
    msr_io(vcpu, argp, do_get_msr, 1);
        do_get_msr
            kvm_x86_ops->get_msr(vcpu, msr) //vmx_get_msr
                case MSR_HWP_STATUS: rdmsrl_safe(MSR_HWP_STATUS, &msr_info->data);
                    native_read_msr_safe
                        asm rdmsr
                default: kvm_get_msr_common


```
- 
