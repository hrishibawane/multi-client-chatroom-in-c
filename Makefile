CC = gcc
LDFLAGS = -lpthread

all: server client

server:
	$(CC) server.c -o server $(LDFLAGS)

client:
	$(CC) client.c -o client $(LDFLAGS)

clean:
	rm -rf server client

