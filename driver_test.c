#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<asm/uaccess.h>
#include<linux/time.h>

static int counter=0;

int demo_open(struct inode *pinode,struct file *pfile)
{
        counter++;
        printk(KERN_ALERT "Number of time device open %d", counter);

        return 0;
}

ssize_t demo_read(struct file *pfile,char  *buffer,size_t length,loff_t *offset)
{

         int error_count = 0;
         int len=0,i,rem,z,w;
         long x,num,y;
         char str[256];

         struct timespec ts,xx;

         getnstimeofday(&ts);

         xx=current_kernel_time();


        printk(KERN_ALERT "getnstime : %ld and %ld",ts.tv_sec,ts.tv_nsec);

        {
            x=xx.tv_sec;
            num=x;

            while (num!=0)
            {
                len++;
                num /= 10;
            }
            for (i = 0; i < len; i++)
            {
                rem = x % 10;
                x = x / 10;
                str[len - (i + 1)] = rem + '0';
            }
        }
        str[len]=' ';

        y=len;y++;len=0;
        {
            x=xx.tv_nsec;
            num=x;

            while (num!=0)
            {
                len++;
                num /= 10;
            }
            for (i = 0; i < len; i++)
            {
                rem = x % 10;
                x = x / 10;
                str[(y+len) - (i + 1)] = rem + '0';
            }
        }

        str[len+y]='X';
        z=len+y;z++;len=0;

        {
            x=ts.tv_sec;
            num=x;

            while (num!=0)
            {
                len++;
                num /= 10;
            }
            for (i = 0; i < len; i++)
            {
                rem = x % 10;
                x = x / 10;
                str[(z+len) - (i + 1)] = rem + '0';
            }
        }
        str[z+len]=' ';
        w=z+len;w++;len=0;
    {
            x=ts.tv_nsec;
            num=x;

            while (num!=0)
            {
                len++;
                num /= 10;
            }
            for (i = 0; i < len; i++)
            {
                rem = x % 10;
                x = x / 10;
                str[(w+len) - (i + 1)] = rem + '0';
            }
        }
        str[w+len]='\0';



                printk(KERN_ALERT "Inside %s\n",__FUNCTION__);

                error_count=copy_to_user(buffer, str ,sizeof(str));


                if (error_count==0)
                {
                          printk(KERN_INFO "EBBChar: %ld characters are sent to the user\n", sizeof(str));
                          return 0;
                }
                else
                {
                      printk(KERN_INFO "EBBChar: Failed to send  %d characters to the user\n", error_count);
                      return -EFAULT;
                 }

        return 0;
}

int demo_close(struct inode *pinode,struct file *pfile)
{
        printk(KERN_ALERT "Closing the Device");

        return 0;
}

struct file_operations mydevice_operations = {
.owner = THIS_MODULE,
.open = demo_open,
.read = demo_read,
.release = demo_close,
};

int __init mydevice_init(void)
{

        register_chrdev(240,"simple char drv",&mydevice_operations);

        return 0;
}

void __exit mydevice_exit(void)
{
        unregister_chrdev(240,"simple char drv");
}

module_init(mydevice_init);
module_exit(mydevice_exit);

