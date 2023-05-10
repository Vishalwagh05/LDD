#include<linux/module.h>
#include<linux/kernal.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/gpio.h>
#include<linux/kdev_t.h>
#include<linux/device.h>
#include<linux/delay.h>
#include<linux/err.h>

#define DEV_MEM_SIZE 10

char device_buffer[DEV_MEM_SIZE];

/* To hold the file operation performed on this driver */

ssize_t hello_read(struct file *pfile, char __user *buff,

