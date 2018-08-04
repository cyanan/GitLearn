#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#define DEN_BUF 1024

/*1.子进程通过无名管道通信方式发送数据，父进程接收数据*/
int main(){
	int pipe_fd[2];                              //定义一个数组作为管道创建函数的参数，注意此参数只有两个元素，第一个元素作为管道
	                                             //读的文件描述符，第二个作为管道写的文件描述符，参数值是多少不追究，顺序要对应！
	int ret = pipe(pipe_fd);                     //在父进程中创建一个无名管道，返回值赋值给变量ret
	pid_t pid;                                   //定义一个pid_t类型的变量pid
	char buf[DEN_BUF];                           //定义一个缓冲区buf用于读写数据存放缓冲区

	if(ret < 0){                                 //如果返回值是-1说明管道创建失败
		perror("pipe error\n");
		return -1;
	}

	pid = fork();                                //在父进程中新建一个子进程，返回值赋值给变量pid

	if(pid < 0){                                 //如果返回值小于0，说明子进程创建失败
		perror("fork error\n");
	}
	else if(pid == 0){                           //返回值0为子进程
		close(pipe_fd[0]);                       //关闭子进程读的管道端口
		printf("children is sending msg \n");
		printf("请输入子进程发送的数据：\n");
		memset(buf,0,DEN_BUF);                   //将缓冲区清0
		fgets(buf,DEN_BUF,stdin);                //输入要发送的数据（保存至数组buf中）
		write(pipe_fd[1],buf,strlen(buf));       //将输入数组buf中的strlen(buf)长度的数据写入管道中
	}
	else{                                        //返回值大于0为父进程
		sleep(3);                                //因为父进程执行速度较快，所以先让父进程睡眠3S
		close(pipe_fd[1]);                       //关闭父进程写的管道端口
		printf("father has recevied msg: \n");
		memset(buf,0,DEN_BUF);
		read(pipe_fd[0],buf,DEN_BUF);            //将子进程写入管道的数据读出DEN_BUF长度的字符放置数组buf中
		printf("%s", buf);                       //打印出父进程接收到的字符串数据
	}
	

}

/*2.父进程通过无名管道通信方式发送数据，子进程接收数据*/
/*
int main(){

	pid_t pid;
	int pipe_fd[2];
	int ret = pipe(pipe_fd);

	char buf[DEN_BUF];

	if(ret < 0){
		perror("pipe error\n");
		return -1;
	
	}
	pid = fork();
	if(pid < 0){
		perror("fork error\n");
	}
	else if(pid == 0){
	//	sleep(1);
		close(pipe_fd[1]);
		memset(buf,0,DEN_BUF);
		read(pipe_fd[0],buf,DEN_BUF);
		printf("children has recived msg from father:\n");
		printf("%s\n", buf);
	}
	else{

		printf("father is sending  msg: \n");
		printf("请输入父进程要发送的数据： \n");

		close(pipe_fd[0]);
		memset(buf,0,DEN_BUF);
		fgets(buf,DEN_BUF,stdin);
		write(pipe_fd[1],buf,strlen(buf));

		//printf("father is sending msg\n");
	
	}
	


}
*/

















