#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * fork调用的一个奇妙之处就是它仅仅被调用一次，却能够返回两次，它可能有三种不同的返回值：
 *  1）在父进程中，fork返回新创建子进程的进程ID；
 *  2）在子进程中，fork返回0；
 *  3）如果出现错误，fork返回一个负值；
 * */

int main() {
	pid_t fpid;
	int cnt = 0;
	fpid = fork();
	if (fpid < 0) {
		perror("fork");
		exit(1);
	}
	else if(fpid == 0) {
		printf("I am the child process, my process id is %d\n",  (int)getpid());
		cnt = 2;
	}
	else {
		printf("I am the parent process, my process id is %d\n", (int)getpid());
		cnt ++;
	}
	printf("cnt = %d\n", cnt);
	return 0;
}
