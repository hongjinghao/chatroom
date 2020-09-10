#include"network.h"
#include"tools.h"

typedef struct sockaddr* SP;

void sigint(int num)
{
	printf("服务器已关闭\n");
	for(int i=0; i<MAX_NP; i++)
	{
		close(fd_sock[i]);
	}
	exit(0);
}

void *func(void* arg)
{
	int cli_fd = *(int*)arg;
	char buf[4096] = {};
	char snd[4096] = {};
	int index = 0;
	for(int i=0; i<MAX_NP; i++)
	{
		if(cli_fd == fd_sock[i])
		{
			index = i;
			break;
		}
	}
	
	// 客户端未满的代码
	char *str = "17815517590";
	send(cli_fd,str,strlen(str)+1,0);
	// 第一次进入获取昵称
	char name[20];
	if(0>recv(cli_fd,name,sizeof(name),0))
	{
		error_func("recv");
	}
	printf("name from %d name:%s,bits:%d\n",cli_fd,name,strlen(name));
	
	// 进入房间的信息发送给其他客户端
	sprintf(buf,"--欢迎%s进入房间",name);
	svr_send(cli_fd,buf);
	/*
	for(int i=0; i<MAX_NP; i++)
	{
		if(fd_sock[i] != 0 && fd_sock[i]!=cli_fd)
		{	
			send(fd_sock[i],buf,strlen(buf)+1,0);
			printf("data send to。 sock:%d data:%s bits:%d\n",fd_sock[i],buf,strlen(buf)+1);
		}
	}*/
	memset(buf,0,sizeof(buf));
	
	
	// 收发数据
	while(1)
	{
		// 接受数据
		int ret = recv(cli_fd,buf,sizeof(buf),0);
		if(ret <= 0)
		{
			sprintf(buf,"--%s退出本房间",name);
			svr_send(cli_fd,buf);

			fd_sock[index] = 0;
			return NULL;
		}
		printf("data recv form %d buf:%s,bits:%d\n",cli_fd,buf,ret);
		
		// 收到quit退出客户端
		if(strstr(buf,"quit"))
		{
			sprintf(buf,"--%s退出本房间",name);
			svr_send(cli_fd,buf);

			fd_sock[index] = 0;
			return NULL;
		}
		
		// 发送数据
		sprintf(snd,"<<<%s:%s",name,buf);
		svr_send(cli_fd,snd);

		memset(buf,0,sizeof(buf));
		memset(buf,0,sizeof(snd));
	}
}

int main()
{
	signal(SIGINT,sigint);
	// 准备socket
	int svr_sock = socket(AF_INET,SOCK_STREAM,0);
	if(0 > svr_sock)
	{

		error_func("socket");
	}

	// 准备通信地址
	struct sockaddr_in svr_addr = {};
	svr_addr.sin_port = htons(8787);
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = INADDR_ANY;
	socklen_t addrlen = sizeof(svr_addr);

	// 绑定
	if(0 > bind(svr_sock,(SP)&svr_addr,addrlen))
	{
		error_func("bind");
	}

	// 监听
	if(0 > listen(svr_sock,MAX_NP))
	{
		error_func("listen");
	}
	
	for(;;)
	{
		int flag = 0;
		int cli_fd = accept(svr_sock,(SP)&svr_addr,&addrlen);
		for(int i=0; i<MAX_NP; i++)
		{
			if(0 == fd_sock[i])
			{
				fd_sock[i] = cli_fd;
				pthread_t pid;
				pthread_create(&pid,NULL,func,&cli_fd);
				flag = 1;
				break;
			}
		}
		
		// 客户端满
		if(0 == flag)
		{
			// 客户端满的代码
			char* str = "17815517591";
			send(cli_fd,str,strlen(str)+1,0);
		}
	}
}
