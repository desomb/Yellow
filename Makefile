all	:
	g++ -I./include src/*.cpp -o Yellow -lsfml-graphics -lsfml-window -lsfml-system -g

fclean:
	rm -rf *.txt
	rm -rf Yellow

re:
	rm -rf *.txt
	        g++ -I./include src/*.cpp -o Yellow -lsfml-graphics -lsfml-window -lsfml-system -g
