.phony all:
all: diskinfo disklist

diskinfo: diskinfo.c
	gcc -Wall diskinfo.c -pthread -o diskinfo

disklist: disklist.c
	gcc -Wall disklist.c -pthread -o disklist

.PHONY clean:
clean:
	-rm -rf *.o *.exe
