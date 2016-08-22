#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>

#define BUFSIZE 10
#define sem_name "mysem"

int main() {
	struct People {
		char name[10];
		int age;
	};
	int shmid;
	sem_t *sem;
	int age = 10, i = 1;
	char buff[BUFSIZE];
	key_t shmkey;
	shmkey = ftok("read.c", 0);

	//创建共享内存和信号量的IPC
	sem = sem_open(sem_name, O_CREAT, 0644, 1);
	if(sem == SEM_FAILED) {
		printf("unable to create semaphore!");
		sem_unlink(sem_name);
		exit(-1);
	}
		
	
	shmid = shmget(shmkey, 1024, 0666 | IPC_CREAT);
	if(shmid == -1) {
		printf("create shm is fail\n");
	}

	//将共享内存映射到当前进程的地址中，之后直接对进程中的地址addr的操作就是对共享内存的操作
	struct People * addr;
	addr = (struct People*) shmat(shmid, 0, 0);
	if(addr == (struct People*) -1) {
		printf("shm shmat is fail\n");
	}
	//向共享内存中写入数据
	addr->age = 0;
	do {
		sleep(1);
		sem_wait(sem);
		memset(buff, 0, BUFSIZE);
		memset( (addr+i)->name, 0, BUFSIZE );
		printf("写进程：输入一些姓名（不超过10个字符）到共享内存（输入'quit'退出）:\n");
		if(fgets(buff, BUFSIZE, stdin) == NULL) {
			perror("fgets");
			sem_post(sem);
			break;
		}
		strncpy( (addr+i)->name, buff, strlen(buff) - 1 );
		(addr+i)->age = ++age;
		addr->age ++;
		i ++;
		sem_post(sem);
	} while( strncmp(buff, "quit", 4) != 0 );

	//将共享内存与当前内存断开
	if(shmdt(addr) == -1) printf("shmdt is fail\n");

	sem_close(sem);
	sem_unlink(sem_name);

	return 0;
}
