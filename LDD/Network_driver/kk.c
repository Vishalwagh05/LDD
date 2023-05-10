#include <linux/module.h>
#include <linux/netdevice.h>

static struct net_device *my_device;

static int my_open(struct net_device *dev)
{
    printk("my_device: interface opened\n");
    return 0;
}

static int my_close(struct net_device *dev)
{
    printk("my_device: interface closed\n");
    return 0;
}

static const struct net_device_ops my_netdev_ops = {
    .ndo_open = my_open,
    .ndo_stop = my_close,
};

static void my_setup(struct net_device *dev)
{
    ether_setup(dev);
    dev->netdev_ops = &my_netdev_ops;
}

static int __init my_init(void)
{
    my_device = alloc_netdev(0, "my_device",0, my_setup);
    if (register_netdev(my_device))
        printk("my_device: registration failed\n");
    printk("my_device: registered\n");
    return 0;
}

static void __exit my_exit(void)
{
    unregister_netdev(my_device);
    free_netdev(my_device);
    printk("my_device: unregistered\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple network device driver");
MODULE_LICENSE("GPL");

