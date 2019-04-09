#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>


struct requested_work {
		char operator[2];
		int blocknum;
		char data[4096];
};



void parsing_cmd(char *cmd, struct requested_work *req);
void store_data(int block_number,char *data);
void bring_data(struct requested_work *req);
void rmvfirst(char *buf);

#define file_name "wdwd.txt"
#define block_size 512

int main()
{
	char *wbuf = "w, 235, dlsfjwdlamvlwdkflwoekgskkdkwflawfklskldsnsdlksdjlskfawlnfkdslnvdslkfjwlkfjwelkvsjdasdmlwkflkwevmlksdvnlfmkawlfqmklegnlbkndlvskflwkfmwleknvlkdnslfkmnlfk";
	
	char *rbuf = "r, 235";

	struct requested_work rq;
	struct requested_work rq2;

	int a = 0;
	parsing_cmd(wbuf, &rq);
	
	printf("<Writing part>\n");
	printf("command : %s\n", wbuf);
	printf("After command parsing\n");
	printf("Operator : %s\nBlock number : %d\nData to send : %s\n", rq.operator, rq.blocknum, rq.data);
	store_data(rq.blocknum,rq.data);
	printf("\nStoring complete\n\n");
	printf("------------------------------------------\n\n<Reading Part>\n");
	printf("command :%s", rbuf);
	parsing_cmd(rbuf, &rq2);
	printf("After command parsing\n");
	printf("Operator : %s\nBlock number : %d\nData to be received : %s\n", rq2.operator, rq2.blocknum, rq2.data);
	
	bring_data(&rq2);


	printf("\nBring complete\n\n");
	printf("Block [%d] : %s\n",rq2.blocknum, rq2.data);
	
	return 0;
}






void parsing_cmd(char *cmd, struct requested_work *req)
{
	int N = 0;
	char *s1 = malloc(sizeof(char) *strlen(cmd));

	strcpy(s1, cmd);

	char* ptr = strtok(s1, ",");

	printf("\n\n");
	while(ptr != NULL)
	{
		if((N == 0)&&((strncmp(ptr,"r",1)==0)||(strncmp(ptr,"w",1)==0))) //operator parsing
			strcpy(req->operator, ptr);
		else if(N == 1)	//Parsing block number
		{
			req->blocknum = atoi(ptr);
		}
		else
		{
			rmvfirst(ptr);
			strcpy(req->data, ptr);
		}

		ptr = strtok(NULL, ",");
		N++;

	}
	printf("\n\n");
}



void bring_data(struct requested_work *req)
{
	//data를 읽어오고 있습니다. 
	int fd;
	int read_len;

	if((fd = open(file_name, O_RDONLY)) == -1)
	{
		printf("%s open error\n", file_name);	
		exit(1);
	}
		
	read_len = pread(fd,req->data, block_size, block_size*req->blocknum);
	if(read_len == -1)
	{
		printf("read error in bring_data\n");
	}

}




void store_data(int block_number,char *data)
{
	int fd;
	int size;
	
	if((fd = open(file_name, O_RDWR | O_CREAT, 0644)) == -1){
		printf("%s open error\n", file_name);
		exit(1);
	}

	if((size = pwrite(fd, data, strlen(data),block_size* block_number)) == -1){
		printf("pwrite error\n");
		close(fd);
		exit(1);
	}

	close(fd);
}



void rmvfirst(char *buf)
{
	int i = 0;
	for(i=1; buf[i]; i++)
		buf[i-1] = buf[i];

	buf[i-1] = '\0';
}
