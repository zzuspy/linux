#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

#define FIFO "myfifo"

int main() {
	char buf[1024];
	int fd;
	int real_read;
	
	if(access(FIFO, F_OK) == -1) {
		if( (mkfifo(FIFO, 0666) < 0) && (errno != EEXIST) ) {
			printf("Can NOT create fifo file!\n");
			exit(1);
		}
	}

	if( (fd = open(FIFO, O_RDONLY) ) == -1 ) {
		printf("Open fifo error!\n");
		exit(1);
	}
	
	while( 1 ) {
		printf("Input bytes you want read : ");
		int len;
		scanf("%d", &len);
		memset(buf, 0, sizeof(buf));
		if( ( real_read = read(fd, buf, len) ) > 0  ) {
			printf("Read from pipe : '%s'.\n", buf);
		}
	}

	close(fd);

	return 0;
}
