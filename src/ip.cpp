#include "all.h"

void ip_header(unsigned char* buffer,int bufflen, FILE *log_txt)
{
  struct iphdr *ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));

  iphdrlen = ip->ihl*4;
  memset(&source, 0, sizeof(source));
  source.sin_addr.s_addr = ip->saddr;
  memset(&dest, 0, sizeof(dest));
  dest.sin_addr.s_addr = ip->daddr;
  fprintf(log_txt , "\nIP Header\n\t-Version              : %d\n\t-Internet Header Length  : %d DWORDS or %d Bytes\n\t-Total Length      : %d  Bytes\n\t-Identification    : %d\n\t-Time To Live    : %d\n\t-Protocol     : %d\n\t-Header Checksum   : %d\n\t-Source IP         : %s\n\t-Destination IP    : %s\n", ip->version,ip->ihl,((ip->ihl))*4,ntohs(ip->tot_len),ntohs(ip->id),ip->ttl,ip->protocol,ntohs(ip->check), inet_ntoa(source.sin_addr),inet_ntoa(dest.sin_addr));
}
