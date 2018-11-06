#include "all.h"

int iphdrlen;
struct sockaddr socket_addr;
struct sockaddr_in source;
struct sockaddr_in dest;

/*
void ethernet_header(unsigned char* buffer,int bufflen, FILE *log_txt)
{
  struct ethhdr *eth = (struct ethhdr *)(buffer);

  fprintf(log_txt,"\nEthernet Header\n\t|-Source Address: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n\t|-Destination Address: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n\t|-Protocol: %d\n",eth->h_source[0],eth->h_source[1],eth->h_source[2],eth->h_source[3],eth->h_source[4],eth->h_source[5],eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],eth->h_dest[4],eth->h_dest[5],eth->h_proto);
}

void ip_header(unsigned char* buffer,int bufflen, FILE *log_txt)
{
  struct iphdr *ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));

  iphdrlen = ip->ihl*4;
  memset(&source, 0, sizeof(source));
  source.sin_addr.s_addr = ip->saddr;
  memset(&dest, 0, sizeof(dest));
  dest.sin_addr.s_addr = ip->daddr;
  fprintf(log_txt , "\nIP Header\n\t|-Version              : %d\n\t|-Internet Header Length  : %d DWORDS or %d Bytes\n\t|-Type Of Service   : %d\n\t|-Total Length      : %d  Bytes\n\t|-Identification    : %d\n\t|-Time To Live    : %d\n\t|-Protocol     : %d\n\t|-Header Checksum   : %d\n\t|-Source IP         : %s\n\t|-Destination IP    : %s\n", ip->version,ip->ihl,((ip->ihl))*4,ip->tos,ntohs(ip->tot_len),ntohs(ip->id),ip->ttl,ip->protocol,ntohs(ip->check), inet_ntoa(source.sin_addr),inet_ntoa(dest.sin_addr));
  
}

void payload(unsigned char* buffer,int bufflen, FILE *log_txt)
{
  unsigned char *data = (buffer + iphdrlen  + sizeof(struct ethhdr) + sizeof(struct udphdr));
  int data_left = bufflen - (iphdrlen  + sizeof(struct ethhdr) + sizeof(struct udphdr));

  fprintf(log_txt,"\nData\n");
  for(int i = 0; i<data_left; i++)
    {
      if(i != 0 && i%16==0)
	fprintf(log_txt,"\n");
      fprintf(log_txt," %.2X ",data[i]);
    }
  fprintf(log_txt,"\n");
}

void tcp_header(unsigned char* buffer,int bufflen, FILE *log_txt)
{
  struct tcphdr *tcp = (struct tcphdr*)(buffer + iphdrlen + sizeof(struct ethhdr));

  fprintf(log_txt,"\n*************************TCP Packet******************************");
  ethernet_header(buffer,bufflen, log_txt);
  ip_header(buffer,bufflen, log_txt);
  fprintf(log_txt , "\nTCP Header\n\t|-Source Port          : %u\n\t|-Destination Port     : %u\n\t|-Sequence Number      : %u\n\t|-Acknowledge Number   : %u\n\t|-Header Length        : %d DWORDS or %d BYTES\n\t|----------Flags-----------\n\t\t|-Urgent Flag          : %d\n\t\t|-Acknowledgement Flag : %d\n\t\t|-Push Flag            : %d\n\t\t|-Reset Flag           : %d\n\t\t|-Synchronise Flag     : %d\n\t\t|-Finish Flag          : %d\n\t|-Window size          : %d\n\t|-Checksum             : %d\n\t|-Urgent Pointer       : %d\n", ntohs(tcp->source),ntohs(tcp->dest),ntohl(tcp->seq),ntohl(tcp->ack_seq),tcp->doff,tcp->doff*4,tcp->urg,tcp->ack,tcp->psh,tcp->rst,tcp->syn,tcp->fin,ntohs(tcp->window),ntohs(tcp->check),tcp->urg_ptr);
 
  payload(buffer,bufflen, log_txt);
fprintf(log_txt,"*****************************************************************\n\n\n");
}

void udp_header(unsigned char* buffer, int bufflen, FILE *log_txt)
{
  struct udphdr *udp = (struct udphdr*)(buffer + iphdrlen + sizeof(struct ethhdr));

  ethernet_header(buffer,bufflen, log_txt);
  ip_header(buffer,bufflen, log_txt);
  fprintf(log_txt,"\n*************************UDP Packet******************************\nUDP Header\n\t|-Source Port    : %d\n\t|-Destination Port: %d\n\t|-UDP Length      : %d\n\t|-UDP Checksum   : %d\n", ntohs(udp->source), ntohs(udp->dest), ntohs(udp->len), ntohs(udp->check));
  payload(buffer,bufflen, log_txt);
  fprintf(log_txt,"*****************************************************************\n\n\n");
}

void data_process(unsigned char* buffer,int bufflen, FILE *log_txt)
{
  struct iphdr *ip = (struct iphdr*)(buffer + sizeof (struct ethhdr));

  if (ip->protocol == 6)
    tcp_header(buffer,bufflen, log_txt);
  else if (ip->protocol == 17)
    udp_header(buffer,bufflen, log_txt);
}
*/

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

