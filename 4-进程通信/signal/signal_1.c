#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_signal(int sig) {
	printf("\nhandle_signal: I got signal %d\n", sig);
	//恢复终端终端信号SIGINT的默认行为
	(void) signal(SIGINT, SIG_DFL);
}

int main() {
	//改变终端中断信号SIGINT的默认行为，使之执行handle_signal函数，而不是终止程序执行
	(void) signal(SIGINT, handle_signal);
	while( 1 ) {
		printf("Hello World!\n");
		sleep(1);
	}
	return 0;
}
