CC = gcc -std=c99
MSRC = /master_server/BRS_master_server.c
SSRC1 = /chunk_server1/BRS_chunk_server_1.c
SSRC2 = /chunk_server2/BRS_chunk_server_2.c
SSRC3 = /chunk_server3/BRS_chunk_server_3.c
SSRC4 = /chunk_server4/BRS_chunk_server_4.c
CSRC = blockdev_client.c

MSTARGET = MS 
STARGET1 = CS1
STARGET2 = CS2
STARGET3 = CS3
STARGET4 = CS4
CTARGET = C
PORTNUM = 9002
IP_ADD = 127.0.0.1

all:
	$(CC) -o $(CTARGET) $(CSRC) -pthread
	$(CC) -o $(MSTARGET) $(MSRC) -pthread
	$(CC) -o $(STARGET1) $(SSRC1) -pthread
	$(CC) -o $(STARGET2) $(SSRC2) -pthread
	$(CC) -o $(STARGET3) $(SSRC3) -pthread
	$(CC) -o $(STARGET4) $(SSRC4) -pthread

master:
	./$(MSTARGET) $(PORTNUM)

servers:
	./$(STARGET1) $(PORTNUM)
	./$(STARGET2) $(PORTNUM)
	./$(STARGET3) $(PORTNUM)
	./$(STARGET4) $(PORTNUM)

clients:
	./$(CTARGET) $(IP_ADD) $(PORTNUM)

test:
	./$(MSTARGET) $(PORTNUM)

	./$(STARGET1) $(PORTNUM) ./in/input.csv > myout1.txt
	./$(STARGET2) $(PORTNUM) ./in/input.csv > myout2.txt
	./$(STARGET3) $(PORTNUM) ./in/input.csv > myout3.txt
	./$(STARGET4) $(PORTNUM) ./in/input.csv > myout4.txt

	./$(CTARGET) $(IP_ADD) $(PORTNUM) ./in/client_input.csv > result.txt

	cmp myout1.txt ./out/sample1.txt
	cmp myout2.txt ./out/sample2.txt
	cmp myout3.txt ./out/sample3.txt
	cmp myout4.txt ./out/sample4.txt

	cmp my_result.txt ./out/result.txt


clean:
	rm $(CTARGET) $(STARGET)
