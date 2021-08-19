#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>

int main()
{
	int FileDesc;
	char Ubuff[80]=("This is user buffer ...sending data to kernel..");
	char Kbuff[100];
	FileDesc=open("/dev/MyCharDevice",O_RDWR,0777);
	if(FileDesc<0)
	{
		printf("\n error opening device\n");
		exit(1);
	}
	write(FileDesc,Ubuff,sizeof(Ubuff));
	read(FileDesc,Kbuff,50);
	printf("the data read from kernel is \n>>> %s <<<\n", Kbuff);
	close(FileDesc);
}
