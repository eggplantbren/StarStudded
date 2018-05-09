CC = g++
CCFLAGS = -std=c++11 -O3 -march=native -DNDEBUG -Wall -Wextra -pedantic
LIBS =  -ldnest4 -lpthread

default:
	make noexamples -C DNest4/code
	$(CC) $(CCFLAGS) -c *.cpp
	$(CC) -L DNest4/code -o main *.o $(LIBS)
	rm -f *.o

