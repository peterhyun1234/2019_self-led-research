#define BLOCK_SIZE 4096

struct command {
	int rw;
	unsigned int block_number;
	unsigned int replica[8];
}
