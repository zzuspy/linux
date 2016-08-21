/*
 * 读者与写者问题
 *
 * */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <semaphore.h>
#include <stdlib.h>

sem_t writeblock;
sem_t mutex;
sem_t read_print;

int reader_cnt;

void do_read(int id) {
	FILE *fp = fopen("./reader_writer.txt", "r");
	if(fp == NULL) {
		perror("fopen");
		exit(1);
	}

	sem_wait( &read_print );
	printf("读者 %d 读取文件内容为：\n", id + 1);
	char buf[1000];
	while( fgets(buf, 1000, fp) != NULL ) {
		printf("%s\n", buf);
	}
	printf("\n");
	sem_post( &read_print );

	fclose(fp);
}

void do_write(int id) {

	char buf[1000];
	printf("写者 %d 正在写入信息...\n\n", id + 1);
	sprintf(buf, "writer %d say :\" Hello World!\"!", id + 1);

	FILE *fp = fopen("./reader_writer.txt", "w");
	if(fp == NULL) {
		perror("writer_fopen");
		exit(1);
	}
	fputs(buf, fp);
	fclose(fp);
	
	usleep(rand() % 1000);
}

void* reader_p(void *arg) {
	int id = *(int *) arg;
	
	usleep(rand() % 1000);
	sem_wait( &mutex );
	reader_cnt ++;
	if(reader_cnt == 1) sem_wait( &writeblock );
	sem_post( &mutex );
	do_read( id );
	sem_wait( &mutex );
	reader_cnt --;
	if(reader_cnt == 0) sem_post( &writeblock );
	sem_post( &mutex );
	usleep(rand() % 1000);
}

void* writer_p(void *arg) {
	int id = *(int *) arg;
	sem_wait( &writeblock );
	do_write( id );
	sem_post( &writeblock );
}

void init() {
	sem_init( &writeblock, 0, 1);
	sem_init( &mutex, 0, 1);
	sem_init( &read_print, 0, 1);
	reader_cnt = 0;
}

void destroy() {
	sem_destroy( &writeblock );
	sem_destroy( &mutex );
	sem_destroy( &read_print );
}

int main() {
	init();

	int i, reader_num, writer_num;
	printf("Input the number of reader and writer : ");
	scanf("%d %d", &reader_num, &writer_num);

	int *reader = (int *) malloc( reader_num * sizeof(int) );
	int *writer = (int *) malloc( writer_num * sizeof(int) );
	
	//读者
	for(i = 0; i < reader_num; i ++) {
		pthread_t rid = i;
		reader[i] = i;
		int ret = pthread_create( &rid, NULL, reader_p, (void *)&reader[i]);
		if(ret < 0) {
			perror("thread create");
			exit(1);
		}
	}
	//写者
	for(i = 0; i < writer_num; i ++) {
		pthread_t wid = i;
		writer[i] = i;
		int ret = pthread_create( &wid, NULL, writer_p, (void *)&writer[i]);
		if(ret < 0) {
			perror("thread create");
			exit(1);
		}
	}

	sleep(1);
	destroy();
	return 0;
}

