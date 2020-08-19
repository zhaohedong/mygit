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
	{ 0xa9366475, __VMLINUX_SYMBOL_STR(bus_register) },
	{ 0x7875fbf2, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x1e90132d, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0xe5f09474, __VMLINUX_SYMBOL_STR(driver_register) },
	{ 0xfb197e03, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x4f8b5ddb, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0xb281fa63, __VMLINUX_SYMBOL_STR(device_register) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xeba76da5, __VMLINUX_SYMBOL_STR(driver_unregister) },
	{ 0x6dc6dd56, __VMLINUX_SYMBOL_STR(down) },
	{ 0x494247c, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0xbe737d17, __VMLINUX_SYMBOL_STR(device_add) },
	{ 0xb50895c6, __VMLINUX_SYMBOL_STR(bus_unregister) },
	{ 0x161650ac, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0xccca000a, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x1e047854, __VMLINUX_SYMBOL_STR(warn_slowpath_fmt) },
	{ 0x7e689fd2, __VMLINUX_SYMBOL_STR(device_initialize) },
	{ 0x78e739aa, __VMLINUX_SYMBOL_STR(up) },
	{ 0xcbdc8014, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x5d1f1b6e, __VMLINUX_SYMBOL_STR(device_unregister) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x47b440b5, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x88db9f48, __VMLINUX_SYMBOL_STR(__check_object_size) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "1556DA37EE52AA6FBFDFBAD");
