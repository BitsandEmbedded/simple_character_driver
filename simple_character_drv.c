#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/fs.h>

#define DEV_NAME "simple_char_dev"
#define MAX_MINOR_COUNT 5
#define MAX_BUF_LEN 256
#define BUF_STR "Hello! I am char driver"

struct simple_char_drv_info {
	struct cdev c_dev;
	dev_t devid;
	int major;
};


static struct simple_char_drv_info *ptr_drv_info = NULL;

static int simple_char_drv_open(struct inode *node, struct file *f) {
	pr_info("Enter %s:%d\n", __func__, __LINE__);
	
	return 0;
}

static int simple_char_drv_close(struct inode *node, struct file *f) {
	pr_info("Enter %s:%d\n", __func__, __LINE__);

	return 0;
}

static ssize_t simple_char_drv_read(struct file *f, char __user *buf, size_t size, loff_t *offset) {
	
	pr_info("Enter %s:%d\n", __func__, __LINE__);

	return size;
}

static ssize_t simple_char_drv_write(struct file *f, const char __user *buf, size_t size, loff_t *offset) {
	
	pr_info("Enter %s:%d\n", __func__, __LINE__);
	
	return size;
}

static const struct file_operations simple_char_drv_fops = {
	.owner   = THIS_MODULE,
	.open = simple_char_drv_open,
	.release = simple_char_drv_close,
	.read = simple_char_drv_read,
	.write = simple_char_drv_write,
};

int __init module_simple_char_init(void) 
{
	int ret = 0;

	pr_info("Enter %s:%d\n", __func__, __LINE__);

	ptr_drv_info = (struct simple_char_drv_info *)kmalloc(sizeof(struct simple_char_drv_info), GFP_KERNEL);
	if (!ptr_drv_info) {
		ret = -ENOMEM;
		goto error_1;
	}

	ret = alloc_chrdev_region(&ptr_drv_info->devid,0, MAX_MINOR_COUNT, DEV_NAME);
	if (ret) {
		pr_err("Failed to alloc chardev region %d\n", ret);
		goto error_2;
	}

	ptr_drv_info->major = MAJOR(ptr_drv_info->devid);

	cdev_init(&ptr_drv_info->c_dev, &simple_char_drv_fops);
	ret = cdev_add(&ptr_drv_info->c_dev, ptr_drv_info->devid, MAX_MINOR_COUNT);
	if (ret) {
		pr_err("Error in adding cdev %d\n", ret);
		goto error_3;
	}

	pr_info("%s : Init completed successfully\n", __func__);

	return ret;

error_3:
	unregister_chrdev_region(ptr_drv_info->devid, MAX_MINOR_COUNT);

error_2:
	if (ptr_drv_info)
		kfree(ptr_drv_info);

error_1:
	pr_err("%s : Error in Init %d\n", __func__, ret);

	return ret;
}

void __exit module_simple_char_exit(void)
{

	if (!ptr_drv_info) {
		pr_err("%s : Driver info is NULL\n", __func__);
		return;
	}	

	cdev_del(&ptr_drv_info->c_dev);

	unregister_chrdev_region(ptr_drv_info->devid, MAX_MINOR_COUNT);

	kfree(ptr_drv_info);

	pr_info("%s : Exit\n", __func__);
}

module_init(module_simple_char_init);
module_exit(module_simple_char_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hardik Gajjar <ghardik78@yahoo.com>");
MODULE_DESCRIPTION("A simple character device driver");
