#include <stdio.h>
#define _USE_BSD 1
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUF_SIZE 80000
int check_url(char * ip);
char path[10000];
char * ptr;

int  conn_socket(const char *service,const char *host,char *url,int portnum);

/*
Arguments:
*service   - service associated with desired port
*host      - name of the host to which connection is desired
*transport - name of the transport protocol to use
*/ 

int conn_socket(const char *service,const char *host,char *url,int portnum)
{
    char *transport = "tcp";
    struct sockaddr_in sin; 
    struct hostent *hptr;					 //an internet endpoint address
    int sockfd,type;               			 //socket descriptor and socket type
    memset(&sin,0,sizeof(sin));  
    sin.sin_family=AF_INET;   				         //family name
    sin.sin_port=htons(portnum);                                        //port number

    /* Determine the transport type */
    if(strcmp(transport,"udp")==0)         
    type=SOCK_DGRAM;
    else
    type=SOCK_STREAM;
    
    /* getrequest char array stores the reponse from the server */
    char getrequest[1024];
    /* Validate the url for valid ip address, hostname or hostname with filename */
    if (check_url(url)) 
    {
        sprintf(getrequest, "GET / HTTP/1.0\nHOST: %s\n\n", url);
    } 
    else 
    {
        if ((ptr = strstr(url, "/")) == NULL) 
        {
            sprintf(getrequest, "GET / HTTP/1.0\nHOST: %s\n\n", url);
        } 
        else 
        {
            strcpy(path, ptr);
            host = strtok(url, "/");
            sprintf(getrequest, "GET %s HTTP/1.0\nHOST: %s\n\n", path, url);
        }
    }
    hptr=gethostbyname(url);
    bcopy((char*)hptr->h_addr, (char*)&sin.sin_addr.s_addr,hptr->h_length);
    printf("\n%s\n",getrequest);
    
    /* sockfd is the socket descriptor which is an integer value */
    sockfd=socket(AF_INET,type,0);

    if((connect(sockfd,(struct sockaddr *) &sin,sizeof(sin)))<0)
    printf("\nConnection successful...\n\n\n");
    ptr = strtok(path, "/");
    /* send HTTP GET request to the server */
    write(sockfd, getrequest, strlen(getrequest));
    return sockfd;
}

int main(int argc,char *argv[])
{
    char *host="localhost";
    ssize_t recv_bytes;
    char *service="time";
    int sockfd,n,portnum,portNumber;
    char *url, *temp;
    char buffer[BUF_SIZE],response_str[1000],header_str[1000];
    clock_t start,end,total;

    url = argv[1];
    portNumber = atoi(argv[2]);
    
    /* If invalid number of arguments provided on command line */
    if (argc < 3) 
    {
        printf("usage: [URL] [port number]\n");
        exit(1);  
    }
    
    /* append http:// or https:// to the url */
    if ((temp = strstr(url, "http://")) != NULL) 
    {
        url = url + 7;
    } 
    else if ((temp = strstr(url, "https://")) != NULL) 
    {
        url = url + 8;
    }
    
    /* Start the clock to record time */
    start=clock();
    
    /* call conn_socket function to the GET request to the server */
    sockfd = conn_socket(service,host,url,portNumber);
    memset(&buffer, 0, sizeof(buffer));
    ssize_t rcvd_bytes;
    int file;                   // File pointer to the new file
    char *filename = "cmpe207_copy.txt";

    /* receive the HTTP response from the server */
    ssize_t head_length = recv(sockfd,response_str,sizeof(response_str),0);
    printf("HTTP response: %s\n",response_str);
    
    /* receive the header from the server */
    recv(sockfd,header_str,sizeof(header_str),0);
    printf("%s",header_str);
    fflush(stdout);
    /* receive the contents of the server file on sockfd socket */
    while((rcvd_bytes = recv(sockfd,buffer,BUF_SIZE,0))>0)
        {
            /* delete the file if already present */
            struct stat s;
            int r = stat("cmpe207_copy.txt", &s);
            if(r == 0) 
            {
                remove("cmpe207_copy.txt");
            }
            /* Print number of bytes received */
            printf("\nNumber of bytes received = %zd\n",(rcvd_bytes+head_length));
            fflush(stdout);
            
            /* create new file and open it */
            FILE *file = fopen(filename,"w+");
            printf("\nWriting to file the below contents\n\n");
            printf("%s\n",buffer);
            
            /* write the contents to the new file created */
            fwrite(buffer,1,sizeof(buffer),file);
            
            /* Close the file after writing */
            fclose(file);
            
            /* Print the contents on terminal */
            printf("\nContents of server file copied to - %s file",filename);
            close(sockfd);                      // close the socket
            
            /* end the clock to stop the time */
            end=clock();
            
            /* Calculate total time required to fetch and store the file */
            total=(double)(end-start);
            printf("\nTime required to fetch the file : %f seconds\n\n", (double)total/1000);
        }
 	return 0;
}

/* Check if the url is valid or not */
int check_url(char * ip)
{
     struct sockaddr_in addr;
     int valid = inet_pton(AF_INET, ip, &(addr.sin_addr));
     return valid != 0;
}
