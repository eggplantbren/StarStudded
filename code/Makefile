CC = g++

#CFLAGS = -m64 -O3 -flto -march=native -Wall -Wextra -ansi -pedantic
CFLAGS = -m64 -O3 -lto -march=native -mno-avx -Wall -Wextra -ansi -pedantic
LDFLAGS = -I/usr/local/Cellar/boost/1.55.0_2/include -I/usr/local/include/
LLIBFLAGS = -L /usr/local/Cellar/boost/1.55.0_2/
LIBS =  -lrjobject -ldnest3 -lgsl -lgslcblas -lboost_system-mt -lboost_thread-mt

default:
	$(CC) $(CFLAGS) $(LDFLAGS) -c *.cpp
	$(CC) -o main *.o $(LLIBFLAGS) $(LIBS)
	rm -f *.o

