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

int howToUse(RenderWindow &app)
{
  text.setString("To start capturing, press \"C\", and to stop it, repress \"C\"\nTo load packet from a file, use \"L\"\nTo pause a Packet and display its content, press \"P\"\nTo display the data content of packet, press D when it's paused");
  text.setPosition(0.f, 1);
  text.setCharacterSize(18);
  app.draw(text);
}

int drawError(RenderWindow &app, int error)
{

  if (error == 0)
    text.setString("\n\n\n\n\n\n\n\n\nError, capture didn't start, press C to capture before pausing to display its content");
  if (error == 1)
    text.setString("\n\n\n\n\n\n\n\n\nError, capture didn't start, press C to capture, then press P to display its content, then you can display it's Data");
    
  text.setPosition(500.f, 300);
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
  std::string line;
  std::list<sf::Text> textList;
  std::list<sf::Text> dataList;
  int i = 0;
  int captureStarted = 0;
  int capturePaused = 0;
  int captureData = 0;
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

  memset(buffer,0,6553600);
  log_txt = fopen("log.txt","w");
  tmp_txt = fopen("tmp.txt","w");
  data_txt = fopen("data.txt","w");
  if(!log_txt || !tmp_txt || !data_txt)
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
	      if (event.key.code == sf::Keyboard::C && capturePaused != 1 && captureData != 1)
		{
		  error = -1;
		  if (captureStarted == 0)
		    captureStarted = 1;
		  else
		    captureStarted = 0;
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
	      //Display
	      if (event.key.code == sf::Keyboard::D && captureStarted != 1)
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
	      //load from file
	      if (event.key.code == sf::Keyboard::L)
		;
	      
	    }	      
	}


      if (capturePaused == 0)
	{
	  std::cout << "test01" << std::endl;
	  tmp_txt = fopen("tmp.txt","w");
	  nt = 0;
	  std::cout << "test02" << std::endl;
	  if(!tmp_txt)
	    {
	      printf("Can't open log file\n");
	      return -1;
	    }
	  std::cout << "test3" << std::endl;
	}
      if (captureData == 0)
	{
	  std::cout << "test04" << std::endl;

	  data_txt = fopen("data.txt","w");
	  nd = 0;
	  std::cout << "test05" << std::endl;

	  if(!data_txt)
	    {
	      printf("Can't open log file : %d\n", errno);
	      return -1;
	    }
	  std::cout << "test06" << std::endl;

	}

      
      std::cout << "test2" << std::endl;
      
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
      fflush(data_txt);
      //  sf::Text text;

      std::cout << "test3" << std::endl;
      //Howtouse & errors
      if (captureStarted == 0)
	howToUse(app);
      if (error != -1)
	drawError(app, error);
      
      std::cout << "test4" << std::endl;
      if (captureData == 1)
	{
	  textList.clear();
	  data.open("data.txt");
	  text.setString(packetData);
	  text.setCharacterSize(12);	  
	  while (std::getline(data, line))
	    {
	      text.setString(line);
	      textList.push_back(text);
	    }
	  std::cout << i << std::endl;
	  for(std::list<sf::Text>::iterator it = textList.begin(); it != textList.end(); ++it)
	    {
	      sf::Text& text = *it;
	      sf::FloatRect boundings = text.getLocalBounds();
	      text.setPosition(0.f, 1 + (i * 10));
	      app.draw(text);
	      i++;
	    }
	  
	  app.draw(text);
	  captureData = 0;
	  data.close();
	}

      std::cout << "test5" << std::endl;      
      if (captureStarted == 1 && capturePaused == 0)
	{
	  std::cout << "test6" << std::endl;
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
	
	  std::cout << "test7" << std::endl;      
	  for(std::list<sf::Text>::iterator it = textList.begin(); it != textList.end(); ++it)
	    {
	      sf::Text& text = *it;
	      sf::FloatRect boundings = text.getLocalBounds();
	      text.setPosition(0.f, 1 + (i * 10));
	      app.draw(text);
	      i++;
	    }

	  std::cout << "test8" << std::endl;      	  
	  sf::sleep(seconds(0.17f));
	  std::cout << "test9" << std::endl;
	}
      std::cout << "test10" << std::endl;

      if (tmp_txt != NULL)
	fclose(tmp_txt);
      std::cout << "test11" << std::endl;
      if (data_txt != NULL)
	fclose(data_txt);
      tmp_txt = NULL;
      data_txt = NULL;

      std::cout << "test12" << std::endl;      

      
    }
  std::remove("tmp.txt");
  std::remove("data.txt");
  close(socket_fd);
  return EXIT_SUCCESS;
}
