#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x683cfe8d, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x6fe9e1dc, __VMLINUX_SYMBOL_STR(trace_event_raw_init) },
	{ 0x2389bd36, __VMLINUX_SYMBOL_STR(trace_event_reg) },
	{ 0x477e59a3, __VMLINUX_SYMBOL_STR(__cpu_online_mask) },
	{ 0x7a2af7b4, __VMLINUX_SYMBOL_STR(cpu_number) },
	{ 0xb3f7646e, __VMLINUX_SYMBOL_STR(kthread_should_stop) },
	{ 0xd62c833f, __VMLINUX_SYMBOL_STR(schedule_timeout) },
	{ 0x2270cd45, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0x23626800, __VMLINUX_SYMBOL_STR(trace_output_call) },
	{ 0x16305289, __VMLINUX_SYMBOL_STR(warn_slowpath_null) },
	{ 0xdb749ecf, __VMLINUX_SYMBOL_STR(trace_event_ignore_this_pid) },
	{ 0x22d52534, __VMLINUX_SYMBOL_STR(event_triggers_call) },
	{ 0x1d716301, __VMLINUX_SYMBOL_STR(trace_event_buffer_commit) },
	{ 0x69acdf38, __VMLINUX_SYMBOL_STR(memcpy) },
	{ 0x5792f848, __VMLINUX_SYMBOL_STR(strlcpy) },
	{ 0xae73d291, __VMLINUX_SYMBOL_STR(trace_event_buffer_reserve) },
	{ 0x63c4d61f, __VMLINUX_SYMBOL_STR(__bitmap_weight) },
	{ 0xeab12d78, __VMLINUX_SYMBOL_STR(__cpu_possible_mask) },
	{ 0x3c51d923, __VMLINUX_SYMBOL_STR(kthread_stop) },
	{ 0x87477ce2, __VMLINUX_SYMBOL_STR(wake_up_process) },
	{ 0xbef1a8d9, __VMLINUX_SYMBOL_STR(kthread_create_on_node) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x1afab522, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x6d55026b, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0x11caddc7, __VMLINUX_SYMBOL_STR(trace_seq_printf) },
	{ 0x61520529, __VMLINUX_SYMBOL_STR(trace_print_symbols_seq) },
	{ 0x7ebd4be4, __VMLINUX_SYMBOL_STR(trace_print_flags_seq) },
	{ 0xcfb4a296, __VMLINUX_SYMBOL_STR(trace_print_array_seq) },
	{ 0xb73b740a, __VMLINUX_SYMBOL_STR(trace_print_bitmask_seq) },
	{ 0x9d05a363, __VMLINUX_SYMBOL_STR(trace_raw_output_prep) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x6d335ecf, __VMLINUX_SYMBOL_STR(perf_trace_run_bpf_submit) },
	{ 0xe914e41e, __VMLINUX_SYMBOL_STR(strcpy) },
	{ 0xad5f0017, __VMLINUX_SYMBOL_STR(perf_trace_buf_alloc) },
	{ 0x53569707, __VMLINUX_SYMBOL_STR(this_cpu_off) },
	{ 0x754d539c, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0xcddfa2cf, __VMLINUX_SYMBOL_STR(trace_define_field) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "C457EC538F163526A8756EF");
