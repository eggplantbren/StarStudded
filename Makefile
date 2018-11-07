CC = g++
CCFLAGS = -std=c++11 -O3 -march=native -DNDEBUG -Wall -Wextra -pedantic -I include -I .
LIBS =  -ldnest4 -lpthread -larmadillo -lyaml-cpp

default:
	make noexamples -C DNest4/code
	$(CC) $(CCFLAGS) -c src/*.cpp
	$(CC) $(CCFLAGS) -c CorrelatedNoise/NoiseModel.cpp
	$(CC) -L DNest4/code -o StarStudded *.o $(LIBS)
	rm -f *.o

