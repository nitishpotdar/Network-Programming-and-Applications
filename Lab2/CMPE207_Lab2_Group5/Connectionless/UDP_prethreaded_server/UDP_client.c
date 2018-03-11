#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define MAX_RECV_BUF 1600
/* Buffer length fixed to max 1600 bytes to receive the contents from server file */

int main(int argc, char *argv[])
{
    /*
     *  sockfd is the socket descriptor which is an integer value
     *  serv_addr is the struct used to specify a local or remote endpoint address of server.
     *  filename is used to store the name of the file
     */
    int sockfd;
    struct sockaddr_in serv_addr;
    char *file_name;
    int serv_len = sizeof(serv_addr);

    /* Filename is passed as a command line argument */
    file_name=argv[1];

    /*
     *  Socket() call takes three arguments:
     *  The family of protocol used/address family
     *  Type of socket (SOCK_STREAM for TCP)
     *  Protocol number or 0 for a given family and type
     */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    /*
     *  Socket call will return a socket descriptor on success which is an integer
     *  And it will return '-1' for error
     */
    if (sockfd == -1) 
    {
        printf("\nerror calling socket\n");
        exit(1);
    }

    /* Populating the sockaddr_in struct with the following values */
    /* Assigning the AF_INET (Internet Protocol v4) address family */
    serv_addr.sin_family = AF_INET;
    /* Populating the Server IP address with the value of the localhost IP address */
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    /* Converting the port number from host byte order to network byte order */
    serv_addr.sin_port = htons(8000); 
    
    ssize_t recv_bytes;
    /* recv_buff is the receive buffer to store the server file contents. */
    char recv_buff[MAX_RECV_BUF];
    printf("\n\nConnecting to the server...\n");
    
    /*
     *  sendto() sends the filename to the server whose details are stored in serv_addr on socket descriptor sockfd
     */
    sendto(sockfd,file_name,strlen(file_name),0,(struct sockaddr *)&serv_addr, sizeof(serv_addr));
    
    int ptr;
    char *newfile= "newfile.txt";
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;

    /*
     *  recv() receives the file contents from the server file
     */
    recv_bytes=recvfrom(sockfd,(char*)recv_buff,sizeof(recv_buff),0,(struct sockaddr *)&serv_addr, (socklen_t*)&serv_len);
    if(recv_bytes <= 0)
    { 
    	printf("\n Error in receving \n");
	    exit(1);
    }
    else
    {
        /* New file is created on client end and error message is printed if the file cannot be created. */
        if((ptr= open(newfile,O_WRONLY|O_CREAT,0644))<0)
        {
            printf("Error in writing to file\n");
        }
        
        /*  Incoming data from server is compared with "INVALID" string or valid data
         *  If the server cannot send the file contents then it sends a string "INVALID"
         *  If the server sends the file contents successfully, the contents are copied in the new file created.
         */
        if(strncmp(recv_buff, "INVALID", 7) == 0)
        {
            printf("File not found\n\n\n");
            exit(1);
        }
        else
        {
            open(newfile,O_RDWR|O_CREAT|O_EXCL|O_APPEND,mode);
            write(ptr,recv_buff,recv_bytes);
            printf("%s\n",recv_buff);
            printf("\nNew file '%s' has been created and the contents from server file have been copied to it.\n\n",newfile);
        }
        
    }
    /* the socket descriptor is closed once the file is transfered. */
    close(sockfd);
    return 0;
}
