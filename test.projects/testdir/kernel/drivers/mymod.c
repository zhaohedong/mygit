#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/major.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>

#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/semaphore.h>

#include <asm/uaccess.h>


dev_t my_devt;

char drv_name[] = "my_chr_drv"; 
struct class *my_class;
struct device *my_device;
struct device *my_chr_device;

char global_buf[20];

struct my_super_device 
{
    struct cdev my_cdev;
    int count;
    struct semaphore my_sem;
};

struct my_super_device *my_super;

ssize_t my_read (struct file *myfile, char __user *my_buf, size_t len, loff_t *off)
{
    int ret;    
    //printk("len = %d\n",len);
    ret = copy_to_user(my_buf, global_buf, len);
    if(ret != 0)
        printk("not able to copy\n");
    printk("copied the data to user buffer\n");
    return len;
}

ssize_t my_write (struct file *my_file, const char __user *buf, size_t len, loff_t *off)
{
    
    int ret;    
    ret = copy_from_user(global_buf, buf, len);
    if(ret != 0)
        printk("not able to copy\n");
    printk("copied the data from user buffer\n");
    return len;
}

int my_mmap (struct file *my_file, struct vm_area_struct *my_vm)
{
    return 0;
}

int my_open (struct inode *my_inode, struct file *my_file)
{
    struct my_super_device *super_ptr;
    printk("open called\n");
    printk("cdev address = %p\n", my_inode->i_cdev);
    
    super_ptr = container_of(my_inode->i_cdev, struct my_super_device, my_cdev);    
    up(&(super_ptr->my_sem));
    printk("super address = %p\n", super_ptr);
    return 0;
}

int my_release (struct inode *my_inode, struct file *my_file)
{
    struct my_super_device *super_ptr;
    printk("release called\n");

    super_ptr = container_of(my_inode->i_cdev, struct my_super_device, my_cdev);    
    down(&(super_ptr->my_sem));
    return 0;
}

unsigned int my_poll (struct file *my_file, struct poll_table_struct *poll_table)
{
    return 0;
}

struct file_operations myfops = {
    .owner = THIS_MODULE,
    .read = my_read,
    .write = my_write,
    .poll = my_poll,
    .open = my_open,
    .release = my_release,
};

int my_bus_match(struct device *dev, struct device_driver *drv)
{
    printk("Entered %s\n",__func__);
    return 1;
}
int my_bus_uevent(struct device *dev, struct kobj_uevent_env *env)
{
    printk("Entered %s\n",__func__);
    return 0;
}

void my_bus_release(struct device *dev)
{
    printk("Entered %s\n",__func__);
    return ;
}

struct device my_bus = {
        .init_name = "my_bus",
    .release = my_bus_release,
};

struct bus_type my_bus_type = {
    .name = "my_bus",
    .match = my_bus_match,
    .uevent = my_bus_uevent,
};

int my_driver_probe(struct device *dev)
{
    int ret;    

    printk("Entered %s\n",__func__);

        ret = alloc_chrdev_region(&my_devt, 0, 1, drv_name);
        if(ret == 0)
                printk("dev_t major is %d minor is %d\n", MAJOR(my_devt), MINOR(my_devt));

        my_class =  class_create(THIS_MODULE, "my_class");
        if(my_class != NULL)
                printk("class created as %s\n", my_class->name);

        my_chr_device = device_create(my_class, NULL, my_devt, NULL, "my_chr_drv");
        if(my_chr_device != NULL)
                printk("device created dev_t as  %d\n", my_chr_device->devt);

        my_super = kzalloc(sizeof(struct my_super_device), GFP_KERNEL);

        sema_init(&(my_super->my_sem), 1);

        cdev_init(&(my_super->my_cdev), &myfops);

        cdev_add(&(my_super->my_cdev), my_devt, 1);
    return 0;
}

int my_driver_remove(struct device *dev)
{
    printk("Entered %s\n",__func__);

    if(my_chr_device != NULL)
        device_destroy(my_class, my_devt);
    if(my_class != NULL)
        class_destroy(my_class);
    if(my_devt != 0)
        unregister_chrdev_region(my_devt,1);

    return 0;
}

struct device_driver my_dev_driver = {
    .owner = THIS_MODULE,
    .name = "my_chr_drv",
    .bus = &my_bus_type,
    .probe = my_driver_probe,
    .remove = my_driver_remove,
};

static int __init my_init(void)
{
     int ret = 0;

    printk("in init\n");

    /*Register the bus*/
    ret = device_register(&my_bus);
    if(ret == 0)
        printk("device registerd correctly\n");
    else
        printk("ret = %d\n",ret);

    ret = bus_register(&my_bus_type);
    if(ret == 0)
        printk("bus registered correctly\n");
    else
        printk("ret = %d\n",ret);

    /*add device for the device driver*/
    my_device = kzalloc(sizeof(struct device), GFP_KERNEL);
    device_initialize(my_device);

    my_device->parent = &my_bus;
    my_device->bus = &my_bus_type;
    my_device->init_name = "my_chr_dev";

    ret = device_add(my_device);
    if(ret == 0)
        printk("device added correctly\n");
    else
        printk("ret = %d\n",ret);

    /*Now register the driver*/
    ret = driver_register(&my_dev_driver);
    if(ret == 0)
        printk("driver registered correctly\n");
    else
        printk("ret = %d\n",ret);

    return 0;
}

static void __exit my_exit(void)
{
    printk("in exit\n");
    driver_unregister(&my_dev_driver);
    bus_unregister(&my_bus_type);
    device_unregister(&my_bus);
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("Kundan");
MODULE_LICENSE("GPL");

