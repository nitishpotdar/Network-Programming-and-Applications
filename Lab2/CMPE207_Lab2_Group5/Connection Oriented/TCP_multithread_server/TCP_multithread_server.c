#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<signal.h>
#include <unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#define MAX_SEND_BUF 1600
/* Buffer length fixed to max 1600 bytes to send the contents of server file */

void con_handler(void *);

int newsockfd, i;
int main(int argc, char *argv[])
{
    /*
     *  sockfd is the socket descriptor which is an integer value
     *  client_address is the struct used to specify a local or remote endpoint address of client.
     */
    int sockfd;
    struct sockaddr_in serv_addr;
    int address_len = sizeof(serv_addr);
    
    /*
     *  Socket() call takes three arguments:
     *  The family of protocol used/address family
     *  Type of socket
     *  Protocol number or 0 for a given family and type
     */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    /*
     *  Socket call will return a socket descriptor on success which is an integer
     *  And it will return '-1' for error
     */
    if (sockfd == -1)
    {
        printf("Error calling Socket");
        exit(1);
    }
    /* Populating the sockaddr_in struct with the following values */
    /* Assigning the AF_INET (Internet Protocol v4) address family */
    serv_addr.sin_family = AF_INET;
    /* Populating the Server IP address with the value of the localhost IP address */
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    /* Converting the port number received from the command line from host byte order to network byte order */
    serv_addr.sin_port = htons(8000);

    /*
     *  Bind takes three arguments: - Used to bind the local endpoint parameters to the socket
     *  Socket descriptor
     *  Server Address Structure - Local endpoint in this case
     *  Size of the address
     */
    if(bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        printf("Error binding\n");
        exit(1);
    }

    /* Listen call makes the socket to be a passive type, taking two arguments:
     *  Socket Descriptor
     *  number of connection that can be queued
     */
    if(listen(sockfd,5) < 0)
    {
        printf("Error listening\n");
        exit(1);
    }
    
    while(1)
    {
        /* server accepts incoming conenction on sockfd and assigns it to newsockfd */
        /*
         *  Accept takes three arguments:
         *  Socket descriptor
         *  Server Address Structure - Local endpoint in this case
         *  Size of the address
         */
        newsockfd = accept(sockfd,(struct sockaddr *) &serv_addr, (socklen_t*)&address_len);

        if (newsockfd < 0)
        {
            printf("Error accepting\n");
            exit(1);
        }
        
        /* new thread is created to handle the file transfer using pthread_create() call
        * returns 0 for success and -1 for failure */
        pthread_t child;
        if(pthread_create( &child, NULL ,  con_handler , (void*) newsockfd) < 0)
        {
            printf("Error creating thread\n");
            exit(1);
        }
    }
    close(newsockfd);
}


void con_handler(void *sockfd)
{
    int sock = (int)sockfd;
    int n, buffer;
   
        char msg[1000];
        long data_len;
        /* recv() call - the requested filename is stored in string msg received on newsockfd socket descriptor */
        data_len = recv(newsockfd,msg,sizeof(msg),0);                              //recieve the file name from client        
        if(data_len)
        {
            printf("\n\nClient connected to Multithread connection oriented server\n");
            printf("File name received: %s\n", msg);
        }
        int file;                                                                   //read the local file (server file)
    
    /* if the filename is incorrect or file is not available, the server sends "INVALID" message on the socket to the client using sendto() call.
     * prints the error message on server end
     */
        if((file = open(msg,O_RDWR)) == -1)
        {
            char* invalid = "INVALID";
            printf("File not found\n");
            printf("Client disconnected\n");
            printf("%s\n",strerror(errno));
            send(newsockfd,invalid,MAX_SEND_BUF,0);
        }
        /* if the file is found then the contents are stored in send_buf buffer using read() call and sent over to the client by send() call. */
        else
        {
            printf("File opened successfully\n");
            ssize_t read_bytes;
            ssize_t sent_bytes;
            
            char send_buf[MAX_SEND_BUF];
            /* send_buf buffer is used to store the file contents */
            while( (read_bytes = read(file, send_buf, MAX_SEND_BUF)) > 0 )          //read the contents of file on server
            {
                if( (sent_bytes = send(newsockfd, send_buf, read_bytes, 0)) < read_bytes )  //send the contents of file to client
                {
                    printf("send error");
                    //return -1;
                }
            }
            printf("\nContents of file - %s , transfered to the client", msg);
            /* close() call closes the file for future references. */
            close(file);
            printf("\n\nClient disconnected\n\n");
        }
        close(newsockfd);
    //pthread_exit(NULL);
}
