#define BLOCK_SIZE 4096
#define EPOLL_SIZE 50	//epoll 객체로 관리할 수 있는 fd의 개수
#define MAX_CLNT 100	//모니터링 할 fd의 수 지정
#define file_name	"Block_storage.txt"

struct command {
	char rw;
	unsigned int block_number;
	unsigned int replica[8];
};
