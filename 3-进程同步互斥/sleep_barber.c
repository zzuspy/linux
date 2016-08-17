/*
 * 睡眠理发师问题
 * 
 * 利用多线程程序模拟生活中的一个例子
 *
 * 理发师线程是一个死循环（假设理发师能一直工作）
 * 顾客线程则是一次性的，有位置就留下，没有就离开
 * 然后用队列queue来维护店内顾客状态（按照先来后到规则）
 *
 * 编译运行后，输入2 30可以发现中间的一段输出，队列长度基本是满的，开头是呈上升趋势，结尾是呈下降趋势，当然这和我设置的sleep时间有关。
 * 
 *
 * */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <semaphore.h>

#define CHAIRS 10

//用queue模拟店里的顾客
int Queue[CHAIRS];
int front, rear;
int waiting;

//信号量
sem_t barber;
sem_t customer;
sem_t mutex;

//统计
int cuthair_cnt;
int notcuthair_cnt;

void cut_hair() {
	usleep(1000);
}

void get_haircut() {
	//todo
}

void print_queue() {
	int i, cnt = 0;
	printf("店内顾客状态（按照进店时间排队等待的顾客） : ");
	for(i = rear; cnt < waiting; i = (i + 1) % CHAIRS, cnt ++) {
		printf("%d ", Queue[i] + 1);
	}
	printf("\n\n");
}

void* barber_p(void *arg) {
	int id = *(int *) arg;
	while( 1 ) {
		sem_wait( &customer );
		sem_wait( &mutex );
		waiting --;
		int tmp = Queue[rear];
		rear = (rear + 1) % CHAIRS;
		sem_post( &barber );
		cuthair_cnt ++;
		printf("理发师%d正在给顾客%d理发\n", id + 1, tmp + 1);
		print_queue();
		sem_post( &mutex );
		cut_hair();
	}
}

void* customer_p(void *arg) {
	int id = *(int *) arg;
	sem_wait( &mutex );
	if(waiting < CHAIRS) {
		waiting ++;
		printf("顾客%d进店\n", id + 1);
		Queue[front] = id;
		front = (front + 1) % CHAIRS;
		print_queue();
		sem_post( &customer );
		sem_post( &mutex );
		sem_wait( &barber );
		get_haircut();
	}
	else {
		notcuthair_cnt ++;
		printf("理发店坐满了，顾客%d离开理发店\n\n", id + 1);
		sem_post( &mutex );
	}
}

void init(int n) {
	sem_init( &mutex, 0, 1);
	sem_init( &barber, 0, n);
	sem_init( &customer, 0, 0);
	waiting = 0;
	front = rear = 0;
	cuthair_cnt = notcuthair_cnt = 0;
}

void destroy() {
	sem_destroy( &mutex );
	sem_destroy( &barber );
	sem_destroy( &customer );
}

int main() {
	pthread_t b_id, c_id;
	
	int i, barberNum, customerNum;
	printf("Input the number of barber and customer : ");
	scanf("%d %d", &barberNum, &customerNum);
	init(barberNum);

	int *bar = (int *) malloc(barberNum * sizeof(int));
	int *cus = (int *) malloc(customerNum * sizeof(int));

	for(i = 0; i < barberNum; i ++) {
		b_id = i;
		bar[i] = i;
		int ret = pthread_create(&b_id, NULL, barber_p, (void *)&bar[i]);
		if(ret != 0) {
			perror("pthread create");
			exit(1);
		}
	}
	for(i = 0; i < customerNum; i ++) {
		c_id = i;
		cus[i] = i;
		usleep(200);
		int ret = pthread_create(&c_id, NULL, customer_p, (void *)&cus[i]);
		if(ret != 0) {
			perror("pthread create");
			exit(1);
		}
	}

	sleep(2);
	printf("总共有%d人进店理发，有%d人进店后因为爆满而离开！\n", cuthair_cnt, notcuthair_cnt);

	destroy();
	return 0;
}


