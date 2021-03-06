#include "all.h"

using namespace sf;

int tcp_header(unsigned char* buffer,int bufflen, FILE *log_txt, RenderWindow &app)
{
  struct tcphdr *tcp = (struct tcphdr*)(buffer + iphdrlen + sizeof(struct ethhdr));

  
  fprintf(log_txt,"\t\t\tTCP Packet");
  fprintf(tmp_txt,"\t\t\tTCP Packet");
  
  
  ethernet_header(buffer,bufflen, log_txt, app);
  ip_header(buffer,bufflen, log_txt, app);
  fprintf(log_txt , "\nTCP Header\n\t-Source Port          : %u\n\t-Destination Port     : %u\n\t-Sequence Number      : %u\n\t-Acknowledge Number   : %u\n\t-Header Length        : %d DWORDS or %d BYTES\n\t----------Flags-----------\n\t\t-Urgent Flag          : %d\n\t\t-Acknowledgement Flag : %d\n\t\t-Push Flag            : %d\n\t\t-Reset Flag           : %d\n\t\t-Synchronise Flag     : %d\n\t\t-Finish Flag          : %d\n\t-Window size          : %d\n\t-Checksum             : %d\n\t-Urgent Pointer       : %d\n", ntohs(tcp->source),ntohs(tcp->dest),ntohl(tcp->seq),ntohl(tcp->ack_seq),tcp->doff,tcp->doff*4,tcp->urg,tcp->ack,tcp->psh,tcp->rst,tcp->syn,tcp->fin,ntohs(tcp->window),ntohs(tcp->check),tcp->urg_ptr);
  fprintf(tmp_txt , "\nTCP Header\n\t-Source Port          : %u\n\t-Destination Port     : %u\n\t-Sequence Number      : %u\n\t-Acknowledge Number   : %u\n\t-Header Length        : %d DWORDS or %d BYTES\n\t----------Flags-----------\n\t\t-Urgent Flag          : %d\n\t\t-Acknowledgement Flag : %d\n\t\t-Push Flag            : %d\n\t\t-Reset Flag           : %d\n\t\t-Synchronise Flag     : %d\n\t\t-Finish Flag          : %d\n\t-Window size          : %d\n\t-Checksum             : %d\n\t-Urgent Pointer       : %d\n", ntohs(tcp->source),ntohs(tcp->dest),ntohl(tcp->seq),ntohl(tcp->ack_seq),tcp->doff,tcp->doff*4,tcp->urg,tcp->ack,tcp->psh,tcp->rst,tcp->syn,tcp->fin,ntohs(tcp->window),ntohs(tcp->check),tcp->urg_ptr);
  payload(buffer,bufflen, log_txt, app);
}
