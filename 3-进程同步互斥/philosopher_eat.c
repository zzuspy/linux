/*
 * 哲学家进餐问题
 *
 * 使用多线程模拟多个哲学家使用叉子进餐的情景
 *
 * */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <semaphore.h>

sem_t p_num;
sem_t _fork[100];

int people_num;

void eat() {
	usleep(50000);
}

void *philosopher_p(void *arg) {
	int id = *(int *) arg;
	sem_wait( &p_num );
	sem_wait( &_fork[id] );
	sem_wait( &_fork[(id + 1) % people_num]);
	printf("第%d个哲学家使用%d，%d号叉子吃面\n\n", id + 1, id + 1, (id + 1) % people_num + 1);
	eat();
	sem_post( &_fork[(id + 1) % people_num] );
	sem_post( &_fork[id] );
	sem_post( &p_num );
}

void init(int n) {
	sem_init( &p_num, 0, n - 1);
	int i;
	for(i = 0; i < n; i ++) {
		sem_init( &_fork[i], 0, 1);
	}
}

void destroy(int n) {
	sem_destroy( &p_num );
	int i = 0;
	for(i = 0; i < n; i ++) {
		sem_destroy( &_fork[i] );	
	}
}

int main() {
	printf("Input the number of philosopher : ");
	scanf("%d", &people_num);
	init( people_num );

	int *people = (int *) malloc(people_num * sizeof(int));
	int i;
	pthread_t pid;
	for(i = 0; i < people_num; i ++) {
		people[i] = i;
		pid = i;
		int ret = pthread_create( &pid, NULL, philosopher_p, (void *)&people[i] );
	}

	sleep(1);

	destroy( people_num );
}
