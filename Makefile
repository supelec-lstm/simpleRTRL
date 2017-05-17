OBJS = main.cc network.o functions.o test.o
CC = clang++
CFLAGS = -std=c++11 -Ofast -c
LFLAGS = -std=c++11 -Ofast

Build : $(OBJS)
	$(CC) $(LFLAGS) -o Build $(OBJS)

network.o : network.hh network.cc functions.hh
	$(CC) $(CFLAGS) network.cc -o network.o

functions.o : functions.hh functions.cc
	$(CC) $(CFLAGS) functions.cc -o functions.o

test.o : test.hh test.cc network.hh
	$(CC) $(CFLAGS) test.cc -o test.o
