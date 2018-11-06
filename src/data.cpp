#include "all.h"

void data_process(unsigned char* buffer,int bufflen, FILE *log_txt)
{
  struct iphdr *ip = (struct iphdr*)(buffer + sizeof (struct ethhdr));

  if (ip->protocol == 6)
    tcp_header(buffer,bufflen, log_txt);
  else if (ip->protocol == 17)
    udp_header(buffer,bufflen, log_txt);
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
  fprintf(log_txt,"-----------------------------------------------------------------\n\n\n");
}
