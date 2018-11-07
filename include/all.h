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
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <list>
#include <fstream>

extern int iphdrlen;
extern struct sockaddr socket_addr;
extern struct sockaddr_in source;
extern struct sockaddr_in dest;
extern int socket_fd;
extern sf::String str;
extern FILE *tmp_txt;

using namespace std;

int data_process(unsigned char* buffer,int bufflen, FILE *log_txt, sf::RenderWindow &app);
int udp_header(unsigned char* buffer, int bufflen, FILE *log_txt, sf::RenderWindow &app);
int tcp_header(unsigned char* buffer,int bufflen, FILE *log_txt, sf::RenderWindow &app);
int payload(unsigned char* buffer,int bufflen, FILE *log_txt, sf::RenderWindow &app);
int ethernet_header(unsigned char* buffer,int bufflen, FILE *log_txt, sf::RenderWindow &app);
int ip_header(unsigned char* buffer,int bufflen, FILE *log_txt, sf::RenderWindow &app);

#endif
