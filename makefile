all: client server

client: multi-client.c
	gcc -w -o multi-client multi-client.c

server: server-mp.c
	gcc -w -o server-mp server-mp.c

clean: 
	rm multi-client
	rm server-mp