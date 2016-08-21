/*
 * signal练习2
 *
 * 这个程序通过发送kill信号给对应的进程（通过pid）
 * 使之结束
 *
 * */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, void * argv[]) {
	if(argc != 2) {
		printf("please input : ./program arg1\n");
		exit(1);
	}
	int pid = atoi(argv[1]);
	printf("kill %d...\n", pid);
	int ret = kill(pid, SIGKILL);
	if(ret != 0) {
		perror("kill");
		exit(1);
	}
	else {
		printf("kill success!\n");
	}
	return 0;
}


