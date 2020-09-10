#include"network.h"
#include"tools.h"



// 服务器发送数据
int svr_send(int fd,char* buf)
{
	int flag = ERROR;
	if(!strcmp(buf,"quit")) return Exit;
	for(int i=0; i<MAX_NP; i++)
	{
		if(fd_sock[i] != 0 && fd_sock[i]!=fd)
		{	
			send(fd_sock[i],buf,strlen(buf)+1,0);
			printf("data send to。 sock:%d data:%s bits:%d\n",fd_sock[i],buf,strlen(buf)+1);
			flag = FINE;
		}
	}
	return flag;
}

