#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/pci.h>
#include <linux/moduleparam.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
MODULE_AUTHOR("zhao-hd@neusoft.com");
MODULE_DESCRIPTION("NEU Read Option Rom Driver");

#define RLT_NORMAL						0
#define RLT_NO_OPTION_ROM				1
#define RLT_ERR_OPTION_ROM_FORMAT		2
#define RLT_ERR_VZALLOC					3
#define RLT_ERR_CREATE_OPTION_ROM_FILE	4
#define RLT_ERR_ASSIGN_ROM_BAR_RES		5
#define RLT_ERR_ASSIGN_NORMAL_BAR_RES	6
#define RLT_OPTION_ROM_ALREADY_ALLOCATED 7

static struct pci_device_id neu_read_oprom_pci_table[] = {
	{PCI_DEVICE(0xffff, 0xffff)},
	{}
};

static struct file *filp;
static struct resource local_resources[PCI_ROM_RESOURCE];
static int has_resource[PCI_ROM_RESOURCE];
static u32 rom_start_addr;

//#define DEBUG_OPTION_ROM 
#ifdef DEBUG_OPTION_ROM
#define dev_dbg_oprom(dev, format, arg...)		\
	dev_printk(dev, format , ## arg)
#else
#define dev_dbg_oprom(dev, format, arg...)		\
	({ if (0) dev_printk(dev, format, ##arg); 0; })
#endif

static int backup_resource(struct pci_dev *pdev)
{
	u32 i = 0;
	int err = 0;
	int rlt = RLT_NORMAL;
	dev_dbg_oprom(KERN_DEBUG, &pdev->dev,"%s start\n", __func__);
	dev_dbg_oprom(KERN_DEBUG, &pdev->dev,"backup_resource() local_resources size = 0x%lx\n",sizeof(local_resources));
	for( i = 0; i < PCI_ROM_RESOURCE; i++)
	{
		dev_dbg_oprom(KERN_DEBUG, &pdev->dev, "C1 backup_resource() i = 0x%x, pdev->resource[i].flags = 0x%lx, pdev->resouce[i].name = %s, pdev->resource[i].start = 0x%llx, pdev->resource[i].end = 0x%llx\n",
		(int)i, pdev->resource[i].flags, pdev->resource[i].name, pdev->resource[i].start, pdev->resource[i].end);

		if(pdev->resource[i].parent) 
		{
			dev_dbg_oprom(KERN_DEBUG, &pdev->dev,"backup_resource() has_resource[i] = 0x%x\n",has_resource[i]);
			release_resource(&pdev->resource[i]);
			has_resource[i] = 1;
		}	
		local_resources[i] = pdev->resource[i]; 
		dev_dbg_oprom(KERN_DEBUG, &pdev->dev, "C2 backup_resource() i = 0x%x, pdev->resource[i].flags = 0x%lx, pdev->resouce[i].name = %s, pdev->resource[i].start = 0x%llx, pdev->resource[i].end = 0x%llx\n",
		(int)i, pdev->resource[i].flags, pdev->resource[i].name, pdev->resource[i].start, pdev->resource[i].end);
		memset(&pdev->resource[i], 0, sizeof(struct resource));
	}
	dev_dbg_oprom(KERN_DEBUG, &pdev->dev, "before pci_read_rom_base backup_resource() pdev->resource[PCI_ROM_RESOURCE].flags = 0x%lx, pdev->resouce[PCI_ROM_RESOURCE].name = %s, pdev->resource[PCI_ROM_RESOURCE].start = 0x%llx, pdev->resource[PCI_ROM_RESOURCE].end = 0x%llx\n",
	 pdev->resource[PCI_ROM_RESOURCE].flags, pdev->resource[PCI_ROM_RESOURCE].name, pdev->resource[PCI_ROM_RESOURCE].start, pdev->resource[PCI_ROM_RESOURCE].end);
	pci_read_rom_base(pdev, PCI_ROM_ADDRESS);
	dev_dbg_oprom(KERN_DEBUG, &pdev->dev, "after pci_read_rom_base backup_resource() pdev->resource[PCI_ROM_RESOURCE].flags = 0x%lx, pdev->resouce[PCI_ROM_RESOURCE].name = %s, pdev->resource[PCI_ROM_RESOURCE].start = 0x%llx, pdev->resource[PCI_ROM_RESOURCE].end = 0x%llx\n",
	 pdev->resource[PCI_ROM_RESOURCE].flags, pdev->resource[PCI_ROM_RESOURCE].name, pdev->resource[PCI_ROM_RESOURCE].start, pdev->resource[PCI_ROM_RESOURCE].end);

	err = pci_assign_resource(pdev, PCI_ROM_RESOURCE);
	dev_dbg_oprom(KERN_DEBUG, &pdev->dev, "after pci_assign_resource backup_resource() pdev->resource[PCI_ROM_RESOURCE].flags = 0x%lx, pdev->resouce[PCI_ROM_RESOURCE].name = %s, pdev->resource[PCI_ROM_RESOURCE].start = 0x%llx, pdev->resource[PCI_ROM_RESOURCE].end = 0x%llx\n",
	 pdev->resource[PCI_ROM_RESOURCE].flags, pdev->resource[PCI_ROM_RESOURCE].name, pdev->resource[PCI_ROM_RESOURCE].start, pdev->resource[PCI_ROM_RESOURCE].end);
	if (err)  
	{
		rlt = RLT_ERR_ASSIGN_ROM_BAR_RES;
		dev_printk(KERN_ERR, &pdev->dev, "%s err = 0x%x pci_assign_resource failed!\n", __func__, err);
	}
	else
	{
		dev_dbg_oprom(KERN_ERR, &pdev->dev, "%s pci_assign_resource successfully!\n", __func__);
	}
	
	dev_dbg_oprom(KERN_DEBUG, &pdev->dev, "backup_resource() end\n");
	return rlt;
}

static int restore_resource(struct pci_dev *pdev)
{
	u32 i = 0;
	int rlt = RLT_NORMAL;
	dev_dbg_oprom(KERN_DEBUG, &pdev->dev,"restore_resource() start\n");
	dev_dbg_oprom(KERN_DEBUG, &pdev->dev,"restore_resource() local_resources size = 0x%lx\n",sizeof(local_resources));

	pci_release_region(pdev, PCI_ROM_RESOURCE);
	release_resource(&pdev->resource[PCI_ROM_RESOURCE]);
	pci_write_config_dword(pdev, PCI_ROM_ADDRESS, rom_start_addr);

	for( i = 0; i < PCI_ROM_RESOURCE; i++)
	{
		pdev->resource[i] = local_resources[i]; 
		dev_dbg_oprom(KERN_DEBUG, &pdev->dev, "restore_resource() i = 0x%x, pdev->resource[i].flags = 0x%lx, pdev->resouce[i].name = %s, pdev->resource[i].start = 0x%llx, pdev->resource[i].end = 0x%llx\n",
		(int)i, pdev->resource[i].flags, pdev->resource[i].name, pdev->resource[i].start, pdev->resource[i].end);
		if(has_resource[i] == 1)
		{
			if(pci_assign_resource(pdev, i))
			{
				dev_printk(KERN_ERR, &pdev->dev, "%s vendor_id = 0x%04x, device_id = 0x%04x\n", __func__, pdev->vendor, pdev->device);
				dev_printk(KERN_ERR, &pdev->dev, "%s can not assign resource for BAR %d\n", __func__, i);
				dev_printk(KERN_ERR, &pdev->dev, "%s i = 0x%x, pdev->resource[i].flags = 0x%lx, pdev->resouce[i].name = %s, pdev->resource[i].start = 0x%llx, pdev->resource[i].end = 0x%llx\n",
					__func__, (int)i, pdev->resource[i].flags, pdev->resource[i].name, pdev->resource[i].start, pdev->resource[i].end);
				rlt = RLT_ERR_ASSIGN_NORMAL_BAR_RES;
				break;
			}
		}
	}
	memset(local_resources, 0, sizeof(local_resources));
	memset(has_resource, 0, sizeof(has_resource));
	dev_dbg_oprom(KERN_DEBUG, &pdev->dev,"restore_resource() end\n");
	return rlt;
}

static int neu_read_oprom_probe(struct pci_dev *pdev,
			     const struct pci_device_id *id)
{
	int rlt = RLT_NORMAL;
	int err = 0;
	void *rom_addr = 0;
	u8 *buf = NULL;
	size_t size = 0;
	char vendor_id[16] = {0};
	char device_id[16] = {0};
	char rom_name[80] = "/tmp/";
	char rlt_name[80] = "";
	char bus_name[48] = {0};
	char bus_name_tmp[48] = {0};
	char dev_name[16] = {0};
	char fn_name[16] = {0};
	char *token = NULL;
	char *p_bus_name = NULL;
	char str_rlt[4] = {0};

	snprintf(dev_name, 16, "%02x", PCI_SLOT(pdev->devfn));
	snprintf(fn_name, 16, "%01x", PCI_FUNC(pdev->devfn));

	strncpy(bus_name_tmp, pdev->bus->name, 48);
	p_bus_name = &bus_name_tmp[0];
	token = strsep(&p_bus_name, " ");
	if(token != NULL) token = strsep(&p_bus_name, " ");
	if(token != NULL) token = strsep(&p_bus_name, " ");
	strncpy(bus_name, token, 48);

	strncat(bus_name, ":", 48);
	strncat(bus_name, dev_name, 48);
	strncat(bus_name, ".", 48);
	strncat(bus_name, fn_name, 48);
	strncat(bus_name, "_", 48);

	snprintf(vendor_id, 16, "%04x", pdev->vendor);
	snprintf(device_id, 16, "%04x", pdev->device);
	strncat(vendor_id, "_", 16);
	strncat(vendor_id, device_id, 16);
	strncat(rom_name, bus_name, 80);
	strncat(rom_name, vendor_id, 80);
	strncpy(rlt_name, rom_name, 80);
	strncat(rom_name, ".rom", 80);
	strncat(rlt_name, ".rlt", 80);

	pci_read_config_dword(pdev, PCI_ROM_ADDRESS, &rom_start_addr);
	dev_dbg_oprom(KERN_DEBUG, &pdev->dev, "neu_read_oprom_probe() rom_start_addr = %llx\n", (long long int)rom_start_addr);

	dev_dbg_oprom(KERN_DEBUG, &pdev->dev, "before pci_read_rom_base neu_read_oprom_probe() pdev->resource[PCI_ROM_RESOURCE].flags = 0x%lx, pdev->resouce[PCI_ROM_RESOURCE].name = %s, pdev->resource[PCI_ROM_RESOURCE].start = 0x%llx, pdev->resource[PCI_ROM_RESOURCE].end = 0x%llx\n",
	 pdev->resource[PCI_ROM_RESOURCE].flags, pdev->resource[PCI_ROM_RESOURCE].name, pdev->resource[PCI_ROM_RESOURCE].start, pdev->resource[PCI_ROM_RESOURCE].end);
	pci_read_rom_base(pdev, PCI_ROM_ADDRESS);
	dev_dbg_oprom(KERN_DEBUG, &pdev->dev, "after pci_read_rom_base neu_read_oprom_probe() pdev->resource[PCI_ROM_RESOURCE].flags = 0x%lx, pdev->resouce[PCI_ROM_RESOURCE].name = %s, pdev->resource[PCI_ROM_RESOURCE].start = 0x%llx, pdev->resource[PCI_ROM_RESOURCE].end = 0x%llx\n",
	 pdev->resource[PCI_ROM_RESOURCE].flags, pdev->resource[PCI_ROM_RESOURCE].name, pdev->resource[PCI_ROM_RESOURCE].start, pdev->resource[PCI_ROM_RESOURCE].end);

	if ((pdev->resource[PCI_ROM_RESOURCE].end - pdev->resource[PCI_ROM_RESOURCE].start) == 0)
	{
		dev_printk(KERN_INFO, &pdev->dev, "%s vendor_id = 0x%04x, device_id = 0x%04x option rom not exist\n", __func__, pdev->vendor, pdev->device);
		rlt = RLT_NO_OPTION_ROM;
		goto gen_rlt;
	}

	if(pdev->resource[PCI_ROM_RESOURCE].parent) 
	{
		rlt = RLT_OPTION_ROM_ALREADY_ALLOCATED;
		dev_printk(KERN_INFO, &pdev->dev, "%s vendor_id = 0x%04x, device_id = 0x%04x option rom already allocated\n", __func__, pdev->vendor, pdev->device);
		goto gen_rlt;
	}
	else
	{
		rlt = backup_resource(pdev);
		if (rlt == RLT_ERR_ASSIGN_ROM_BAR_RES) goto err_pci_reg;
		dev_printk(KERN_INFO, &pdev->dev, "%s vendor_id = 0x%04x, device_id = 0x%04x start dumping option rom\n", __func__, pdev->vendor, pdev->device);
	}

	dev_dbg_oprom(KERN_DEBUG, &pdev->dev,"neu_read_oprom_probe() resource[6].start = 0x%x, resource[6].end = 0x%x\n",(int)pdev->resource[6].start,(int)pdev->resource[6].end);

	err = pci_enable_device(pdev);
	if (err) {
		dev_printk(KERN_ERR, &pdev->dev, "pci_enable_device failed\n");
		goto err_enable_dev;
	}
	pci_set_master(pdev);

	err = pci_request_region(pdev, PCI_ROM_RESOURCE, "neu dsc rom");
	if(err) dev_dbg_oprom(KERN_DEBUG, &pdev->dev, "neu_read_oprom_probe() pci_request_region failed\n");
	
	rom_addr = pci_map_rom(pdev, &size);
	dev_dbg_oprom(KERN_DEBUG, &pdev->dev, "neu_read_oprom_probe() rom_addr = %llx\n", (long long int)rom_addr);

	if(readb(rom_addr) == 0x55 && readb(rom_addr+1) == 0xaa)
	{
		rlt = RLT_NORMAL;
		dev_printk(KERN_INFO, &pdev->dev, "%s vendor_id = 0x%04x, device_id = 0x%04x dump option rom sucessfully!\n", __func__, pdev->vendor, pdev->device);
	}
	else
	{
		rlt = RLT_ERR_OPTION_ROM_FORMAT;
		dev_printk(KERN_INFO, &pdev->dev, "%s vendor_id = 0x%04x, device_id = 0x%04x dump option rom failed!\n", __func__, pdev->vendor, pdev->device);
	}

	if(rom_addr && size > 0 && rlt == RLT_NORMAL)
	{
		buf = vzalloc(size);
		if(buf)
		{
			memcpy_fromio(buf, rom_addr, size);

			filp = filp_open(rom_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (filp)
			{
				dev_dbg_oprom(KERN_DEBUG, &pdev->dev, "neu_read_oprom_probe() before write filp->f_pos = 0x%x\n", (int)filp->f_pos);
				set_fs(KERNEL_DS);
				filp->f_op->write(filp, buf, size, &filp->f_pos);
				set_fs(USER_DS);
				dev_dbg_oprom(KERN_DEBUG, &pdev->dev, "neu_read_oprom_probe() after write filp->f_pos = 0x%x", (int)filp->f_pos);
				filp_close(filp,NULL);
				rlt = RLT_NORMAL;
			}
			else
			{
				dev_printk(KERN_ERR, &pdev->dev, "%s create rom file failed\n", __func__); 
				rlt = RLT_ERR_CREATE_OPTION_ROM_FILE;
			}
			vfree(buf);
		}
		else
		{
			dev_printk(KERN_ERR, &pdev->dev, "%s allocate memory rom temp use failed\n", __func__); 
			rlt = RLT_ERR_VZALLOC;
		}
	}

	if(rom_addr) pci_unmap_rom(pdev, rom_addr);

	rlt = restore_resource(pdev);

err_pci_reg:
	if (rlt != RLT_ERR_ASSIGN_NORMAL_BAR_RES && rlt != RLT_ERR_ASSIGN_ROM_BAR_RES)
	goto normal_probe;

	/* When we failed to assign normal bar res, if we generate option rom successfully!
	Let dsc-service use the option rom*/
	dev_printk(KERN_ERR, &pdev->dev, "%s err_pci_reg occurred rlt = 0x%x\n", __func__, rlt);
	pci_release_mem_regions(pdev);

	err = pci_request_regions(pdev, "neu_read_oprom");
	if (err) {
		dev_printk(KERN_ERR, &pdev->dev, "%s request region failed\n", __func__);
	}

normal_probe:
	pci_clear_master(pdev);
	pci_disable_device(pdev);

gen_rlt:
	filp = filp_open(rlt_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (filp)
	{
		snprintf(str_rlt, 4, "%d", rlt);
		set_fs(KERNEL_DS);
		filp->f_op->write(filp, &str_rlt[0], 1, &filp->f_pos);
		set_fs(USER_DS);
		filp_close(filp,NULL);
	}
	else
	{
		dev_printk(KERN_ERR, &pdev->dev, "%s generate result failed\n", __func__);
	}
	return 0;

err_enable_dev:
	return err;
}

static void neu_read_oprom_remove(struct pci_dev *pdev)
{
	dev_printk(KERN_INFO, &pdev->dev, "%s\n", __func__);
	if(rom_start_addr != 0)
	{
		rom_start_addr = 0;
		pci_clear_master(pdev);
		pci_disable_device(pdev);
	}
}

static struct pci_driver neu_read_oprom_pci_driver = {
	.name = "neu_read_oprom",
	.id_table = neu_read_oprom_pci_table,
	.probe = neu_read_oprom_probe,
	.remove = neu_read_oprom_remove,
	.suspend = NULL,
	.resume = NULL,
	.shutdown = NULL,
};

static int __init neu_read_oprom_init(void)
{
	int rval;
	printk(KERN_INFO "neu_read_oprom_init()\n");
	rval = pci_register_driver(&neu_read_oprom_pci_driver);
	return rval;
}

static void __exit neu_read_oprom_exit(void)
{
	printk(KERN_INFO "neu_read_oprom_exit()\n");
	pci_unregister_driver(&neu_read_oprom_pci_driver);
}

module_init(neu_read_oprom_init);
module_exit(neu_read_oprom_exit);