all: TCP_fork_server.o TCP_fork_client.o

TCP_fork_client.o: TCP_client.c	
	gcc TCP_client.c -o TCP_fork_client.o

TCP_fork_server.o: TCP_fork_server.c
	gcc TCP_fork_server.c -o TCP_fork_server.o