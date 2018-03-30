#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define MAJOR_NUMBER 261
#define DEVICE_NAME "amard"


/* forward declaration */
int onebyte_open(struct inode *inode, struct file *filep);
int onebyte_release(struct inode *inode, struct file *filep);
ssize_t onebyte_read(struct file *filep, char *buf, size_t count, loff_t *f_pos);
ssize_t onebyte_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos);

static void onebyte_exit(void);

/* definition of file_operation structure */
static struct file_operations onebyte_fops = {
	.read:	onebyte_read,
	.write:	onebyte_write,
	.open:	onebyte_open,
	.release:	onebyte_release
};

static char *onebyte_data = NULL;

int onebyte_open(struct inode *inode, struct file *filep)
{
	return 0; // always successful
}

int onebyte_release(struct inode *inode, struct file *filep)
{
	return 0; // always successful
}


ssize_t onebyte_read(struct file *filep, char *buf, size_t count, loff_t *f_pos)
{

	int bytes_read = 0;

	/*
	* If we're at the end of the message, return 0 signifying end of file.
	*/
	if (*msg_Ptr == 0)
		return 0;

  
	//Actually put the data into the buffer
   
	while (count && *onebyte_data) {
     	/*
     	* The buffer is in the user data segment, not the kernel segment so "*"
    	* assignment won't work. We have to use put_user which copies data from the
     	* kernel data segment to the user data segment.
     	*/

	put_user(*(onebyte_data++), buf++);
	count--;
	bytes_read++;

	return bytes_read;
  }

  /*
   * Most read functions return the number of bytes put into the buffer
   */
return bytes_read;
}
ssize_t onebyte_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos)
{

	/*please complete the function on your own*/
}

static int onebyte_init(void)
{
	int result;
	// register the device
	result = register_chrdev(MAJOR_NUMBER, DEVICE_NAME, &onebyte_fops);
	if (result < 0) {
		 printk(KERN_ALERT "Registering char device failed with %d\n", result);
		return result;
	}
	// allocate one byte of memory for storage
	// kmalloc is just like malloc, the second parameter is

	// the type of memory to be allocated.
	// To release the memory allocated by kmalloc, use kfree.
	onebyte_data = kmalloc(sizeof(char), GFP_KERNEL);
	if (!onebyte_data) {
		onebyte_exit();
		// cannot allocate memory
		// return no memory error, negative signify a failure
		return -ENOMEM;
	}
	// initialize the value to be X

	*onebyte_data = 'X';
	printk(KERN_ALERT "This is a onebyte device module\n");
	return 0;
}

static void onebyte_exit(void)
{
	// if the pointer is pointing to something
	if (onebyte_data) {
		// free the memory and assign the pointer to NULL
		kfree(onebyte_data);
		onebyte_data = NULL;
	}
	// unregister the device
	unregister_chrdev(MAJOR_NUMBER, DEVICE_NAME );
	printk(KERN_ALERT "Onebyte device module is unloaded\n");
}

MODULE_LICENSE("GPL");
module_init(onebyte_init);
module_exit(onebyte_exit);

