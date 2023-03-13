# Make file for NetSetHost
#
#

CC = gcc
INSTALL = install

prefix = /usr/local
exec_prefix = ${prefix}
bindir = ${exec_prefix}/bin
mandir = ${prefix}/man
sysconfdir = ${prefix}/etc
datadir = ${prefix}/share

VERSION=1.0

docdir = $(datadir)/doc/motion-$(VERSION)
examplesdir = $(docdir)/examples


CFLAGS		= -I. -I./src -Wall -g -O2 -c -DOSA_POSIX 
LDFLAGS		= -Wl,-rpath,/usr/local/lib -lstdc++ -lm
LIBS		= -L/usr/lib -lpthread 

SRCOBJ	        = enigma.o \
                  enigmaM3.o \
                  enigmaM4.o \
                  walze.o\
                  umkehrwalze.o\
                  steckerbrett.o\
                  turing.o\
                  exampleEnigma.o\
                  exampleTuring.o \
                  coincidence.o \
                  exampleCoincidence.o \
                  ngramscore.o \
                  adfgx.o \
                  toolbox.o \
                  workDispatcher.o

MAINOBJ         = main.o crack01.o crack02.o crack03.o

TESTOBJ         = test.o

OBJ             = $(MAINOBJ) $(SRCOBJ) $(TESTOBJ)

VPATH = ./src

all: enigma test

enigma: $(MAINOBJ) $(SRCOBJ) 
	$(CC) $(LDFLAGS) -o $@ $(MAINOBJ) $(SRCOBJ) $(LIBS)

test: $(TESTOBJ) $(SRCOBJ)
	$(CC) $(LDFLAGS) -o $@ $(TESTOBJ) $(SRCOBJ) $(LIBS)

$(OBJ):%.o:%.c
	$(CC) $(CFLAGS) $<



install:


clean:
	 rm -f *.o enigma test enigma.exe test.exe
