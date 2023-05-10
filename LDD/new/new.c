#include <linux/init.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include<linux/slab.h>                 //kmalloc()
#include<linux/uaccess.h>
#include <linux/err.h>
#include <linux/kernel.h>

#define DEV_MEM_SIZE 512
#define mem_size        1024

uint8_t *kernel_buffer;

/*pseudo devices imemory*/
char device_buffer[DEV_MEM_SIZE];

/*
 ** This function will be called when we read the Device file
 */
static ssize_t file_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
	//Copy the data from the kernel space to the user-space
	if( copy_to_user(buf, kernel_buffer, len ) )
	{
		pr_err("Data Read : Err!\n");
		return 0;
	}
	else
	{
		pr_info("Driver Read Function Called...!!!\n");
		return len ;
	}
}
/*
 ** This function will be called when we write the Device file
 */
static ssize_t file_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
	//Copy the data to kernel space from the user-space
	if( copy_from_user(kernel_buffer, buf, len) )
	{
		pr_err("Data Write : Err!\n");
		return 0;
	}
	else
	{
		pr_info("Driver Write Function Called...!!!\n");
		return len;
	}
}



static struct net_device *my_net_device;


static int my_net_device_open(struct net_device *dev)
{
	/*Creating Physical memory*/
	if((kernel_buffer = kmalloc(mem_size , GFP_KERNEL)) == 0){
		pr_info("Cannot allocate memory in kernel\n");
		return -1;
	}
	else{
		printk(KERN_INFO "my_net_device: device opened\n");
		return 0;
	}
}

static int my_net_device_release(struct net_device *dev)
{
	kfree(kernel_buffer);

	printk(KERN_INFO "my_net_device: device closed\n");
	return 0;
}

static int  my_net_device_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct sk_buff *new_skb;
	char *data;

	// allocate a new skb
	new_skb = dev_alloc_skb(skb->len + sizeof(int));
	if (!new_skb) {
		return -1;
	}

	// copy the data from the original skb to the new skb
	skb_copy_bits(skb, 0, new_skb->data, skb->len);
	data = new_skb->data + skb->len;

	// append some extra data to the new skb
	*(int *)data = 123;
	new_skb->len = skb->len + sizeof(int);

	// send the new skb to the other network driver
	dev_queue_xmit(new_skb);

	printk(KERN_INFO "my_net_device: transmitting packet\n");
	return 0;
}

static const struct net_device_ops my_net_device_ops = {
	.ndo_open = my_net_device_open,
	.ndo_stop = my_net_device_release,
	.ndo_start_xmit = my_net_device_start_xmit,
	.read       = file_read,
	.write      = file_write,

};

static int __init my_net_device_init(void)
{
	my_net_device =alloc_etherdev(sizeof(my_net_device));

	my_net_device->	netdev_ops = &my_net_device_ops;
	strcpy(my_net_device->name, "my_net_device");
	if(register_netdev(my_net_device) < 0)
	{
		printk(KERN_ERR "my_net_device: module loaded failed\n");
		return -1 ;
	}
	else{
		printk(KERN_INFO "my_net_device: module loaded\n");
		return 0;
	}
}

static void __exit my_net_device_exit(void)
{
	unregister_netdev(my_net_device);
	free_netdev(my_net_device);
	printk(KERN_INFO "my_net_device: module unloaded\n");
}


module_init(my_net_device_init);
module_exit(my_net_device_exit);

MODULE_AUTHOR("VISHAL WAGH");
MODULE_DESCRIPTION("A simple network device driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
