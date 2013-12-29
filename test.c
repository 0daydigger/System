#include <stdio.h>
#include <string.h>
#include <fcntl.h>
int main()
{
    int hDevice = 0;
    char buffer[70] = {0};
    char deviceName[50] = {0};
    int k = 0;
    for( k = 0 ; k < 4; k++)
    {
	//清空buffer
	memset(buffer,0,sizeof(buffer));
	//打印设备名称
	sprintf(deviceName,"/dev/booga%d",k);
	//打开设备
	hDevice = open(deviceName,O_RDWR);
	if ( hDevice == -1)
	{
	    printf("Fail to open deivce %s\n",deviceName);//打开失败
	    return 0;
	}
	printf(" Buffer is %s and size is %d\n",buffer,sizeof(buffer));
	read(hDevice,buffer,sizeof(buffer));
	printf("After READ buffer is %s\n",buffer);
    }
   /* try to write */
    for( k = 0 ; k < 4 ; k++)
    {
	//清空buffer
	memset(buffer,0,sizeof(buffer));
	//打印设备名称
	sprintf(deviceName,"/dev/booga%d",k);
	//打开设备
	hDevice = open(deviceName,O_RDWR);
	printf("Now write to %s\n",deviceName);
	write(hDevice,buffer,sizeof(buffer));
    }
    return 0;    

}
