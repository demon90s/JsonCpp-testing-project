all: main

CC=g++ -g -Wall
OBJS=main.o JsonReader.o

main: $(OBJS)
	$(CC) -o $@ $(OBJS) ./lib/libjsoncpp.a

main.o: main.cpp
	$(CC) -I./include -o main.o -c main.cpp

JsonReader.o: JsonReader.hpp JsonReader.cpp
	$(CC) -I./include -o JsonReader.o -c JsonReader.cpp

clean:
	-rm -f *.o main
