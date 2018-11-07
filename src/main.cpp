#include "all.h"

using namespace sf;

int iphdrlen;
int socket_fd;
struct sockaddr socket_addr;
struct sockaddr_in source;
struct sockaddr_in dest;
sf::String str;
FILE *tmp_txt;
ifstream tmp;

int main()
{
  int addr_len;
  int bufflen;
  unsigned char *buffer = (unsigned char *)malloc(6553600);
  FILE *log_txt;
  RenderWindow app(VideoMode(1600, 900, 32), "Ma premiere fenetre SFML ! ");
  sf::Text text;
  std::string line;
  std::list<sf::Text> textList;
  sf::Font font;
  int i = 0;
  
  memset(buffer,0,6553600);
  log_txt = fopen("log.txt","w");
  tmp_txt = fopen("tmp.txt","w");
  if(!log_txt || !tmp_txt)
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

  while (app.isOpen())
    {
      Event event;
      tmp_txt = fopen("tmp.txt","w");

      while (app.pollEvent(event))
	{
	  if (event.type == Event::Closed)
	    app.close();
	}

      app.display();
      app.clear();
      //text.setCharacterSize(17);
      
      addr_len = sizeof(socket_addr);
      bufflen = recvfrom(socket_fd,buffer,65536,0,&socket_addr,(socklen_t *)&addr_len);
      if(bufflen<0)
	{
	  printf("recvfrom error, don't forget to launch as rooted user\n");
	  close(socket_fd);
	  return -1;
	}
      fflush(log_txt);
      fflush(tmp_txt);
      data_process(buffer, bufflen, log_txt, app);
      fclose(tmp_txt);

      if (!font.loadFromFile("arial.ttf"))
	{
	  printf("Error loading arial.ttf");
	  close(socket_fd);
	  return (-1);
	}
      text.setFont(font);
      text.setCharacterSize(12);
      
      tmp.open("tmp.txt");
      while (std::getline(tmp, line))
	{
	  text.setString(line);
	  textList.push_back(text);
	}
      tmp.close();
      std::remove("tmp.txt");
	
      for(std::list<sf::Text>::iterator it = textList.begin(); it != textList.end(); ++it)
	{
	  sf::Text& text = *it;
	  sf::FloatRect boundings = text.getLocalBounds();
	  text.setPosition(0.f, 1 + (i * 10));
	  app.draw(text);
	  i++;
	}
      sf::sleep(seconds(0.1f));
      textList.clear();
      i = 0;
      
    }
  close(socket_fd);
  return EXIT_SUCCESS;
}
