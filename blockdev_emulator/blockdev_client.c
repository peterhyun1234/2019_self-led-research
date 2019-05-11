/**************************************************************************************************
	name : blockdev_client.c
	func : read, write operation to block device.
	implementation logic : Accessing a block device to bring or transmit data by "Always on server"
	last edit : 2019.04.04
***************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <blkdev_common.h>
#include <time.h>

void * send_msg(void * arg);
void * recv_msg(void * arg);
void error_handling(char * msg);
	
char msg[BUF_SIZE];
	
int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread;
	void * thread_return;

	//버퍼 내부 설정
	int buffer[BLOCK_SIZE/4];
	int total = 0;
	float signature = 0.0;
	srand(time(NULL));

	while(i < BUF_SIZE/4)
	{
		buffer[i] = rand()%10;
		total += buffer[i];
		i++;
		if(i%100 == 0)
			signature += (float)total/10;


	}



	printf("%s\n\n", buffer);
	printf("size of buffer : %d\n", sizeof(buffer));
	printf("signature code of buffer : %f\n",signature);


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
	
	pthread_create(&snd_thread, NULL, send_msg, (void*)&sock, 'R', 1543, buffer);
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
	last edit : 2019.04.04
***************************************************************************************************/
void * send_msg(void * arg, int rw, unsigned int block_number, unsigned int *data)   // send thread main
{
	int sock=*((int*)arg);

	struct command cmd {
		.rw = rw,
		.block_number =block_number,
		.reclica[0] = 0,
		.reclica[1] = 5,
		.reclica[2] = 3,
	};
	size_t offset = 0;

	while(offset < sizeof(cmd)) {
		offset += send(sock, ((unsigned char *)&cmd) + offset, sizeof(cmd) - offset);
	}

	offset = 0;
	while(offset < BUF_SIZE)
	{
		offset += send(sock, data + offset, BUF_SIZE - offset);
	}
	return NULL;
}
	
	
/**************************************************************************************************
	name : recv_msg
	func : 서버로부터 명령을 받는다.
	implementation logic : Accessing a block device to bring or transmit data by "Always on server"
	last edit : 2019.04.04
***************************************************************************************************/
void * recv_msg(void * arg) 
{
	int sock=*((int*)arg);
	char received_msg[BLOCK_SIZE];
	int str_len;
	while(1)
	{
		str_len=read(sock, received_msg, BLOCK_SIZE-1);
		if(str_len==-1) 
			return (void*)-1;
		received_msg[str_len]=0;
		puts(received_msg);
	}
	return NULL;
}
	
void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
