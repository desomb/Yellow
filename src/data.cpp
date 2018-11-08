#include "all.h"

using namespace sf;

std::string convert_int(int n)
{
  std::stringstream ss;
  ss << n;
  return ss.str();
}

int data_process(unsigned char* buffer,int bufflen, FILE *log_txt, RenderWindow &app)
{
  struct iphdr *ip = (struct iphdr*)(buffer + sizeof (struct ethhdr));

  if (ip->protocol == 6)
    tcp_header(buffer,bufflen, log_txt, app);
  else if (ip->protocol == 17)
    udp_header(buffer,bufflen, log_txt, app);
}

int payload(unsigned char* buffer,int bufflen, FILE *log_txt, RenderWindow &app)
{
  unsigned char *data = (buffer + iphdrlen  + sizeof(struct ethhdr) + sizeof(struct udphdr));
  int data_left = bufflen - (iphdrlen  + sizeof(struct ethhdr) + sizeof(struct udphdr));

  if (data_txt != NULL)
    fprintf(data_txt,"\nData\n");
  fprintf(log_txt,"\nData\n");
  fprintf(tmp_txt,"\nData\n");
  for(int i = 0; i<data_left; i++)
    {
      if(i != 0 && i%16==0)
	{
	  fprintf(log_txt,"\n");
	  fprintf(tmp_txt,"\n");
	}
      fprintf(log_txt," %.2X ",data[i]);
      fprintf(tmp_txt," %.2X ",data[i]);
  if (data_txt != NULL)
      fprintf(data_txt, " %.2X ", data[i]);
    }
  fprintf(log_txt,"\n-----------------------------------------------------------------\n\n\n");
  fprintf(tmp_txt,"\n-----------------------------------------------------------------\n\n\n");
}
