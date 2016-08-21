/*
 * signal练习2
 *
 * 这个程序是一个无限循环的程序
 * 通过别的进程发送kill信号结束
 *
 * */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
	printf("%d\n", (int)getpid());
	while( 1 ) {
		sleep(1);
		printf("Hello\n");
	}
	return 0;
}
