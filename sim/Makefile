CC = g++

CFLAGS = -Wall -Werror -Ofast

all: main

run: main
	./main

memcheck: main
	valgrind ./main

clean:
	rm *.o main

main: main.o distribution.o job.o dataStore.o
	$(CC) $(CFLAGS) -o main main.o distribution.o job.o dataStore.o

main.o: distribution.h job.h main.cpp queue_standard.h dataStore.h
	$(CC) $(CFLAGS) -c main.cpp

job.o: job.h job.cpp distribution.h queue_standard.h
	$(CC) $(CFLAGS) -c job.cpp

distribution.o: distribution.h distribution.cpp queue_standard.h
	$(CC) $(CFLAGS) -c distribution.cpp

dataStore.o: dataStore.h dataStore.cpp queue_standard.h job.h distribution.h
	$(CC) $(CFLAGS) -c dataStore.cpp
