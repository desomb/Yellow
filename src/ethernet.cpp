#include "all.h"

using namespace sf;

int ethernet_header(unsigned char* buffer,int bufflen, FILE *log_txt, RenderWindow &app)
{
  struct ethhdr *eth = (struct ethhdr *)(buffer);

  fprintf(log_txt,"\nEthernet Header\n\t-Source Address: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n\t-Destination Address: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_source[0],eth->h_source[1],eth->h_source[2],eth->h_source[3],eth->h_source[4],eth->h_source[5],eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],eth->h_dest[4],eth->h_dest[5]);
  fprintf(tmp_txt,"\nEthernet Header\n\t-Source Address: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n\t-Destination Address: %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_source[0],eth->h_source[1],eth->h_source[2],eth->h_source[3],eth->h_source[4],eth->h_source[5],eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],eth->h_dest[4],eth->h_dest[5]);
}
