#include "all.h"

void udp_header(unsigned char* buffer, int bufflen, FILE *log_txt)
{
  struct udphdr *udp = (struct udphdr*)(buffer + iphdrlen + sizeof(struct ethhdr));

  fprintf(log_txt,"\t\t\tUDP Packet");
  ethernet_header(buffer,bufflen, log_txt);
  ip_header(buffer,bufflen, log_txt);
  fprintf(log_txt,"\nUDP Header\n\t-Source Port    : %d\n\t-Destination Port: %d\n\t-UDP Length      : %d\n\t-UDP Checksum   : %d\n", ntohs(udp->source), ntohs(udp->dest), ntohs(udp->len), ntohs(udp->check));
  payload(buffer,bufflen, log_txt);
}
