#ifndef ALL_H_
#define ALL_H_

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_packet.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>

extern int iphdrlen;
extern struct sockaddr socket_addr;
extern struct sockaddr_in source;
extern struct sockaddr_in dest;


void data_process(unsigned char* buffer,int bufflen, FILE *log_txt);
void udp_header(unsigned char* buffer, int bufflen, FILE *log_txt);
void tcp_header(unsigned char* buffer,int bufflen, FILE *log_txt);
void payload(unsigned char* buffer,int bufflen, FILE *log_txt);
void ethernet_header(unsigned char* buffer,int bufflen, FILE *log_txt);
void ip_header(unsigned char* buffer,int bufflen, FILE *log_txt);

#endif
