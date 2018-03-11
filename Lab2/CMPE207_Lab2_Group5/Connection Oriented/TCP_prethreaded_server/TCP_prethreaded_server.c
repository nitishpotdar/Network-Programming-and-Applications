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

typedef struct 
{
    pthread_t thread_tid;      /* thread ID */
    long    thread_count;      
 }  Thread;

/*
 *  sockfd is the socket descriptor which is an integer value
 *  client_address is the struct used to specify a local or remote endpoint address of client.
 */
Thread *tptr;                  
socklen_t addrlen;
pthread_mutex_t mlock;
void thread(int);
struct sockaddr_in serv_addr;
int sockfd, i, nthreads;
int address_len = sizeof(serv_addr);
pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER;
int main(int argc, char *argv[])
{    
    nthreads = atoi(argv[argc - 1]);
    tptr = calloc(nthreads, sizeof(Thread));
    
    /*  Socket() call takes three arguments:
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
        printf("Error calling Socket\n");
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
    /* Returns 0 for success and -1 for failure */
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
    
    /* nthreads number of threads are created as specified on command line */
    for(i=0; i<nthreads; i++)
    {
        thread(i);
    }
    for ( ; ; )       
        pause();   
}             

void thread(int i)
{
    void *thread_main(void *);

    /* new threads are created to handle file transfer using pthread_create(0 call */
    pthread_create(&tptr[i].thread_tid, NULL, &thread_main, (void *) i);
    return;
}

void *thread_main(void *arg)
{
     int     connfd, buffer, n;
     void    web_child(int);
     printf("Thread %d starting\n", (int) arg);
     for ( ; ; ) 
     {
        pthread_mutex_lock(&mlock);
         
        /* server accepts incoming conenction on sockfd and assigns it to newsockfd */
        /*
          *  Accept takes three arguments:
          *  Socket descriptor
          *  Server Address Structure - Local endpoint in this case
          *  Size of the address
        */
        connfd = accept(sockfd, (struct sockaddr *) &serv_addr,(socklen_t*)&address_len);
        pthread_mutex_unlock(&mlock);
        tptr[(int) arg].thread_count++;

        char msg[1000];
        long data_len;
        /* recv() call - the requested filename is stored in string msg received on newsockfd socket descriptor */
        data_len = recv(connfd,msg,sizeof(msg),0);
        if(data_len)
        {
            printf("\n\nClient connected to Prethreaded connection oriented server\n");
            printf("File name received: %s\n", msg);
            
        }
        int file;                                                                   //read the local file (server file)
        if((file = open(msg,O_RDWR)) == -1)
        {
            char* invalid = "INVALID";
            printf("File not found\n");
            printf("Client disconnected\n");
            printf("%s\n",strerror(errno));
            send(connfd,invalid,MAX_SEND_BUF,0);
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
                if( (sent_bytes = send(connfd, send_buf, read_bytes, 0)) < read_bytes )  //send the contents of file to client
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
        close(connfd);
    }
}
