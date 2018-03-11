#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <netdb.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define __USE_BSD 1
#define BUFSIZE 64
#define UNIXEPOCH 2208988800    /* UNIX epoch, in UCT secs */
#define MSG "What time is it?\n"
#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff
#endif

int connectSock(const char *host, const char *service, const char *transport);
int connectUDP(const char *host, const char *service);
int error_exit(const char *format, ...);

int main(int argc, char *argv[])
{
    char *host = "localhost";   /* Host to be used if none passed */
    char *service = "time";     /* default service name */
    time_t now;                 /* 32-bit integer to hold time */
    int s, n;                   /* socket descriptor and read count */
    
    switch (argc)
    {
        case 1:
            host = "localhost";
            break;
        case 2:
            host = argv[1];
            break;
        case 3:
            service = argv[2];

        default:
            fprintf(stderr, "usage: UDPtime [host [port]]\n");
            exit(1);
    }
    
    /* Establishes connectivity with the server by choosing UDP Connected mode.
     Returns a positive integer on connecting */
    s = connectUDP(host, service);
    
    (void) write(s, MSG, strlen(MSG));
    
    /* Reading the time */
    n = (int) read(s, (char *)&now, sizeof(now));
    if (n < 0)
    {
        printf("read failed: %s\n", strerror(errno));
    }
    now = ntohl((unsigned long)now);    /* in host byte order */
    now -= UNIXEPOCH;                   /* convert UCT to UNIX epoch */
    printf("%s", ctime(&now));
    exit(0);
}


/*connectUDP - connect to a specified service on a specified host */
/*
 * Arguments passed in function call:
 * host - host name to which connection is desired
 * service - service associated with the desired port
 */
int connectUDP(const char *host, const char *service )
{
    return connectSock(host, service, "udp");
}


/*connectSock - connect to a specified TCP service on a specified host */
/*
 * Arguments passed in function call:
 * host - host name to which connection is desired
 * service - service associated with the desired port
 * transport - name of transport protocol to use (here - udp)
 */

int connectSock(const char *host, const char *service, const char *transport)
{
    struct hostent *phe;        /* pointer to host information */
    struct servent *pse;        /* pointer to service information */
    struct protoent *ppe;       /* pointer to protocol information */
    struct sockaddr_in sin;     /* internet endpoint address */
    int s, type;                /* socket descriptor and socket type */
    
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    
    /* Map service name to port number */
    if ( (pse = getservbyname(service, transport)) )
    {
        sin.sin_port = pse->s_port;
    }
    else if ( (sin.sin_port = htons((unsigned short)atoi(service))) == 0 )
    {
        error_exit("can't get \"%s\" service entry\n", service);
    }
    
    /* Map host name to IP address, allowing for dotted decimal */
    if ( (phe = gethostbyname(host)) )
    {
        memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
    }
    else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
    {
        error_exit("can't get \"%s\" host entry\n", host);
    }
    
    /* Map transport protocol name to protocol number */
    if ( (ppe = getprotobyname(transport)) == 0)
    {
        error_exit("can't get \"%s\" protocol entry\n", transport);
    }
    
    /* Use protocol to choose a socket type */
    if (strcmp(transport, "udp") == 0)
    {
        type = SOCK_DGRAM;      /* type selected as socket datagram */
    }
    else
    {
        type = SOCK_STREAM;     /* type selected as socket stream */
    }
    
    /* Use protocol to choose a socket type */
    s = socket(PF_INET, type, ppe->p_proto);
    if (s < 0)
    {
        error_exit("can't create socket: %s\n", strerror(errno));
    }
    
    /* Connect the socket */
    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        error_exit("can't connect to %s.%s: %s\n", host, service,strerror(errno));
    }
    return s;
}

/* to print the error message and exit */
int error_exit(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    exit(1);
}
