#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen, buffer, n;
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

    /* 
    *  Fetching the port number from the command line argument 
    *  In this case it will be the second argument in the command line
    */
    portno = atoi(argv[1]);
    printf("\nPort number received from command line is %d\n ",portno);

    /* Populating the sockaddr_in struct with the following values */
    /* Assigning the AF_INET (Internet Protocol v4) address family */
    serv_addr.sin_family = AF_INET;

    /* Populating the Server IP address with the value of the localhost IP address */
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* Converting the port number received from the command line from host byte order to network byte order */
    serv_addr.sin_port = htons(portno);

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
    if(listen(sockfd,5) < 0)
    {
        printf("error listening");
        exit(1);
    }

    newsockfd = accept(sockfd,(struct sockaddr *) &serv_addr, (socklen_t*)&address_len);
    buffer = 0;

    /* Read() takes three arguments similar to the write call
    *  The Socket Descriptor 
    *  Address of the data to be read
    *  And the length of the data
    */
    n = read(newsockfd,&buffer,sizeof(int));

    printf("Here is the numeric: %d\n",buffer);
    buffer = buffer - 1; /* Decrementing the numeric value received from the client*/
    printf("%d sent to the client", buffer);

    /* Write() takes three arguments:
    *  The socket Descriptor
    *  Address of the data to be sent 
    *  And the length of the data
    */
    n = write(newsockfd,&buffer,sizeof(int));
    close(sockfd);
    return 0;
}
