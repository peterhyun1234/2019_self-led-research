#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void parsing_cmd(char *cmd, struct requested_work);
void store_data(int block_number,char *data);
void bring_data(int block_number);

#define file_name "wdwd.txt"


int main()
{
	char *buf = "w, 235, dlsfjwdlamvlwdkflwoekgskkdkwflawfklskldsnsdlksdjlskfawlnfkdslnvdslkfjwlkfjwelkvsjdasdmlwkflkwevmlksdvnlfmkawlfqmklegnlbkndlvskflwkfmwleknvlkdnslfkmnlfk";

	typedef struct requested_work {
		char operator;
		int blocknum;
		char* data;
	}


	parsing_cmd(buf, requested_work);
	store_data(requested_work.blocknum,requested_work.data);

}


void parsing_cmd(char *cmd, struct requested_work)
{
	int N = 0;
	char *s1 = malloc(sizeof(char) *strlen(cmd));

	strcpy(s1, cmd);

	char* ptr = strtok(s1, ",");

	while(ptr != NULL)
	{
		if((N == 0)&&((strncmp(ptr,"r",1)==0)||(strncmp(ptr,"w",1)==0)))
			strcpy(requested_work.operator, ptr);
		else if(N == 1)
			requested_work.blocknum = atoi(ptr);
		else
		{
			rmvfirst(ptr);
			strcpy(requested_work.data, ptr);
		}

		ptr = strtok(NULL, ",");
		N++;

	}
}



void bring_data(struct requested_work)
{
	//data를 읽어오고 있습니다. 
	int fd;
	
	if((fd = open(_path, O_RDONLY)) == -1)
	{
		printf("%s open error\n", _path);	
		exit(1);
	}
		
	printf("fd = %d\n",fd);
	//read_len = read(fd, requested_data.data, strlen(requested_work.data));
	if(read_len == -1)
	{
		printf("read error in bring_data\n");
	}
}




void store_data(int block_number,char *data)
{}


