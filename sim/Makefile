CC = g++

CFLAGS = -Wall -Werror -Ofast

all: main

run: main
	./main

memcheck: main
	valgrind ./main

clean:
	rm *.o main

main: main.o distribution.o job.o
	$(CC) $(CFLAGS) -o main main.o distribution.o job.o

main.o: distribution.h job.h main.cpp queue_standard.h
	$(CC) $(CFLAGS) -c main.cpp

job.o: job.h job.cpp distribution.h queue_standard.h
	$(CC) $(CFLAGS) -c job.cpp

distribution.o: distribution.h distribution.cpp queue_standard.h
	$(CC) $(CFLAGS) -c distribution.cpp
