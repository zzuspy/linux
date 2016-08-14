#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* thread_1 ( void* n ) {
	int c = 10;
	while( c -- ) {
		printf("%d : This is thread_1 !\n", c);
		sleep(1);
	}
	return NULL;
}

void* thread_2 ( void* n ) {
	int c = 10;
	while( c -- ) {
		printf("%d : This is thread_2 !\n", c);
		sleep(1);
	}
	return NULL;
}

void* thread_3 ( void* n ) {
	int c = 10;
	while( c -- ) {
		printf("%d : This is thread_3 !\n", c);
		sleep(1);
	}
	return NULL;
}

int main() {
	int c = 10;
	pthread_t thread_id1, thread_id2, thread_id3;
	pthread_create ( &thread_id1, NULL, &thread_1, NULL);
	pthread_create ( &thread_id2, NULL, &thread_2, NULL);
	pthread_create ( &thread_id3, NULL, &thread_3, NULL);
	while(c --) {
		printf("%d : This is main_thread !\n", c);
		sleep(1);
	}
	return 0;
}
