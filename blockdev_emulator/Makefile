CC = gcc -std=c99
SSRC = blockdev_server.c
CSRC = blockdev_client.c
STARGET = S 
CTARGET = C
PORTNUM = 9002
IP_ADD = 127.0.0.1

all:
	$(CC) -o $(CTARGET) $(CSRC) -pthread
	$(CC) -o $(STARGET) $(SSRC)

s:
	./$(STARGET) $(PORTNUM)

c:
	./$(CTARGET) $(IP_ADD) $(PORTNUM)

clean:
	rm $(CTARGET) $(STARGET)
