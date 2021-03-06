#pragma once
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<signal.h>
#include<pthread.h>

#define ERROR -1
#define FINE 1
#define Exit 2

#ifndef DDEBUG
#define debug(format,...) {}
#else
#define debug(format,...) fprintf(stdout,format,__VA_ARGS__)
#endif


// 函数返回错误，进程结束
void error_func(const char* func);

// 清空输入缓冲区
void stdin_clear(void);
