/*
 * 生产者与消费者问题
 *
 * 这里是用多线程模拟的生产者以及消费者
 * 使用信号量实现的进程（线程）的同步互斥
 *
 * */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_BUFSIZE 10

//生产者的生产量，消费者的消费量
const int product_num = 10;
const int consumer_num = 10;

//信号量
sem_t mutex;
sem_t empty;
sem_t full;

//共享缓冲区
int buf[MAX_BUFSIZE];
int cur_size;

//打印缓冲区情况
void print_buf() {
	int i;
	for(i = 0; i < cur_size; i ++) {
		printf("%d ", buf[i]);
	}
	printf("\n\n");
}

//统计实际生产和消费的数量
int pro_cnt = 0;
int con_cnt = 0;

void* product(void *arg) {
	int id = *(int*) arg;
	int t = product_num;
	while(t --) {
		sem_wait(&empty);
		sem_wait(&mutex);
		if(cur_size < MAX_BUFSIZE) {
			
			int tmp = rand() % 9 + 1;
			buf[cur_size ++] = tmp;
			printf("生产者%d生产%d\n缓冲区情况", id, tmp);
			print_buf();
			pro_cnt ++;
		}
		else printf("缓冲区已满，生产失败\n\n");//注释掉sem_wait(&empty)和sem_post(&full)可以看到效果
		sem_post(&mutex);
		sem_post(&full);
		//usleep(1000);
	}
}

void* consumer(void* arg) {
	int id = *(int*)arg;
	int t = consumer_num;
	while(t --) {
		sem_wait(&full);
		sem_wait(&mutex);
		if(cur_size > 0) {
			printf("消费者%d消费%d\n缓冲区情况", id, buf[cur_size - 1]);
			cur_size --;
			print_buf();
			con_cnt ++;
		}
		else printf("没有产品！\n\n");//注释掉sem_wait(&full)和sem_post(&empty)可以看到效果
		sem_post(&mutex);
		sem_post(&empty);
		usleep(1000);
	}
}

void init() {
	sem_init(&mutex, 0, 1);
	sem_init(&empty, 0, MAX_BUFSIZE);
	sem_init(&full, 0, 0);
	cur_size = 0;
}

void destroy() {
	sem_destroy( &mutex );
	sem_destroy( &empty );
	sem_destroy( &full );
}

int main() {
	init();
	
	pthread_t pro_pid, con_pid;

	int i, proNum, conNum;
	printf("Input the num of product and consumer : ");
	scanf("%d %d", &proNum, &conNum);
	int *pro = (int *)malloc(proNum * sizeof(int));
	int *con = (int *)malloc(conNum * sizeof(int));


	for(i = 0; i < proNum; i ++) {
		pro_pid = (pthread_t) i;
		pro[i] = i;
		int ret = pthread_create(&pro_pid, NULL, product, (void *)&pro[i]);
		if(ret != 0) {
			perror("pthread create");
			exit(1);
		}
	}
	for(i = 0; i < conNum; i ++) {
		con_pid = (pthread_t) i;
		con[i] = i;
		int ret = pthread_create(&con_pid, NULL, consumer, (void *)&con[i]);
		if(ret != 0) {
			perror("pthread create");
			exit(1);
		}
	}

	sleep(2);

	printf("\n总共生产%d,总共消费%d\n\n", pro_cnt, con_cnt);
	destroy();

	return 0;
}
