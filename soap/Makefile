CC = g++

CFLAGS = -Wall -Werror -Ofast

all: main

run: main
	./main

memcheck: main
	valgrind ./main

clean:
	rm *.o main

main: main.o distribution.o job.o dataStore.o stream.o policy.o system.o policyManager.o
	$(CC) $(CFLAGS) -o main main.o distribution.o job.o dataStore.o stream.o policy.o system.o policyManager.o

main.o: distribution.h job.h main.cpp queue_standard.h dataStore.h
	$(CC) $(CFLAGS) -c main.cpp

job.o: job.h job.cpp distribution.h queue_standard.h
	$(CC) $(CFLAGS) -c job.cpp

distribution.o: distribution.h distribution.cpp queue_standard.h
	$(CC) $(CFLAGS) -c distribution.cpp

dataStore.o: dataStore.h dataStore.cpp queue_standard.h job.h distribution.h
	$(CC) $(CFLAGS) -c dataStore.cpp

stream.o: stream.h queue_standard.h stream.cpp distribution.h
	$(CC) $(CFLAGS) -c stream.cpp

policy.o: queue_standard.h policy.h job.h policy.cpp
	$(CC) $(CFLAGS) -c policy.cpp

policyManager.o: policyManager.h job.h policy.h policyManager.cpp
	$(CC) $(CFLAGS) -c policyManager.cpp

system.o: system.h queue_standard.h job.h stream.h policy.h dataStore.h system.cpp
	$(CC) $(CFLAGS) -c system.cpp
