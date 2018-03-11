all: TCP_preforked_server.o TCP_preforked_client.o

TCP_preforked_client.o: TCP_client.c	
	gcc TCP_client.c -o TCP_preforked_client.o

TCP_preforked_server.o: TCP_preforked_server.c
	gcc TCP_preforked_server.c -o TCP_preforked_server.o