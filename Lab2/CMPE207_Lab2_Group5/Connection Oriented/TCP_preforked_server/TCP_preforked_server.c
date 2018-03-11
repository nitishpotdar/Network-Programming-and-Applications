#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#define MAX_SEND_BUF 1600
/* Buffer length fixed to max 1600 bytes to send the contents of server file */

static int num_child;
static pid_t *pids;

pid_t create_child(int i, int listenfd, int addrlen);
void child_process(int i, int listenfd, int addrlen);
int file_transfer(int i);

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in serv_addr;
    num_child = atoi(argv[1]);

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
    if(bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)/* Returns 0 for success and -1 for failure */
    {
        printf("error binding");
        exit(1);
    }

    /* Listen call makes the socket to be a passive type, taking two arguments:
    *  Socket Descriptor
    *  number of connection that can be queued 
    */
    if(listen(sockfd,100) < 0)
    {
        printf("error listening");
        exit(1);
    }
    socklen_t addrlen = 0;
    pids = calloc(num_child, sizeof(pid_t));
    int i;
    
    /* Create number of child process as specified on command line*/
    for (i = 0; i < num_child; i++)
    pids[i] = create_child(i, sockfd, addrlen); /* parent returns */
    child_process(pids[i], sockfd, addrlen); /* never returns */
    while(1)
    {
        pause();
    }
    return 0;
}

/* Create child process*/
pid_t create_child(int i, int listenfd, int addrlen)
{
    pid_t pid;
    if ( (pid = fork()) > 0)
    {
        return (pid);
    }
    return pid;
}

/* Child in accept mode and initiate transfer on client request*/
void child_process(int i, int listenfd, int addrlen)
{
    int connfd;
    socklen_t clilen;
    struct sockaddr *cliaddr;
    cliaddr = malloc(addrlen);
    for(;;)
    {
        clilen = addrlen;
        /* server accepts incoming conenction on sockfd and assigns it to newsockfd */
        /*
         *  Accept takes three arguments:
         *  Socket descriptor
         *  Server Address Structure - Local endpoint in this case
         *  Size of the address
         */
        connfd = accept(listenfd, cliaddr, &clilen);
        file_transfer(connfd);                                                          //process the request from the client
    }
}

/* Initiate the file transfer upon request from client */
int file_transfer(int newsockfd)
{
    char msg[1000];
    long data_len;
    /* recv() call - the requested filename is stored in string msg received on newsockfd socket descriptor */
    data_len = recv(newsockfd,msg,sizeof(msg),0);                                       //recieve the file name from client
    if(data_len)
    {
        printf("\n\nClient connected to Preforked Multiprocessing connection oriented server\n");
        printf("File name received: %s\n", msg);

    }
    int file;                                                                           //read the local file (server file)
    /* if the filename is incorrect or file is not available, the server sends "INVALID" message on the socket to the client using sendto() call.
     * prints the error message on server end
     */
    if((file = open(msg,O_RDWR)) == -1)
    {
        char *invalid= "INVALID";              
        printf("\nFile not found");
        printf("\nClient disconnected");
        printf("\n%s",strerror(errno));
        send(newsockfd,invalid,MAX_SEND_BUF,0);
    }
    /* if the file is found then the contents are stored in send_buf buffer using read() call and sent over to the client by send() call. */
    else
    {
        printf("\nFile opened successfully");
        ssize_t read_bytes;
        ssize_t sent_bytes;

        char send_buf[MAX_SEND_BUF];
        /* send_buf buffer is used to store the file contents */
        while( (read_bytes = read(file, send_buf, MAX_SEND_BUF)) > 0 )                  //read the contents of file on server
        {
            if( (sent_bytes = send(newsockfd, send_buf, read_bytes, 0)) < read_bytes )  //send the contents of file to client
            {
                printf("\nsend error");
                return -1;
            }
        }
        printf("\nContents of file - %s , transfered to the client", msg);
        /* close() call closes the file for future references. */
        close(file);
        printf("\nClient disconnected\n");
    }
    close(newsockfd);
    return 0;
}


