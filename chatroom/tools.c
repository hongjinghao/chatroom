#include"tools.h"


// 函数返回错误，进程结束
void error_func(const char* func)
{
	perror(func);
	exit(ERROR);
}

// 清空输入缓冲区
void stdin_clear(void)
{
	stdin->_IO_read_ptr = stdin->_IO_read_end;
}
