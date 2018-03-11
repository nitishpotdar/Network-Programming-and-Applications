#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


int main(int argc, char *argv[])
{
    /* 
    *  sockfd is the socket descriptor which is an integer value
    *  portno will be recived from the command line argument, which will also be an integer value
    *  n is used to store the number of bytes sent/received to and from the server
    *  buffer will be used to send and the receive the numeric to and from the server.
    */
    int sockfd, portno, n, buffer;
    struct sockaddr_in serv_addr;
 
    /* 
    *  Fetching the port number from the command line argument 
    *  In this case it will be the second argument in the command line
    */
    portno = atoi(argv[1]); 

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
        printf("\nerror calling socket\n");
        exit(1);
    }

    /* Populating the sockaddr_in struct with the following values */
    /* Assigning the AF_INET (Internet Protocol v4) address family */
    serv_addr.sin_family = AF_INET; 

    /* Populating the Server IP address with the value of the localhost IP address */
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

    /* Converting the port number received from the command line from host byte order to network byte order */
    serv_addr.sin_port = htons(portno); 

    printf("\nPort Number received from the command line argument is: %d\n", portno);

    /* 
    *  Connect () takes three arguments:
    *  The sockect descriptor
    *  Server Address Structure
    *  And the size of Address 
    */
    if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0) /* Connect() returns '-1' on failure and 0 on succes */
    {
        printf("\nerror connecting\n");
        exit(1);
    }

    printf("\nPlease enter a numeric digit: \n"); /* Enter the value that is to be decremented by the Server */
    scanf("%d", &buffer);

    /* Write() takes three arguments:
    *  The socket Descriptor
    *  Address of the data to be sent 
    *  And the length of the data
    */
    n = write(sockfd,&buffer,sizeof(int));

    /* Read() takes three arguments similar to the write call
    *  The Socket Descriptor 
    *  Address of the data to be read
    *  And the length of the data
    */
    n = read(sockfd,&buffer, sizeof(int));

    printf("\nValue received from server after decrementing is %d\n",buffer);
    close(sockfd);
    return 0;

    
}