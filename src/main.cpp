#include "all.h"

using namespace sf;

int iphdrlen;
int socket_fd;
struct sockaddr socket_addr;
struct sockaddr_in source;
struct sockaddr_in dest;
sf::String str;
FILE *tmp_txt;
FILE *data_txt;
ifstream tmp;
ifstream data;
std::string packetData;
sf::Font font;
sf::Text text;

void howToUse(RenderWindow &app)
{
  text.setString("To start capturing, press \"C\", and to stop it, repress \"C\"\nTo load packet from a file, use \"L\"\tThe file must be named log.txt\nTo pause a Packet and display its content, press \"P\"\nTo display the data content of packet, press D when it's paused");
  text.setPosition(0.f, 1);
  text.setCharacterSize(18);
  app.draw(text);
}

void drawError(RenderWindow &app, int error)
{

  if (error == 0)
    text.setString("\n\n\n\n\n\n\n\n\nError, capture didn't start, press C to capture before pausing to display its content");
  if (error == 1)
    text.setString("\n\n\n\n\n\n\n\n\nError, capture didn't start, press C to capture, then press P to display its content, then you can display it's Data");
    
  text.setPosition(500.f, 300);
  text.setCharacterSize(18);
  app.draw(text);
}

int dispFile(FILE *log_txt, RenderWindow &app)
{
  int i = 0;
  ifstream log;
  std::string line;
  std::list<sf::Text> textList;

  app.clear();
  //  fclose(log_txt);
  log.open("log.txt");
  text.setCharacterSize(12);
  while (std::getline(log, line))
    {
      text.setString(line);
      textList.push_back(text);
    }
  log.close();
	
  for(std::list<sf::Text>::iterator it = textList.begin(); it != textList.end(); ++it)
    {
      sf::Text& text = *it;
      sf::FloatRect boundings = text.getLocalBounds();
      text.setPosition(0.f, 1 + (i * 10));
      app.draw(text);
      i++;
    }

}

int main()
{
  int addr_len;
  int bufflen;
  unsigned char *buffer = (unsigned char *)malloc(65536);
  FILE *log_txt;
  RenderWindow app(VideoMode(1600, 900, 32), "Ma premiere fenetre SFML ! ");
  std::string line;
  std::list<sf::Text> textList;
  std::list<sf::Text> dataList;
  int i = 0;
  int captureStarted = 0;
  int capturePaused = 0;
  int captureData = 0;
  int captureFromF = 0;
  int error = -1;  
  int nt = 0;
  int nd = 0;

  log_txt = NULL;
  data_txt = NULL;
  tmp_txt = NULL;
  
  if (!font.loadFromFile("arial.ttf"))
    {
      printf("Error loading arial.ttf");
      return (-1);
    }
  text.setFont(font);

  memset(buffer,0,65536);
  tmp_txt = fopen("tmp.txt","w");
  data_txt = fopen("data.txt","w");
  if(!tmp_txt || !data_txt)
    {
      printf("Can't open log file\n");
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
      std::cout << "test0" << std::endl;
      Event event;
      



      std::cout << "test1" << std::endl;
      //Gestion events
      while (app.pollEvent(event))
	{
	  if (event.type == Event::Closed)
	    app.close();

	  if (event.type == Event::KeyPressed)
	    {
	      //capture
	      if (event.key.code == sf::Keyboard::C && capturePaused != 1 && captureFromF != 1)
		{

		  error = -1;
		  if (captureStarted == 0)
		    {
		      log_txt = fopen("log.txt","w");
		      if (!log_txt)
			{
			  printf("Can't open log file\n");
			  return -1;
			}
		      captureStarted = 1;
		    }
		  else
		    {
		      fclose(log_txt);
		      captureStarted = 0;
		    }
		}
	      //pause
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
	      if (event.key.code == sf::Keyboard::L && capturePaused == 0 && captureStarted == 0)
		{
		  if (captureFromF == 0)
		    captureFromF = 1;
		  else
		    captureFromF = 0;
		  if (dispFile(log_txt, app) == -1)
		    {
		      close(socket_fd);
		      return (-1);
		    }
		}
	      
	    }	      
	}


      if (capturePaused == 0)
	{
	  tmp_txt = fopen("tmp.txt","w");
	  nt = 0;
	  if(!tmp_txt)
	    {
	      printf("Can't open log file\n");
	      return -1;
	    }
	}
      if (captureData == 0)
	{

	  data_txt = fopen("data.txt","w");
	  nd = 0;

	  if(!data_txt)
	    {
	      printf("Can't open log file : %d\n", errno);
	      return -1;
	    }

	}

      
      
      app.display();
      if (capturePaused == 0 && captureFromF != 1)
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
      //  sf::Text text;

      //Howtouse & errors
      if (captureStarted == 0 && captureFromF != 1)
	howToUse(app);
      if (error != -1 && captureFromF != 1)
	drawError(app, error);
      

      if (captureStarted == 1 && capturePaused == 0)
	{
	  textList.clear();
	  i = 0;
	  data_process(buffer, bufflen, log_txt, app);
	  if (tmp_txt != NULL)
	    fclose(tmp_txt);
	  if (data_txt != NULL)
	    fclose(data_txt);
	  tmp_txt = NULL;
	  data_txt = NULL;
	  text.setCharacterSize(12);
	  tmp.open("tmp.txt");
	  while (std::getline(tmp, line))
	    {
	      text.setString(line);
	      textList.push_back(text);
	    }
	  tmp.close();
	
	  if (textList.empty())
	    textList = dataList;
	  for(std::list<sf::Text>::iterator it = textList.begin(); it != textList.end(); ++it)
	    {
	      sf::Text& text = *it;
	      sf::FloatRect boundings = text.getLocalBounds();
	      text.setPosition(0.f, 1 + (i * 10));
	      app.draw(text);
	      i++;
	    }

	  dataList = textList;
	  sf::sleep(seconds(0.05f));
	}

      if (tmp_txt != NULL)
	fclose(tmp_txt);
      if (data_txt != NULL)
	fclose(data_txt);
      tmp_txt = NULL;
      data_txt = NULL;

    }
  
  std::remove("tmp.txt");
  std::remove("data.txt");
  close(socket_fd);
  return EXIT_SUCCESS;
}
