#
#FILE
#AUTHOR/LEGAL
#NOTES
#

GIT_VERSION := $(shell git describe --dirty --always --tags)
CFLAGS+=-Wall -DGitVersion=\"$(GIT_VERSION)\"
#LDFLAGS+=`curl-config --libs`

TARGETS=dockprox
OBJS=buf.o log.o json.o jsmn.o template.o

all: $(TARGETS) .gitignore

clean:
	$(RM) $(TARGETS)
	$(RM) $(OBJS)

dockprox: $(OBJS) dockprox.o
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: all clean
