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
std::string packetData;

int howToUse(RenderWindow &app)
{
  sf::Text text;
  sf::Font font;

  text.setString("To start capturing, press \"C\", and to stop it, repress \"C\"\nTo load packet from a file, use \"L\"\nTo pause a Packet and display its content, press \"P\"\nTo display the data content of packet, press D when it's paused");
  text.setPosition(0.f, 1);
  if (!font.loadFromFile("arial.ttf"))
    {
      printf("Error loading arial.ttf");
      close(socket_fd);
      return (-1);
    }
  text.setFont(font);
  text.setCharacterSize(18);
  app.draw(text);
}

int drawError(RenderWindow &app, int error)
{
  sf::Text text;
  sf::Font font;

  if (error == 0)
    text.setString("\n\n\n\n\n\n\n\n\nError, capture didn't start, press C to capture before pausing to display its content");
  if (error == 1)
    text.setString("\n\n\n\n\n\n\n\n\nError, capture didn't start, press C to capture, then press P to display its content, then you can display it's Data");
    
  text.setPosition(500.f, 300);
  if (!font.loadFromFile("arial.ttf"))
    {
      printf("Error loading arial.ttf");
      close(socket_fd);
      return (-1);
    }
  text.setFont(font);
  text.setCharacterSize(18);
  app.draw(text);
}

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
  int captureStarted = 0;
  int capturePaused = 0;
  int captureData = 0;
  int error = -1;
  
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



      //Gestion events
      while (app.pollEvent(event))
	{
	  if (event.type == Event::Closed)
	    app.close();

	  if (event.type == Event::KeyPressed)
	    {
	      if (event.key.code == sf::Keyboard::C)
		{
		  error = -1;
		  if (captureStarted == 0)
		    captureStarted = 1;
		  else
		    captureStarted = 0;
		}
	      if (event.key.code == sf::Keyboard::P)
		{
		  if (captureStarted == 1)
		    {
		      if (capturePaused == 0)
			capturePaused = 1;
		      else
			capturePaused = 0;
		    }
		  else
		    error = 0;
		}
	      if (event.key.code == sf::Keyboard::D)
		{
		  if (capturePaused == 1)
		    {
		      if (captureData == 0)
			captureData = 1;
		      else
			captureData = 0;
		    }
		  else
		    error = 1;
		}
	      
	    }	      
	}


      
      app.display();
      if (capturePaused == 0)
	app.clear();
      //text.setCharacterSize(17);
      
      addr_len = sizeof(socket_addr);
      bufflen = recvfrom(socket_fd,buffer,65536,0,&socket_addr,(socklen_t *)&addr_len);
      if( bufflen < 0)
	{
	  printf("recvfrom error, don't forget to launch as rooted user\n");
	  close(socket_fd);
	  return (-1);
	}
      fflush(log_txt);
      fflush(tmp_txt);
      //  sf::Text text;

      //Howtouse & errors
      if (captureStarted == 0)
	howToUse(app);
      if (error != -1)
	drawError(app, error);
      
      if (captureStarted == 1 && capturePaused == 0)
	{
	  textList.clear();
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
	
	  for(std::list<sf::Text>::iterator it = textList.begin(); it != textList.end(); ++it)
	    {
	      sf::Text& text = *it;
	      sf::FloatRect boundings = text.getLocalBounds();
	      text.setPosition(0.f, 1 + (i * 10));
	      app.draw(text);
	      i++;
	    }
	  
	  sf::sleep(seconds(0.17f));
	  i = 0;
	}


      
    }
  std::remove("tmp.txt");
  close(socket_fd);
  return EXIT_SUCCESS;
}
