#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include "blkdev_common.h"


void error_handling(char* buf);
void setNonBlockingMod(int fd);

static int clntNumber[MAX_CLNT];
static int clntCnt=0;	

int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t adr_sz;

	int i, j;
	int storage_fd;
	size_t offset = 0;
	struct command cmd;
	char DATA[BLOCK_SIZE];
	char rcv_Buf[BLOCK_SIZE];

	// eppoll 사용할 때 필요한 변수선언
	struct epoll_event* ep_events;
	struct epoll_event event;
	int epfd, event_cnt;

	if(argc!=2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");

	if(listen(serv_sock, 5)==-1)
		error_handling("bind() error");

	// epoll 객체 생성
	epfd = epoll_create(EPOLL_SIZE);
	ep_events=malloc(sizeof(struct epoll_event)*EPOLL_SIZE);


	event.events = EPOLLIN;	// 읽기동작을 관찰하고 싶을 때
	event.data.fd = serv_sock;

	epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);	// epoll 객체에 접속 소켓을 등록

	while(1)
	{
		event_cnt=epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);	// "읽기 가능"한  소켓 디스크립터가 나타날 때 까지  모니터링!
		if(event_cnt==-1)
		{
			if(errno==EINTR) continue;	// 인터럽트인 경우는 계속 진행
			puts("epoll_wait() error");
			break;
		}


		// 여기서부터 "읽기 가능" 한 소켓 디스크립터에 대한 추가 작업 진행
		for(i=0; i<event_cnt; i++)
		{




			if(ep_events[i].data.fd==serv_sock)	// accept a client : 듣기소켓이 "읽기 가능" 한 경우에 전송 소켓 생성 
			{
				// < 에지 트리거 모드를 사용하기 위한 기본적인 설정 >
				adr_sz = sizeof(clnt_adr);
				clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
				setNonBlockingMod(clnt_sock); // 소켓을 논 블로킹 소켓으로!
				event.events = EPOLLIN | EPOLLET; // 에지 트리거 모드로  읽기동작을 관찰
				event.data.fd = clnt_sock; //accept된 소켓을 data의 fd에 지정
				epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event); //epoll 객체에 듣기소켓을 등록
				clntNumber[clntCnt++] = clnt_sock; //연결된 fd를 클라이언트 number에 지정.(누가 들어오고 누가 나가는 지 알리기 위해)
			}




			else	// IO : 전송 소켓이 "읽기 가능" 한 경우 입출력 작업 진행
			{


				while(1) // 루프를 돌면서 read를 충분히 호출한다.
				{
					while (offset < sizeof(cmd))
					{
						offset += recv(ep_events[i].data.fd, ((unsigned char *)&cmd) + offset, sizeof(cmd) - offset, 0);
					}


					if(cmd.rw == 'R')
					{

						if((storage_fd = open(file_name, O_RDONLY)) == -1)
						{							
							error_handling("Open error occur in recv_message");
						}	

						lseek(storage_fd, cmd.block_number * BLOCK_SIZE, SEEK_SET);
						offset = 0;

						while (offset < BLOCK_SIZE)
						{
							offset += read(storage_fd, rcv_Buf + offset, BLOCK_SIZE - offset);		
						}

						//client로 보내주자!
						offset = 0;
						while (offset < BLOCK_SIZE)
						{
							offset += send(ep_events[i].data.fd, rcv_Buf + offset, BLOCK_SIZE - offset, 0);
						}

						break;
					}
					else if(cmd.rw == 'W'){
						offset = 0;

						while(offset < BLOCK_SIZE)
						{
							offset += recv(ep_events[i].data.fd, DATA + offset, BLOCK_SIZE - offset, 0);
						}


						if((storage_fd = open(file_name, O_RDWR | O_CREAT, 0644)) == -1){
							error_handling("Open error occur in store func");
						}
						lseek(storage_fd, cmd.block_number * BLOCK_SIZE, SEEK_SET);
						offset = 0;
						while(offset < BLOCK_SIZE)
						{
							offset += write(storage_fd, DATA + offset, BLOCK_SIZE - offset);
						}
						//additional point : replication 
						break;
					}
					close(storage_fd);

					if(offset == 0) 	// client가 연결을 끊을 때
					{
						epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);	// 다 읽은 fd를 epoll 객체의 관심 리스트에서 제거
						close(ep_events[i].data.fd);

						for(j=0; j<clntCnt; j++)	//

						{
							if(clntNumber[j]==ep_events[i].data.fd)	// 지우면서  지정된 관심 리스트 수정
							{
								memcpy(&clntNumber[j], &clntNumber[j+1], clntCnt-(j+1)); 	// 소켓간 바이트 복사
								clntCnt--;
								break;
							}
						}

						break;
					}
					else if(offset<0)
					{
						if(errno == EAGAIN)
							break;
					}
				}
			}

		}
	}
	close(serv_sock);
	close(epfd);
	return 0;
}





void error_handling(char *buf)
{
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
} 




// 소켓을 논 블로킹 모드로 동작
// - read함수를 쓰면 수신 버퍼에 남은 데이터가 없으면 블로킹이 되기 때문에 read 함수를 여러번 호출할 경우 이 함수 사용 ㄱㄱ 
// - read함수는 수신 버퍼에 남아있는 데이터가 없으면 에러코드를 반환한다.
void setNonBlockingMod(int fd)
{
	int flag = fcntl(fd, F_GETFL, 0); // 기존 파일 디스크립터 플래그 추출
	fcntl(fd, F_SETFL, flag | O_NONBLOCK); // 기존 플래그에서 논블록 추가 + 플래그 지정
}


