all: server client
server: server.c rsa.o rsa.h
	gcc -o server server.c rsa.c
client: client.c rsa.o rsa.h
	gcc -o client client.c rsa.c
clean:
	rm server
	rm client