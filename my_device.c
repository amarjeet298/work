#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/fs_struct.h>
#include <linux/uaccess.h>



#define MAJOR_NUMBER 261
#define DEVICE_NAME "amard"


/* forward declaration */
int onebyte_open(struct inode *inode, struct file *filep);
int onebyte_release(struct inode *inode, struct file *filep);
ssize_t onebyte_read(struct file *filep, char *buf, size_t count, loff_t *f_pos);
ssize_t onebyte_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos);

static void onebyte_exit(void);

/* definition of file_operation structure */
struct file_operations onebyte_fops = {
	.read =	onebyte_read,
	.write = onebyte_write,
	.open = onebyte_open,
	.release = onebyte_release
};

static char *onebyte_data = NULL;
static int BUFFER_SIZE = 0;

int onebyte_open(struct inode *inode, struct file *filep)
{
	return 0; // always successful
}

int onebyte_release(struct inode *inode, struct file *filep)
{
	return 0; // always successful
}

ssize_t onebyte_read(struct file *filep, char *buff, size_t length, loff_t *f_pos)
{
	
	char c;
	int bytes_read = 0;
	printk(KERN_INFO "Driver: read () %i \n", BUFFER_SIZE);

	if(f_pos == NULL ) return -EINVAL;
	
	if( BUFFER_SIZE ==1 ) {
		BUFFER_SIZE = 0;
		return 0;	
	}
  
	//Actually put the data into the buffer
	c = onebyte_data[0];
	put_user(c, buff);
	bytes_read++;
	//set the read data length to 0 to force the function to return.
	
	while((bytes_read < length)){
		bytes_read++;
	} 
	BUFFER_SIZE = 1;
	printk(KERN_ALERT "byte read is %d\n", bytes_read);

  
	return bytes_read;
	
}

ssize_t onebyte_write(struct file *filep, const char *buff, size_t len, loff_t *off)
{
	
	int i;
	//char byte;
	printk(KERN_INFO "Driver: write() %i \n", BUFFER_SIZE );
	printk(KERN_INFO "1. The value of *off is:%lli\n",*off);
	if (*off >= (BUFFER_SIZE - 1 || BUFFER_SIZE == 1)){
		BUFFER_SIZE = 0;
        	return 0;
    	}

	printk(KERN_INFO "2. The value of *off is:%lli\n",*off);

    	if ((*off + len) >  1 ){
		printk( "bash : printf : Write Error: No space left on device \n" );
		return 0;
        	len = 0;
		*off = 0;
		return len;
    	}

	printk(KERN_INFO "3. The value of *off is:%lli\n",*off);
	 if (copy_from_user(onebyte_data, buff , 1)){
                return -EFAULT;
        }
	//onebyte_data = &byte;

	for (i = 0; i < len; i++){
	      i++ ;
    	}
	//BUFFER_SIZE = 1;
	printk(KERN_INFO "4. The value of *off is:%lli\n",*off);
	//*off += len;
	//printk(KERN_INFO "5. The value of *off is:%lli\n",*off);

    return i;

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
	BUFFER_SIZE = 0 ;
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

