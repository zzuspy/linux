#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main( void ) {
	pid_t childpid;
	int status;
	int retval;
	childpid = fork();
	if(childpid == -1) {
		perror("fork()");
		exit(1);
	}
	else if(childpid == 0){
		printf("运行子进程，子进程id=%d\n子进程结束\n\n", (int) getpid());
		exit(0);
	}
	else {
		usleep(1000);
		printf("运行父进程，父进程id=%d\n若子进程未结束则杀死子进程\n", (int) getpid());
		//这里的waitpid,如果子进程结束了，则返回子进程ID，否则返回0
		if( (waitpid( childpid, &status, WNOHANG) ) == 0 ) {
			retval = kill(childpid, SIGKILL);
			if(retval) {
				puts("kill failed.");
				perror("kill");
				waitpid(childpid, &status, 0);
			}
			else {
				printf("子进程未结束，杀死子进程 %d\n", (int) childpid);
			}
		}
	}
	return 0;
}
