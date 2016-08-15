#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int num = 200, cnt = 100;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* thread_1(void* arg) {
	int i, tmp;
	for(i = 0; i < cnt; i ++) {
		//pthread_mutex_lock(&mutex);
		tmp = num - 1;
		usleep(30000);
		num = tmp;
		printf("1 --> %d\n", num);
		//pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

void* thread_2(void* arg) {
	int i, tmp;
	for(i = 0; i < cnt; i ++) {
		//pthread_mutex_lock(&mutex);
		tmp = num - 1;
		usleep(20000);
		num = tmp;
		printf("2 --> %d\n", num);
		//pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int main() {
	pthread_t thread1, thread2;

	int err = pthread_create(&thread1, NULL, &thread_1, NULL);
	if(err != 0) {
		perror("create thread");
		exit(-1);
	}
	err = pthread_create(&thread2, NULL, &thread_2, NULL);
	if(err != 0) {
		perror("create thread");
		exit(-1);
	}
	
	void *tret;
	err = pthread_join(thread1, &tret);
	if(err != 0) {
		perror("1 finish");
		exit(-1);
	}
	err = pthread_join(thread2, &tret);
	if(err != 0) {
		perror("2 finish");
		exit(-1);
	}

	printf("%d\n", num);

	return 0;
}
