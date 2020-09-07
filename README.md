# 2019_self-led-research
Title: Optimization of communication layer for distributed block storage
------------------------------------
## 1. Research background
![bottleneck](https://user-images.githubusercontent.com/46476398/69224648-4788de80-0bc0-11ea-9fbc-1183c16f46ee.PNG)
### 1.1 Distriduted block system
    - 다른 기계의 스토리지를 원격으로 접근해서 사용
    - 로컬 스토리지의 공간의 한계를 극복
    - 원격 접근이기 때문에 네트워크 전송이 중요함
    - 데이터 복제 저장을 통한 결함 감래 시스템
    - 복제할 때 병목현상을 방지하기 위한 릴레이 전송  
### 1.2. Research goal
    - 블록을 복제할 때 소프트웨어적 기법들로 속도 증진
    - 과도한 네트워크 트래픽을 줄여 병목현상 방지
---------------------------------------- 
## 3. System structure
### 3.1. block_replicating_system (Replication from server to server)
![server_structure](https://user-images.githubusercontent.com/46476398/69224657-4a83cf00-0bc0-11ea-88ac-ecb6e29da5f0.PNG)
* Master server (in "/block_replicating_system/master_server") 
* Chunk servers (in "/block_replicating_system/chunk_server1-4") 

### 3.2. Blockdev_emulator (emulator for test)
```c
if(ep_events[i].data.fd==serv_sock)	// accept a client : 듣기소켓이 "읽기 가능" 한 경우에 전송 소켓 생성 
{
	// < 에지 트리거 모드를 사용하기 위한 기본적인 설정 >
	adr_sz = sizeof(clnt_adr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
	setNonBlockingMod(clnt_sock); // 논 블로킹 소켓
	event.events = EPOLLIN | EPOLLET; // 에지 트리거 모드로  읽기동작을 관찰
	event.data.fd = clnt_sock; //accept된 소켓을 data의 fd에 지정
	epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event); //epoll 객체에 듣기소켓을 등록
	clntNumber[clntCnt++] = clnt_sock; // 누가 들어오고 누가 나가는 지 알리기 위해
}
```
> emulator server (in "/blockdev_emulator)

```c
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
```
> emulator client (in "/blockdev_emulator)
----------------------------------------    
## 4. How to process
``` bash
### Make exec files
    $ make all

### Start emulating sytem
    $ make master
    $ make servers
    $ make clients

### Test
    $ make test

### Delete object file: 
    $ make clean
```
> 자세한건 "/blockdev_emulator/Makefile" 참조
----------------------------------------
## 5. I felt that
    * 처음해보는 연구를 통해서 조금이지만 SW연구 과정을 알 수 있었음
    * 기존의 연구 결과에 비해서 좋은 결과를 내진 못했음. 개인적으로 스레드를 활용한 concurrency에 우선순위를 뒀어야했음 
    * SW 연구에 작은 부분이지만 참여할 수 있음에 자부심을 느낌
    * 다음부터는 확실하게 개발 절차를 파악하고 개발 계획을 구체적으로 세워야한다고 생각함
----------------------------------------
## 6. contact
    e-mail: peterhyun1234@gmail.com
