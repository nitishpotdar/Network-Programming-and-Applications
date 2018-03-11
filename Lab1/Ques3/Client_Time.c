#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>

#define UNIXEPOCH 2208988800

int main()
{
  int clientSocket1;
  int clientSocket2;
  char buffer[1024];

  struct sockaddr_in serverAddr1;
  struct sockaddr_in serverAddr2;
  socklen_t addr_size;

  time_t time1;
  time_t time2;
  double diff;

  // Create 2 sockets. The three arguments are family, type and protocol.
  clientSocket1 = socket(AF_INET, SOCK_STREAM, 0);
  clientSocket2 = socket(AF_INET, SOCK_STREAM, 0);
  
  // Address family = Internet 
  serverAddr1.sin_family = AF_INET;
  serverAddr2.sin_family = AF_INET;

  //Assign port number and ip address for server1
  serverAddr1.sin_port = htons(37);
  serverAddr1.sin_addr.s_addr = inet_addr("128.138.140.44");

  //Assign port number and ip address for server2
  serverAddr2.sin_port = htons(37);
  serverAddr2.sin_addr.s_addr = inet_addr("128.138.141.172");
    
  //
  addr_size = sizeof serverAddr1;
  connect(clientSocket1, (struct sockaddr *) &serverAddr1, addr_size);

  //
  addr_size = sizeof serverAddr2;
  connect(clientSocket2, (struct sockaddr *) &serverAddr2, addr_size);

  //Time for Server1
  recv(clientSocket1, (char *)&time1, 1024, 0);
  time1 = ntohl((u_long)time1);
  time1 -= UNIXEPOCH;
  
  //To print the time of Server1
  printf("Server1 Time: %s",ctime(&time1));
  
  //Time for Server2
  recv(clientSocket2, (char *)&time2, 1024, 0);
  time2 = ntohl((u_long)time2);
  time2 -= UNIXEPOCH;
  
  //To print the time for of Server2
  printf("Server2 Time: %s",ctime(&time2));
  
  //Calculate difference between time1 and time2
  diff = difftime (time1,time2);

  //Print the difference
  printf("Difference is: %f \n",diff);

  return 0;
}
