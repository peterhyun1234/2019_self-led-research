#define BLOCK_SIZE 1024

struct command {
	char rw;
	unsigned int block_number;
	unsigned int replica[8];
};
