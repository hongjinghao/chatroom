#pragma once

// 最大在线人数
#define MAX_NP 50

// 50个客户端的sockfd
char fd_sock[MAX_NP];


// 服务器发送数据
int svr_send(int fd,char* buf);

// 客户端退出
