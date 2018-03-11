#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#define BUF_SIZE 80000
#define URL_SIZE 128

static char* HTTP_ACCEPTED =
    "HTTP/1.0 200 OK\n"
    "Content-type: text/html\n";

static char* FILE_NOT_FOUND =
    "HTTP/1.0 404 Not Found\n"
    "Content-type: text/html\n"
    "\n"
    "<html>\n"
    " <body>\n"
    "  <h1>FILE NOT FOUND</h1>\n"
    "  <p>The requested was not found on this server.</p>\n"
    " </body>\n"
    "</html>\n";

int main(int argc, char *argv[])
{
    /*
     *  sockfd is the socket descriptor which is an integer value
     *  client_address is the struct used to specify a local or remote endpoint address of client.
     */
    int sockfd, newsockfd;
    struct sockaddr_in serv_addr;
    int address_len = sizeof(serv_addr);
    char *dir, *host;
    int portno;
    host = argv[1];             //Get the host address
    dir = argv[2];              //Get the file directory
    portno = atoi(argv[3]);     //Get the port number
    
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
    serv_addr.sin_addr.s_addr = htons(INADDR_ANY);

    /* Converting the port number received from the command line from host byte order to network byte order */
    serv_addr.sin_port = htons(portno);
    

    /* 
    *  Bind takes three arguments: - Used to bind the local endpoint parameters to the socket
    *  Socket descriptor
    *  Server Address Structure - Local endpoint in this case
    *  Size of the address
    */
    if(bind(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)/* Returns 0 for success and -1 for failure */
    {
        printf("Error binding");
        exit(1);
    }

    /* Listen call makes the socket to be a passive type, taking two arguments:
    *  Socket Descriptor
    *  number of connection that can be queued 
    */
    while(1)
    {
        if(listen(sockfd,100) < 0)
        {
            printf("Error listening");
            exit(1);
        }

        /* server accepts incoming conenction on sockfd and assigns it to newsockfd */
        /*
         *  Accept takes three arguments:
         *  Socket descriptor
         *  Server Address Structure - Local endpoint in this case
         *  Size of the address
         */
        newsockfd = accept(sockfd,(struct sockaddr *) &serv_addr, (socklen_t*)&address_len);
        
        time_t timenow;
        struct tm * timeinfo;
        time (&timenow);
        timeinfo = localtime(&timenow);
        int count;
        char buffer[BUF_SIZE];
        char request_str[URL_SIZE];
        char filename[URL_SIZE];
        int fd;
        char *header;
        char *temp;
        header = (char *)malloc(BUF_SIZE*sizeof(char));
        count = read(newsockfd, buffer, BUF_SIZE-1);
        buffer[count] = (char)0;
        printf("\n\n%s\n\n",buffer);
        if (count == 0)
        {
            close(newsockfd);
            return 0;
        }

        /* Check for a HTTP GET Method */
        /* If the HTTP GET is NOT FOUND, send the FILE_NOT_FOUND on the socket */
        if (1 > sscanf(buffer, "GET %s", request_str))
        {
            send(newsockfd,FILE_NOT_FOUND,strlen(FILE_NOT_FOUND),0);
        }
        /* If the HTTP GET is OK, send the HTTP_ACCEPTED on the socket and
         also send the file contents*/
        else
        {
            /* File name requested by client */
            printf("Requested File: %s\n", request_str);
            bzero(filename, URL_SIZE);
            strncpy(filename, dir, URL_SIZE);
            strncat(filename, request_str, URL_SIZE);
            /* Path as speficified on the command line */
            printf("FilePath: %s\n\n", filename);
            /* Append the date, host, location, port number and directory in header */
            sprintf(header, "Date: %sHostname: %s:%d\nLocation: %s\n\n", asctime(timeinfo), host, portno, dir);
            
            /* Opening the file */
            fd = open(filename, O_RDONLY);
            /* If unable to open the file, send FILE_NOT_FOUND on socket */
            if (fd < 0)
            {
                printf("Requested URL %s is not found on this server.\n",filename);
                send(newsockfd,FILE_NOT_FOUND,strlen(FILE_NOT_FOUND),0);
             }
            /* If the file opened successfully, send the contents on the socket */
            else
            {
                printf("The contents of the file are:\n");
                size_t readbytes,sendbytes;
                while((readbytes = read(fd,buffer,80000))>0)
                {
                    send(newsockfd,HTTP_ACCEPTED,strlen(HTTP_ACCEPTED),0);
                    send(newsockfd,header,strlen(header),0);
                    printf("%s\n",buffer);
                    if((sendbytes =send(newsockfd,buffer,readbytes,0)) < readbytes)
                    {
                        printf("send() failed");
                        exit(-1);
                    }
                }
                close(fd);          // Close the file descriptor
                printf("\nThe file contents transfered to client\n");
                printf("Client disconnected\n");
            }
        }
        close(newsockfd);           // Close the new socket
        printf("Waiting for new connection...\n\n");
    }
    close(sockfd);                  // Close the main socket
    return 0;
}
