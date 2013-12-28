#include <stdio.h>
#include <string.h>
int main()
{
    int i = 0;
    FILE *fp0 = NULL;
    char Buf[4096];
    
    /*初始化Buf*/
    strcpy(Buf,"Mem is char dev!");
    printf("BUF: %s\n",Buf);
    
    /*打开设备文件*/ 
    for(i = 0 ; i < 4 ; i++)
{
    char devName[60] = {0};
	/* 输出一下子设备号 */
    sprintf(devName,"/dev/chardev%d",i);
    printf("Current device minor number is %s\n",devName);

    fp0 = fopen(devName,"r+");
    if (fp0 == NULL)
    {
        printf("Open Chardev0 Error!\n");
        return -1;
    }
    
    /*写入设备*/
    fwrite(Buf, sizeof(Buf), 1, fp0);
    
    /*重新定位文件位置（思考没有该指令，会有何后果)*/
    fseek(fp0,0,SEEK_SET);
    
    /*清除Buf*/
    strcpy(Buf,"Buf is NULL!");
    printf("BUF: %s\n",Buf);
    
    
    /*读出设备*/
    fread(Buf, sizeof(Buf), 1, fp0);
    
    /*检测结果*/
    printf("BUF: %s\n",Buf);
}
    return 0;    

}
