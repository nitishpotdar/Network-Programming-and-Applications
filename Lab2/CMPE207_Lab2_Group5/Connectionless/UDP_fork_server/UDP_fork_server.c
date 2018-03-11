#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#define MAX_SEND_BUF 1600
/* Buffer length fixed to max 1600 bytes to send the contents of server file */

int main(int argc, char *argv[])
{
    /*
     *  sockfd is the socket descriptor which is an integer value
     *  client_address is the struct used to specify a local or remote endpoint address of client.
     */
    int sockfd;
    struct sockaddr_in serv_addr, client_address;
    int address_len = sizeof(serv_addr);
    int cli_len = sizeof(client_address);

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
        printf("Error calling Socket\n");
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
        printf("Error binding\n");
        exit(1);
    }

    signal(SIGCHLD, SIG_IGN);
    while(1)
    {
        char msg[1000];
        long data_len;
        /* recvfrom() call - the requested filename is stored in string msg and client address are stored in client_address struct to used for reply */
        data_len = recvfrom(sockfd,(char*)msg,sizeof(msg),0,(struct sockaddr *)&client_address, (socklen_t*)&cli_len);
        //recieve the file name from client
            
        if(data_len)
        {
            printf("\n\nClient connected to Multiprocess connectionless server using fork call\n");
            printf("File name received: %s\n", msg); 
        }
        /* child process is created to handle the file transfer */
        if (fork() == 0)
        {
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
                while( (read_bytes = read(file, send_buf, MAX_SEND_BUF)) > 0 )          //read the contents of file on server
                {   
                }
                sent_bytes = sendto(sockfd,send_buf, 1600 , 0,(struct sockaddr *)&client_address, sizeof(client_address));
                printf("\nContents of file - %s , transfered to the client", msg);
                /* close() call closes the file for future references. */
                close(file);
                printf("\nClient disconnected\n\n\n");
            }
        }
    }
}
