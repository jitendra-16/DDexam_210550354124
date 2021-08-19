#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/wait.h>
#include<linux/semaphore.h>
#include<linux/rwsem.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JITENDRA")

#define NAME MyCharDevice
dev_t Mydev1,Mydev2;

int flag=1;
struct semaphore sem;
struct rw_semaphore *rw_sem;

void  sema_init(sem); //sema initialisation
void down(sem); //sema lock
void up(sem); //sema unlock


void init_sem(struct rw_sem);
void down_read(struct rw_sem );// sema read lock
void down_write(struct rw_sem); //sema write lock
void up_read(struct rw_sem);//sema read unlock
void up_write(struct rw_sem); //sema write unlock


char Kbuff [50]="I am from kernel";
char Ubuff[50];

// fun prototypes

int NAME_open(struct inode *inode,struct file *filp);
int NAME_release(struct inode *inode, struct file *filp);
ssize_t NAME_write(struct file *filp, const char __user *Ubuff,size_t count ,loff_t *offp);
ssize_t NAME_read(struct file *filp,char __user *Ubuff,size_t count, loff_t *offp);


//structure that defines the operation that driver provide

struct file_operations fops=
{
	.owner = THIS_MODULE,
	.open = NAME_open,
	.read = NAME_read,
	.write = NAME_write,
	.release= NAME_release,
	// .flush= NAME_flush,
	
};


// structure for a character driver
struct cdev1 *my_cdev1;
struct cdev2 *my_cdev2;

//init module
static int __init  CharDevice_init(void)
{
	int result1, result2;

	int MAJOR1,MINOR1 ,MAJOR2,MINOR2;
	

	Mydev1 = MKDEV(255,0);	// CREATE A DEVICE NO.
	Mydev2 =MKDEV(42,0);

	MAJOR1=MAJOR1(Mydev1);
	MINOR1=MINOR2(Mydev1);

	MAJOR2=MAJOR(Mydev2);
	MINOR2=MINOR(Mydev2);

	printk("\n Major no. is %d...Minor no. is %d\n",MAJOR1,MINOR1);
	printk("\n Major no. is %d...Minor no. is %d\n",MAJOR2,MINOR2);

	printk("\n Inside the Char device\n");

	result1 = register_chrdev_region(Mydev1, 1, "MyCharDevice");
	result2=register_chrdev_region(Mydev2,1, "MyCharDevice"); //register device region
	if(result<0)
	{
		printk(KERN_ALERT "\nThe regin is not obtanied\n");
		return(-1);
	}

	my_cdev1 = cdev_alloc();	// allocate memory to char structuire
	my_cdev2 = cdev_alloc();

	my_cdev1->ops = &fops;
	my_cdev2->ops = &fops; // link our file operation to char device

	result1=cdev1_add(my_cdev,Mydev1,1); //notify kernel abt the new device
	result2=cdev2_add(my_cdev,Mydev2,1);

	if(result1<0)
	{
             printk(KERN_ALERT "\n The char device not created...\n");
	     unregister_chrdev_region(Mydev1,1);
	     return (-1);
	}

	if(result2<0)
	{
		printk(KERN_ALERT "\n The char device not created..\n");
		unregister_chrdev_region(Mydev2,1);
	return 0;
}

//clean module
void __exit  CharDevice_exit(void)
{

	int MAJOR1,MINOR1,MAJOR2,MINOR2;

	Mydev1=MKDEV(255,0);
	Mydev2=MKDEV(42,0);

	MAJOR1=MAJOR1(Mydev1);
	MINOR1=MINOR1(Mydev1);

	MAJOR2=MAJOR2(Mydev2);
	MINOR2=MINOR2(Mydev2);

//	printk("\n The Major no is %d ...The Minor no. is %d\n",MAJOR,MINOR);
	unregister_chrdev_region(Mydev1,1); //unregister the device no and the device is created
	unregister_chrdev_region(Mydev2,1);

	cdev_del(my_cdev1);
        cdev_del(my_cdev2);

	printk(KERN_ALERT "\n I have unregister the stuff that was allocated... Good bye for ever...\n");
	 
}

//Open system call

int NAME_open(struct inode *inode,struct file *filp)
{
	printk(KERN_ALERT "\n this kernel open call.. i have nothing to do...but u have all\n");
	return 0;
}



/*sszint NAME_read(ssize_t)*/

ssize_t NAME_read(struct file *filp, char __user *Ubuff, size_t count ,loff_t *offp)
{
	unsigned long int result;
	ssize_t retval;
       printk("\n Inside read ...system call ");

//	printk("\n In read ...after wait...data available");
	result=copy_to_user((char *)Ubuff,(char *) Kbuff,count);
	if(result!=0)
	{
		printk(KERN_ALERT "\n data not copied\n");
	        retval=-EFAULT;
                return retval;
	}

	else
	{
	 flag=1;
	 printk("\n Reading data\n");
	 retval=count;
	 return retval;
	 }
}

// write system call

 ssize_t NAME_write(struct file *filp, const char __user *Ubuff, size_t count , loff_t *offp)
{
    unsigned long result;
    ssize_t retval;
    
     
    printk("\n In write...shortly to write the data");
    result=copy_from_user((char *)Kbuff,(char *)Ubuff,count);
    if(result!=0)
    {
	    flag=0;
	    printk(KERN_ALERT "\n insde write sys call have problem");
	    retval=-EFAULT;
	    return retval;
    }

    else
    {
	    flag=1;
	    printk(KERN_ALERT"\n msg from user   %s \n",Kbuff);
	    retval=count ;
	    return retval ;
    }

    

}
//close system call

int NAME_release(struct inode *inode,struct file *filp)
{
	printk(KERN_ALERT"\n This is release method of my character driver ... bye \n");
	return 0;
}

