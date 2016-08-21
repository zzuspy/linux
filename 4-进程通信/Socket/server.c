#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main() {
	//创建套接字
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	//服务器监听地址结构
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;//使用IPv4地址
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//接受所有IP地址的连接
	server_addr.sin_port = htons(8888);//绑定端口
	
	//客户端地址结构
	struct sockaddr_in client_addr;
	int client_len = sizeof(server_addr);
	

	//将套接字和IP,端口绑定
	int ret = bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(ret < 0) {
		perror("bind");
		exit(1);
	}

	//服务器监听
	ret = listen(server_fd, 5);
	if(ret < 0) {
		perror("listen");
		exit(1);
	}


	signal(SIGCHLD, SIG_IGN);
	
	//服务器程序主体
	while( 1 ) {
		printf("Server waiting ...\n");
		int client_fd;
		client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

		//下面是fork了一个进程单独进行操作，是多进程的，并具有并发性
		//如果注释掉的话就是单线程操作
		if( fork() == 0 ) {
			char buf[1024];
			char info[] = "haha";
			read(client_fd, buf, sizeof(buf));
			printf("get info from client : %s\n", buf);
			sleep(5);
			write(client_fd, info, sizeof(info));
			close(client_fd);
			exit(0);
		}
		

	}

	return 0;
}

