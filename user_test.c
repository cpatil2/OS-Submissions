#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#include <sys/time.h>

#define size 2               // size decides the number of times current time is fetched from kernel

struct timeval gtodTimes[50];

static char *msg1[50];
static char *msg2[50];

int main(){
   
	int ret, fd1;

	for(int i=0;i<size;i++)
	{
    		msg1[i] = (char *)malloc((30)*sizeof(char));
        }

    		for(int i=0;i<size;i++)
    		{
         		msg2[i] = (char *)malloc((30)*sizeof(char));
    		}

   		fd1 = open("/dev/driver_test", O_RDWR);             // fd1 is file descriptor for the opened device with read/write access
   
   		//checking erron in opening the device
   		if (fd1 < 0){
      			perror("\nSorry! Failed to open the device...");
      			return errno;
   		}

   		else
  	 	{
   			printf("\nReading from the device!\n");

   			for(int  i=0; i < size; i++)
  			{
        			int q=gettimeofday(&gtodTimes[i], 0);

        			ret = read(fd1, msg1[i], 80);        // Read the response from the device

        			if(ret < 0)
        			{
              				perror("\nSorry! Failed to read the message from the device.");
              				return errno;
        			}
   			}

   
                        //separating current kernel times in two parts msg1 ang msg2
        		for(int  i=0; i < size; i++)
        		{
                		int zz,q;
                		for(zz=0;zz<50;zz++)
                		{
                			if(msg1[i][zz]=='X')
                			{
                        			msg1[i][zz]='\0';
                        			break;
                			}
                		}
                		zz++;
                
				for(q=0;msg1[i][zz]!='\0';q++)
                		{
                        		msg2[i][q]=msg1[i][zz];
                        		zz++;
                		}
                		msg2[i][q]='\0';
        		}

                        //finally printing the times with different ways  	
	 		for(int  k=0; k < size; k++)
   			{
                                printf("\n\n***********     Printing Timings       ***************");
        			printf("\n\nmessage from module: Current_kernel_time() : [%s]\n\n", msg1[k]);
        			printf("message from module: getnstimeofday() : [%s]\n\n", msg2[k]);
		 		printf("Test application(user level) time: gettimeofday() : %ld %ld\n\n",gtodTimes[k].tv_sec,gtodTimes[k].tv_usec);
                                printf("\n");
   			}
   		}	
   		
		close(fd1);
   		return 0;
}

