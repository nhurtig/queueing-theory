CC = g++

CFLAGS = -Wall -Werror -Ofast

all: main

run: main
	./main

memcheck: main
	valgrind ./main

clean:
	rm -f *.o main

main: main.o distribution.o job.o indexedJob.o dataStore.o stream.o policy.o system.o policyManager.o preemptPolicy.o scenarios.o
	$(CC) $(CFLAGS) -o main main.o distribution.o job.o indexedJob.o dataStore.o stream.o policy.o system.o policyManager.o preemptPolicy.o scenarios.o

main.o: distribution.h job.h main.cpp queue_standard.h dataStore.h
	$(CC) $(CFLAGS) -c main.cpp

job.o: job.h job.cpp distribution.h queue_standard.h
	$(CC) $(CFLAGS) -c job.cpp

indexedJob.o: indexedJob.h indexedJob.cpp policy.h job.h
	$(CC) $(CFLAGS) -c indexedJob.cpp

distribution.o: distribution.h distribution.cpp queue_standard.h
	$(CC) $(CFLAGS) -c distribution.cpp

dataStore.o: dataStore.h dataStore.cpp queue_standard.h job.h distribution.h
	$(CC) $(CFLAGS) -c dataStore.cpp

stream.o: stream.h queue_standard.h stream.cpp distribution.h
	$(CC) $(CFLAGS) -c stream.cpp

policy.o: queue_standard.h policy.h job.h policy.cpp
	$(CC) $(CFLAGS) -c policy.cpp

policyManager.o: policyManager.h job.h indexedJob.h policy.h preemptPolicy.h policyManager.cpp
	$(CC) $(CFLAGS) -c policyManager.cpp

preemptPolicy.o: queue_standard.h indexedJob.h job.h preemptPolicy.cpp
	$(CC) $(CFLAGS) -c preemptPolicy.cpp

system.o: system.h queue_standard.h job.h stream.h policy.h dataStore.h system.cpp
	$(CC) $(CFLAGS) -c system.cpp

scenarios.o: scenarios.h queue_standard.h preemptPolicy.h policyManager.h system.h distribution.h scenarios.cpp
	$(CC) $(CFLAGS) -c scenarios.cpp
