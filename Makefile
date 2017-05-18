OBJS = main.cpp network.o functions.o test.o
CC = clang++
CFLAGS = -std=c++11 -Ofast -march=native -c
LFLAGS = -std=c++11 -Ofast -march=native

Build : $(OBJS)
	$(CC) $(LFLAGS) -o test/Build $(OBJS)

network.o : network.h network.cpp functions.h
	$(CC) $(CFLAGS) network.cpp -o network.o

functions.o : functions.h functions.cpp
	$(CC) $(CFLAGS) functions.cpp -o functions.o

test.o : test.h test.cpp network.h
	$(CC) $(CFLAGS) test.cpp -o test.o
