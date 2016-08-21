#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

#define FIFO "myfifo"

int main(int argc, char *argv[]) {
	/*
	if(argc != 2) {
		fprintf(stderr, "Usage : %s argv[1].\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	*/

	char buf[1024];
	//sscanf(argv[1], "%s", buf);

	//测试FIFO是否存在，若不存在，则创建一个FIFO
	if( access(FIFO, F_OK) == -1 ) {
		if( (mkfifo(FIFO, 0666) < 0) && (errno != EEXIST) ) {
			printf("Can NOT create fifo file!\n");
			exit(EXIT_FAILURE);
		}
	}

	//调用open以只写方式打开FIFO，返回文件描述符fd
	int fd;
	if( (fd = open(FIFO, O_WRONLY)) == -1 ) {
		perror("open");
		exit(1);
	}
	
	//调用write将buf写到文件描述符fd指向的FIFO中
	int real_write;
	while( 1 ) {
		printf("please input data you want : ");
		scanf("%s", buf);
		if( (real_write = write(fd, buf, sizeof(buf))) > 0 ) {
			printf("Write into pipe : '%s'.\n", buf);
		}
	}

	close(fd);

	return 0;
}
