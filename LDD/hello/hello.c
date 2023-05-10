#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>



static int __init hello_world_init (void)
{
	printk(KERN_INFO "HI....%s",__FUNCTION__);
	printk(KERN_INFO "This is simple module\n");
	printk(KERN_INFO "Kernel is successful loaded\n");
	return 0;
}

static void __exit hello_world_exit (void)
{
	printk(KERN_INFO "Kernel is removed\n");
	printk(KERN_INFO "Good bye.....%s",__FUNCTION__);
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("vishal");
MODULE_DESCRIPTION("SIMPLE PROGRAM");
//MODULE_VERSION("1:0:0");

