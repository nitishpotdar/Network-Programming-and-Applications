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
    pthread_t thread_tid;
    long    thread_count;      
 }  Thread;
 
Thread *tptr;                  
socklen_t addrlen;
pthread_mutex_t mlock;
void thread(int);

/*
 *  sockfd is the socket descriptor which is an integer value
 *  client_address is the struct used to specify a local or remote endpoint address of client.
 */
struct sockaddr_in serv_addr, client_address;
int sockfd, i, nthreads;
int cli_len = sizeof(client_address);
pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[])
{    
    nthreads = atoi(argv[argc - 1]);
    tptr = calloc(nthreads, sizeof(Thread));
    
    /*
     *  Socket() call takes three arguments:
     *  The family of protocol used/address family
     *  Type of socket
     *  Protocol number or 0 for a given family and type
     */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
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
    /* Converting the port number from host byte order to network byte order */
    serv_addr.sin_port = htons(8000);

    /* bind() call binds the socket descriptor to the serv_addr */
    /* Returns 0 for success and -1 for failure */
    if(bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        printf("error binding");
        exit(1);
    }
    
    /* nthreads are created as requested from the command line */
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
    /* threads are created using pthread_create() call */
    pthread_create(&tptr[i].thread_tid, NULL, &thread_main, (void *) i);
    return;
}

void *thread_main(void *arg)
{
     printf("Thread %d starting\n", (int) arg);
     for ( ; ; ) 
     {
        char msg[1000];
        long data_len;
         /* recvfrom() call - the requested filename is stored in string msg and client address are stored in client_address struct to used for reply */
        data_len = recvfrom(sockfd,(char*)msg,sizeof(msg),0,(struct sockaddr *)&client_address, (socklen_t*)&cli_len);
         //recieve the file name from client
        if(data_len)
        {
            printf("\n\nClient connected to Prethreaded connectionless server\n");
            printf("File name received: %s\n", msg);
        }
        int file;                                                                   //read the local file (server file)
         
         /* if the filename is incorrect or file is not available, the server sends "INVALID" message on the socket to the client using sendto() call.
          * prints the error message on server end
          */
        if((file = open(msg,O_RDWR)) == -1)
        {
            char* invalid = "INVALID";
            printf("\nFile not found\n");
            printf("Client disconnected\n\n");
            printf("%s\n",strerror(errno));
            sendto(sockfd, invalid, MAX_SEND_BUF, 0, (struct sockaddr*)&client_address, sizeof(client_address));
        }
        /* if the file is found then the contents are stored in send_buf buffer using read() call and sent over to the client by sendto() call. */
        else
        {
            printf("File opened successfully\n");
            ssize_t read_bytes;
            ssize_t sent_bytes;
            
            char send_buf[MAX_SEND_BUF];
            /* send_buf buffer is used to store the file contents */
            while( (read_bytes = read(file, send_buf, MAX_SEND_BUF)) > 0 )
            {
            }
            sent_bytes = sendto(sockfd,send_buf, 1600 , 0,(struct sockaddr *)&client_address, sizeof(client_address));
            printf("\nContents of file - %s , transfered to the client", msg);
            /* close() call closes the file for future references. */
            close(file);
            printf("\n\nClient disconnected\n\n\n");
            //pthread_exit(NULL);
        }
    }
}
