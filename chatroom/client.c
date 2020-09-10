#include"network.h"
#include"tools.h"

typedef struct sockaddr* SP;
int flag = FINE;

void sigint(int num)
{
	flag = Exit;
	printf("已退出\n");
	exit(0);
}

void* receive(void* arg)
{
	char read_str[4096];
	int cli_sock = *(int*)arg;
	while(1)
	{
		int ret = recv(cli_sock,read_str,sizeof(read_str),0);
		// 接收别人的聊天数据
		if(0>ret)
		{
			error_func("recv");
		}
		else if(0 == ret) continue;
		
		printf("%s\n",read_str);
			
		memset(read_str,0,sizeof(read_str));
			

	}
	
}

int main()
{
	// 创建socket
	int cli_sock = socket(AF_INET,SOCK_STREAM,0);
	if(0 > cli_sock)
	{
		error_func("socket");
	}
	signal(SIGINT,sigint);
	
	// 准备通信地址
	struct sockaddr_in cli_addr = {};
	cli_addr.sin_port = htons(8787);
	cli_addr.sin_family = AF_INET;
	cli_addr.sin_addr.s_addr = inet_addr("172.20.10.2");
	
	// 连接服务器
	if(0 > connect(cli_sock,(SP)&cli_addr,sizeof(cli_addr)))
	{
		error_func("connect");
	}
	
	char name[20];
	char write_str[4096];
	printf("\n请输入昵称>>>");
	scanf("%s",name);
	send(cli_sock,name,strlen(name)+1,0);
	
	// 接收客户端是否满的状态码
	char str[12] = {};
	recv(cli_sock,str,12,0);
	if(!strcmp(str,"17815517591"))
	{
		printf("客户端已满，请等待更新或充钱开启传输通道。。。。。\n");
		return FINE;
	}
	
	system("clear");
	printf("----------%s快乐冲浪时间----------\n",name);
	
	// 接收数据的线程
	pthread_t tid;
	pthread_create(&tid,NULL,receive,&cli_sock);
	
	int len = 0;
	
	for(;;)
	{
		/*char *str = "quit";
		if(Exit == flag)
		{
			send(cli_sock,str,strlen(str)+1,0);
		}*/
		
		// 输入聊天数据
		// printf(">>>");
		stdin_clear();
		gets(write_str);
		len = strlen(write_str);
		write_str[len] = 0;
		
		// 发送给服务器
		if(0>send(cli_sock,write_str,strlen(write_str)+1,0))
		{
			error_func("send");
		}
		
		if(Exit == flag || !strcmp(write_str,"quit"))
		{
			printf("已退出\n");
			return FINE;
		}
		memset(write_str,0,sizeof(write_str));
	}
	debug("%s\n",__func__);
}
