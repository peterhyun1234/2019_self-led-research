/*************************************************************************************************
	name : blockdev_client.c
	func : read, write operation to block device.
	implementation logic : Accessing a block device to bring or transmit data by "Always on server"
	last edit : 2019.05.13
***************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <fcntl.h>
#include "blkdev_common.h"

void * send_msg(void * arg);
void * recv_msg(void * arg);
void error_handling(char * msg);
	
char msg[BLOCK_SIZE];
	
int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread;
	void * thread_return;

	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	 }
	
	sock=socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	  
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("connect() error");
	
	pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
	pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
	pthread_join(snd_thread, &thread_return);
	pthread_join(rcv_thread, &thread_return);
	close(sock);  
	return 0;
}
	
	
	
	
/**************************************************************************************************
	name : send_msg
	func : 서버에게 명령을 보낸다.
	implementation logic : Accessing a block device to bring or transmit data by "Always on server"
	last edit : 2019.05.13
***************************************************************************************************/
void * send_msg(void * arg)   // send thread main
{
	int sock=*((int*)arg);

	//버퍼 내부 설정
	int send_fd;
	char buffer[BLOCK_SIZE];
	size_t offset = 0;
	if((send_fd = open("test.txt", O_RDONLY)) == -1)
		error_handling("read open error\n");

	while(offset < BLOCK_SIZE)
	{
		offset += read(send_fd, buffer + offset, BLOCK_SIZE - offset);
	}
	// cmd 설정
	struct command cmd;
	cmd.rw = 'W';
	cmd.block_number = 0;
	cmd.replica[0] = 0;
	cmd.replica[1] = 5;
	cmd.replica[2] = 3;
	
	offset = 0;

	while(offset < sizeof(cmd)) {
		offset += send(sock, ((unsigned char *)&cmd) + offset, sizeof(cmd) - offset, 0);	// default sending
	}

	while(offset < BLOCK_SIZE)
	{
		offset += send(sock, buffer + offset, BLOCK_SIZE - offset, 0);	//default sending
	}
	printf("DATA's offset = %d\n", offset);
	return NULL;
}
	
	
/**************************************************************************************************
	name : recv_msg
	func : 서버로부터 명령을 받는다.
	implementation logic : Accessing a block device to bring or transmit data by "Always on server"
	last edit : 2019.05.13
***************************************************************************************************/
void * recv_msg(void * arg) 
{
	int sock=*((int*)arg);
	int received_msg[BLOCK_SIZE];
	int str_len;
	int offset = 0;
	while(offset < BLOCK_SIZE)
	{
		offset += recv(sock, received_msg + offset, BLOCK_SIZE - offset, 0);	//default resv
	}
	for(str_len = 0; str_len > BLOCK_SIZE; str_len++)
		printf("%d",received_msg[str_len]);
	return NULL;
}
	
void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
