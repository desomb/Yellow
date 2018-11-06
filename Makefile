all	:
	g++ -I./include src/*.cpp -o Yellow -g

fclean:
	rm -rf *.txt
	rm -rf Yellow

re:
	rm -rf *.txt
	        g++ -I./include src/*.cpp -o Yellow -g
