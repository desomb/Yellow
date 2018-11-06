#include "all.h"

int iphdrlen;
struct sockaddr socket_addr;
struct sockaddr_in source;
struct sockaddr_in dest;

int main()
{
  int socket_fd;
  int addr_len;
  int bufflen;
  unsigned char *buffer = (unsigned char *)malloc(6553600);
  FILE *log_txt;
  
  memset(buffer,0,6553600);
  log_txt = fopen("log.txt","w");
  if(!log_txt)
    {
      printf("Can't open log file, don't forget to launch as rooted user\n");
      return -1;
    }
  printf("Packet capturing...\n");
  socket_fd = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
  if(socket_fd<0)
    {
      printf("Socket error, don't forget to launch as rooted user\n");
      return -1;
    }
  while(1)
    {
      addr_len = sizeof(socket_addr);
      bufflen = recvfrom(socket_fd,buffer,65536,0,&socket_addr,(socklen_t *)&addr_len);
      if(bufflen<0)
	{
	  printf("recvfrom error, don't forget to launch as rooted user\n");
	  return -1;
	}
      fflush(log_txt);
      data_process(buffer, bufflen, log_txt);
    }
  close(socket_fd);
}

