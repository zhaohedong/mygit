#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

static ulong address = 0;
module_param(address, ulong, 0644);
MODULE_PARM_DESC(address_ulong, "Get an address from user...\n");

static int size = 0;
module_param(size, int, 0644);
MODULE_PARM_DESC(size_int, "Get an size from user...\n");

static int __init init_my_module(void) {
  int i = 0;
  char *pchar = (char *)address;

  printk(KERN_INFO "Start dump data from kernel virtual address!\n");
  printk(KERN_INFO "Dump start address = 0x%lx, dump size = %d\n", address, size);

  while(i < size)
  {
	//printk("0x%x ", *(pchar+i));
	printk(KERN_INFO "address = 0x%lx, value = 0x%02x ", address+i, *(pchar+i));
	i++;
  }
  return 0;
}

static void __exit exit_my_module(void) {
  printk(KERN_INFO "End dump data from kernel virtual address!\n");
}

module_init(init_my_module);
module_exit(exit_my_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhaohedong");
