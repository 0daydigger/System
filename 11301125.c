#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>	/* O_ACCMODE */
#include <linux/seq_file.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>	/* copy_*_user */
#include <linux/random.h> 	/* get_random_bytes */
/* for current variable */
#include <linux/sched.h> 
#include <asm/current.h>
#include "memdev.h"
/*
	系统级编程期末大作业
	YuXun Lu - 11301125
*/
MODULE_AUTHOR("Yuxun Lu");
MODULE_LICENSE("GPL");

/* 计数器 */
int open_time[4] = {0}; //booga0,booga1,booga2,booga3
int output_time[4] = {0}; //booga,googoo,wooga,neka
int read_bytes = 0;
int write_bytes = 0;

static mem_major = MEMDEV_MAJOR;

module_param(mem_major, int, S_IRUGO);

struct mem_dev *mem_devp; /*设备结构体指针*/

struct cdev cdev; 

// Proc入口点
#define procfs_name "booga" //the proc/driver/booga
struct proc_dir_entry *Our_Proc_File;

//proc 读函数
int procfile_read(char *buffer,char **buffer_location,off_t offset, int buffer_length, int *eof, void *data)
{
int ret;

printk(KERN_INFO "procfile_read (/proc/%s) called\n", procfs_name);

/*
* We give all of our information in one go, so if the
* user asks us if we have more information the
* answer should always be no.
*
* This is important because the standard read
* function from the library would continue to issue
* the read system call until the kernel replies
* that it has no more information, or until its
* buffer is filled.
*/
if (offset > 0) {
/* we have finished to read, return 0 */
ret = 0;
} else {
/* fill the buffer, return the buffer size */
ret = sprintf(buffer, " bytes read = %d\n \
bytes written = %d\n \
number of opens:\n \
  /dev/booga0 = %d times\n \
  /dev/booga1 = %d times\n \
  /dev/booga2 = %d times\n \
  /dev/booga3 = %d times\n \
strings output:\n \
  booga!booga! = %d times\n \
  googoo!gaga! = %d times\n \
  wooga!wooga! = %d times\n \
  neka!maka! = %d times\n",read_bytes,write_bytes,
					  open_time[0],open_time[1],open_time[2],open_time[3],
					  output_time[0],output_time[1],output_time[2],output_time[3]);
}

return ret;
}
/* 随机数生成函数，返回值为0,1,2,3之中的一个 */
char random()
{
	char result = 0;
	get_random_bytes(&result,1);
	result = result & 0x7F;
	return (result % 4);
}
/*文件打开函数*/
int booga_open(struct inode *inode, struct file *filp)
{
    struct mem_dev *dev;
    
    /*获取次设备号*/
    int num = MINOR(inode->i_rdev);

    if (num >= MEMDEV_NR_DEVS) 
            return -ENODEV;
    dev = &mem_devp[num];
    open_time[num]++;
    /*将设备描述结构指针赋值给文件私有数据指针*/
    filp->private_data = dev;
    
    return 0; 
}

/*文件释放函数*/
int booga_release(struct inode *inode, struct file *filp)
{
  return 0;
}

/*读函数*/
static ssize_t booga_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos)
{
  unsigned long p =  *ppos;        /*记录文件指针偏移位置*/  
  unsigned int count = size;    /*记录需要读取的字节数*/ 
  int ret = 0;    /*返回值*/  
  struct mem_dev *dev = filp->private_data; /*获得设备结构体指针*/
  unsigned int k = 0; //循环变量
  char *return_char; //返回给用户的值
  /*判断读位置是否有效*/
  if (p >= MEMDEV_SIZE)    /*要读取的偏移大于设备的内存空间*/  
    return 0;
  if (count > MEMDEV_SIZE - p)     /*要读取的字节大于设备的内存空间*/ 
    count = MEMDEV_SIZE - p;

  /* 输出一下次设备号看是否成功打开次设备*/
  printk("READ:The minor number is %d\n",dev->minor_number);
  printk("READ: the block asked for is %d\n",size);
  /* 为返回结果分配内存 */
  return_char = kmalloc(count,GFP_KERNEL);
  /* 内存清零 */
  memset(return_char,0,sizeof(return_char));
  //一个字符就是一个字节，所以这里可以这么用，开始复制结果字符串
  printk("The asked bytes is %d\n",count);
  switch( random() )
  {
	/* 注意！这里由于strlcat是安全的字符串操作函数因此，应该，大概，可能，似乎，不会出现缓冲区溢出问题
	   【但是没有参考文献表明了这一点！】因此可能会带来潜在的安全问题，在下一版本中应该fix
		by yuxun lu
	*/
    case 0:
	output_time[0]++;
	for(k = 0; k < count; k++)
		strlcat(return_char, "booga!booga!", count);
	break;
    case 1:
	output_time[1]++;
	for(k = 0; k < count; k++)
		strlcat(return_char, "googoo!gaagaa!", count);
	break;
    case 2:
	output_time[2]++;
	for(k = 0; k < count; k++)
		strlcat(return_char, "neka!maka!", count);
	break;
    case 3:
	output_time[3]++;
	for(k = 0; k < count; k++)
		strlcat(return_char, "wooga!wooga!", count);
	break;
  }
  /*读数据到用户空间:内核空间->用户空间交换数据*/
  /* copy_to_user(用户指针,内核指针,字节数量) */
  printk("The return char is %s and the size is %d\n",return_char,count);
  if (copy_to_user(buf, (void*)return_char, count) )
  { 
    //读取失败
    ret =  - EFAULT;
  }
  else
  {
     //偏移量不改动 *ppos += count;
     ret = count;
     read_bytes += count;
     printk(KERN_INFO "read %d bytes(s) from %d\n", count, p);
  }
  //分配之后要记得释放
  kfree(return_char);
  return ret;
}

/*写函数*/
static ssize_t booga_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos)
{
  unsigned long p =  *ppos;
  unsigned int count = size;
  struct mem_dev *dev = filp->private_data; /*获得设备结构体指针*/
  /*成功打开次设备号*/
  printk("WRITE:the minor number is %d\n",dev->minor_number);
  switch( dev->minor_number )
  {
     case 3:
	//TODO:杀死正在操作自己的进程
	//force_sig(SIGTERM, int pid);
	//STEP1:获取pid
	force_sig(SIGTERM,current);
	break;
     default:
	write_bytes += count;
	break;
  }
  return count;
}

/* seek文件定位函数 */
static loff_t booga_llseek(struct file *filp, loff_t offset, int whence)
{ 
    loff_t newpos;      

    switch(whence) {
      case 0: /* SEEK_SET */       /*相对文件开始位置偏移*/ 
        newpos = offset;           /*更新文件指针位置*/
        break;

      case 1: /* SEEK_CUR */
        newpos = filp->f_pos + offset;    
        break;

      case 2: /* SEEK_END */
        newpos = MEMDEV_SIZE -1 + offset;
        break;

      default: /* can't happen */
        return -EINVAL;
    }
    if ((newpos<0) || (newpos>MEMDEV_SIZE))
        return -EINVAL;
        
    filp->f_pos = newpos;
    return newpos;

}

/*文件操作结构体*/
static const struct file_operations mem_fops =
{
  .owner = THIS_MODULE,
  .llseek = booga_llseek,
  .read = booga_read,
  .write = booga_write,
  .open = booga_open,
  .release = booga_release,
};
/*设备驱动模块加载函数*/
static int booga_init(void)
{
  int result;
  int i;

  dev_t devno = MKDEV(mem_major, 0);

   /* 申请设备号，当xxx_major不为0时，表示静态指定；当为0时，表示动态申请*/ 
  /* 静态申请设备号 */
  if (mem_major) //定义在memdev.h中，MEMDEV_MAJOR为251,MEMDEV_NR_DEVS为设备数量，四个，设备名称"chardev"
    result = register_chrdev_region(devno, MEMDEV_NR_DEVS, "booga");
  else  /* 动态分配设备号 */
  {
    result = alloc_chrdev_region(&devno, 0, MEMDEV_NR_DEVS, "booga");
    mem_major = MAJOR(devno);    /*获得申请的主设备号*/
  }  
  
  if (result < 0)
    return result;

 /*初始化cdev结构，并传递file_operations结构指针*/ 
  cdev_init(&cdev, &mem_fops);    
  cdev.owner = THIS_MODULE;    /*指定所属模块*/
  cdev.ops = &mem_fops;
  
  /* 注册字符设备,设备数量为4个，MEMDEV_NR_DEVS指定 */
  cdev_add(&cdev, MKDEV(mem_major, 0), MEMDEV_NR_DEVS);
   
  /* 为设备描述结构分配内存*/
  mem_devp = kmalloc(MEMDEV_NR_DEVS * sizeof(struct mem_dev), GFP_KERNEL);
  if (!mem_devp)    /*申请失败*/
  {
    result =  - ENOMEM;
    goto fail_malloc;
  }
  memset(mem_devp, 0, sizeof(struct mem_dev));
  
  /*为设备分配内存*/
  for (i=0; i < MEMDEV_NR_DEVS; i++) 
  {
        mem_devp[i].size = MEMDEV_SIZE;
        mem_devp[i].data = kmalloc(MEMDEV_SIZE, GFP_KERNEL);
        memset(mem_devp[i].data, 0, MEMDEV_SIZE);
	mem_devp[i].minor_number = i; //指定设备号
  }
    
  /* 注册/proc/driver/ */
  Our_Proc_File = create_proc_entry(procfs_name, 0644, NULL);

  if (Our_Proc_File == NULL) {
  remove_proc_entry(procfs_name, NULL);
  printk(KERN_ALERT "Error: Could not initialize /proc/%s\n",
  procfs_name);
  return -ENOMEM;
  }

  Our_Proc_File->read_proc = procfile_read;
  //Our_Proc_File->owner = THIS_MODULE;
  Our_Proc_File->mode = S_IFREG | S_IRUGO;
  Our_Proc_File->uid = 0;
  Our_Proc_File->gid = 0;
  Our_Proc_File->size = 37;
  return 0;

  fail_malloc: 
  unregister_chrdev_region(devno, 1);
  
  return result;
}

/*模块卸载函数*/
static void booga_exit(void)
{ 
  remove_proc_entry(procfs_name, NULL); 
  cdev_del(&cdev);   /*注销设备*/
  kfree(mem_devp);     /*释放设备结构体内存*/
  unregister_chrdev_region(MKDEV(mem_major, 0), MEMDEV_NR_DEVS); /*释放设备号*/

}


module_init(booga_init);
module_exit(booga_exit);
