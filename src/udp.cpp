#include "all.h"

using namespace sf;

int udp_header(unsigned char* buffer, int bufflen, FILE *log_txt, RenderWindow &app)
{
  struct udphdr *udp = (struct udphdr*)(buffer + iphdrlen + sizeof(struct ethhdr));

  std::cout << "UDPPPPPPPPPPPPPPPPPPPPPPPPPPPP" << std::endl;
  fprintf(log_txt,"\t\t\tUDP Packet");
  fprintf(tmp_txt,"\t\t\tUDP Packet");
  ethernet_header(buffer,bufflen, log_txt, app);
  ip_header(buffer,bufflen, log_txt, app);
  fprintf(log_txt,"\nUDP Header\n\t-Source Port    : %d\n\t-Destination Port: %d\n\t-UDP Length      : %d\n\t-UDP Checksum   : %d\n", ntohs(udp->source), ntohs(udp->dest), ntohs(udp->len), ntohs(udp->check));
  fprintf(tmp_txt,"\nUDP Header\n\t-Source Port    : %d\n\t-Destination Port: %d\n\t-UDP Length      : %d\n\t-UDP Checksum   : %d\n", ntohs(udp->source), ntohs(udp->dest), ntohs(udp->len), ntohs(udp->check));
  payload(buffer,bufflen, log_txt, app);
}
