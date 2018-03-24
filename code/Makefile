CFLAGS = -std=c++17 -O3 -march=native -Wall -Wextra -pedantic
LIBS =  -ldnest4 -lpthread

default:
	make noexamples -C DNest4/code
	g++ $(CFLAGS) -c *.cpp
	g++ -L DNest4/code -o main *.o $(LIBS)
	rm -f *.o

