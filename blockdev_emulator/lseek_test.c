#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


struct requested_work {
		char operator[2];
		int blocknum;
		char data[4096];
};



void parsing_cmd(char *cmd, struct requested_work *req);
void store_data(int block_number,char *data);
void bring_data(int block_number);
void rmvfirst(char *buf);

#define file_name "wdwd.txt"


int main()
{
	char *buf = "w, 235, dlsfjwdlamvlwdkflwoekgskkdkwflawfklskldsnsdlksdjlskfawlnfkdslnvdslkfjwlkfjwelkvsjdasdmlwkflkwevmlksdvnlfmkawlfqmklegnlbkndlvskflwkfmwleknvlkdnslfkmnlfk";

	struct requested_work rq;
	int a = 0;
	parsing_cmd(buf, &rq);
	
	printf("파싱 잘됐는지 확인 ㄱㄱ \n");
	printf("%s\n%d\n%s\n", rq.operator, rq.blocknum, rq.data);
	//store_data(rq.blocknum,rq.data);

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
		printf("%s.\n",ptr);
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



void bring_data(int block_number)
{
	//data를 읽어오고 있습니다. 
	int fd;
	
	if((fd = open(file_name, O_RDONLY)) == -1)
	{
		printf("%s open error\n", file_name);	
		exit(1);
	}
		
	printf("fd = %d\n",fd);
	/*read_len = read(fd, requested_data.data, strlen(requested_work.data));
	if(read_len == -1)
	{
		printf("read error in bring_data\n");
	}*/
}




void store_data(int block_number,char *data)
{}



void rmvfirst(char *buf)
{
	int i = 0;
	for(i=1; buf[i]; i++)
		buf[i-1] = buf[i];

	buf[i-1] = '\0';
}
