.phony all:
all: ACS

ACS: main.c queue.c
	gcc -Wall main.c queue.c -pthread -o ACS

.PHONY clean:
clean:
	-rm -rf *.o *.exe
