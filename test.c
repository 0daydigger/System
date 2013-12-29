#include <stdio.h>
#include <string.h>
#include <fcntl.h>
int main()
{
    int hDevice = 0;
    char buffer[50] = {0};
    char deviceName[50] = {0};
    int k = 0;
    for( k = 0 ; k < 4; k++)
    {
	//清空buffer
	memset(buffer,0,sizeof(buffer));
	//打印设备名称
	sprintf(deviceName,"/dev/chardev%d",k);
	//打开设备
	hDevice = open(deviceName,O_RDWR);
	if ( hDevice == -1)
	{
	    printf("Fail to open deivce %s\n",deviceName);//打开失败
	}
	printf(" Buffer is %s and size is %d\n",buffer,sizeof(buffer));
	read(hDevice,buffer,50);
	printf("After READ buffer is %s\n",buffer);
    }
    return 0;    

}
