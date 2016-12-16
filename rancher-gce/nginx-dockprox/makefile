#
#FILE
#AUTHOR/LEGAL
#NOTES
#

#GIT_VERSION := $(shell git describe --dirty --always --tags)

CFLAGS=-Wall -DGitVersion=\"$(GIT_VERSION)\"
LIBS= ./cJSON/libcjson.a -lm

nginx-dockprox: main.o
	cc main.o -o nginx-dockprox $(LIBS) 

main.o: main.c
	cc -c main.c -o main.o $(CFLAGS)

clean:
	rm -f *.o nginx-dockprox
