#define BLOCK_SIZE 1024

struct command {
	int rw;
	unsigned int block_number;
	unsigned int replica[8];
}
