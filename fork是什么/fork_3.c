#include <stdio.h>  
#include <unistd.h>

/*
 * 这里总共产生了20个进程：
 * 2 * (4 + 4 + 2) = 20
 * 这里主要是考你对&& || 操作符的熟悉程度
 * */

int main(int argc, char* argv[]) {  
   fork();  
   fork() && fork() || fork();  
   fork();  
   printf("+\n");
   sleep(1);
}  
