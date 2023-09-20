CC = g++

CFLAGS = -Wall -Werror -Ofast

all: main

run: main
	./main

memcheck: main
	valgrind ./main

clean:
	rm *.o main

main: main.o distribution.o
	$(CC) $(CFLAGS) -o main main.o distribution.o

main.o: distribution.h main.cpp queue_standard.h
	$(CC) $(CFLAGS) -c main.cpp

distribution.o: distribution.h distribution.cpp queue_standard.h
	$(CC) $(CFLAGS) -c distribution.cpp
