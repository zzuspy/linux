#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct _msg_buf {
	long type;
	char buf[100];
} msg_buf;

int main() {
	int key, qid;
	msg_buf buf;
	key = ftok(".", 10);
	qid = msgget(key, IPC_CREAT | 0666);
	printf("key: %d\nqid: %d\n", key, qid);
	while ( 1 ) {
		if( msgrcv(qid, (void *)&buf, 100, 0, 0) < 0 ) {
			perror("msgrcv");
			exit(-1);
		}
		printf("type:%ld\nget:%s\n", buf.type, buf.buf);
	}
	return 0;
}

