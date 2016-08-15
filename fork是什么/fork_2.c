#include <stdio.h>
#include <unistd.h>

/*
 * 这组代码会输出2 + 2 * 2 = 6组值
 * 总共会产生1 * 2 * 2 = 4个进程
 * */

int main() {
	pid_t fpid;
	int i;
	printf("%2s%10s%10s%10s%10s\n", "i", "son/pa", "ppid", "pid", "fpid");
	
	sleep(1);
	for( i = 0; i < 2; i ++) {
		fpid = fork();
		if(fpid == 0) 
			printf("%2d%10s%10d%10d%10d\n", i, "child", (int)getppid(), (int)getpid(), fpid);
		else 
			printf("%2d%10s%10d%10d%10d\n", i, "parent", (int)getppid(), (int)getpid(), fpid);
	}
	sleep(1);
}
