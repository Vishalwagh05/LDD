#include <linux/init.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>

static struct net_device *my_net_device;


static int my_net_device_open(struct net_device *dev)
{
	printk(KERN_INFO "my_net_device: device opened\n");
	return 0;
}

static int my_net_device_release(struct net_device *dev)
{
	printk(KERN_INFO "my_net_device: device closed\n");
	return 0;
}

static int  my_net_device_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	printk(KERN_INFO "my_net_device: transmitting packet\n");
	return 0;
}

static const struct net_device_ops my_net_device_ops = {
	.ndo_open = my_net_device_open,
	.ndo_stop = my_net_device_release,
	.ndo_start_xmit = my_net_device_start_xmit,
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

