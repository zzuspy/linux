#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, void* argv[]) {
	
	if(argc != 2) {
		printf("please input : client infomation(send to server)\n");
		exit(0);
	}

	//创建套接字
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	//服务器套接字地址
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");//服务器地址
	address.sin_port = htons(8888);//服务器端口
	int address_len = sizeof(address);
	
	//和服务器进行连接
	int ret = connect(sockfd, (struct sockaddr*)&address, address_len);
	if(ret < 0) {
		perror("connect");
		exit(1);
	}

	//和服务器进行通信
	char buf[1024];
	printf("client send data to server : %s\n", (char *)argv[1] );
	write(sockfd, (char *)argv[1], sizeof(buf));
	read(sockfd, buf, sizeof(buf));
	printf("Get data from server : %s\n", buf);

	//关闭套接字
	close(sockfd);

	return 0;
}

