# Make file for engima
#
#

CC      = gcc
INSTALL = install
OBJDIR  = ./obj

VERSION=1.0


CFLAGS		= -I. -I./src -I./src/generics -I./src/enigma -I./src/turing -I./src/ioc -I./src/various -I./src/examples -I./src/testframe -I./src/test -Wall -g -O2 -c -DOSA_POSIX 
LDFLAGS		= -Wl,-rpath,/usr/local/lib -lstdc++ -lm
LIBS		= -L/usr/lib -lpthread 

_SRCOBJ	        = enigma.o \
                  enigmaM3.o \
                  enigmaM4.o \
                  walze.o\
                  umkehrwalze.o\
                  steckerbrett.o\
                  turing.o\
                  exampleEnigma.o\
                  enigmachallenge.o \
                  crack02.o \
                  crack03.o \
                  exampleTuring.o \
                  coincidence.o \
                  exampleCoincidence.o \
                  ngramscore.o \
                  adfgx.o \
                  linkedList.o \
                  toolbox.o \
                  log.o \
                  workDispatcher.o

_TESTSRCOBJ     = testframe.o \
                  testToolbox.o \
                  testWorkDispatcher.o \
                  testWalze.o \
                  testUkw.o \
                  testSteckerbrett.o \
                  testEnigma.o \
                  testTuring.o \
                  testCoincidence.o \
                  testNgram.o

_MAINOBJ        = main.o

_TESTOBJ        = test.o 

SRCOBJ          = $(patsubst %,$(OBJDIR)/%,$(_SRCOBJ))
TESTSRCOBJ      = $(patsubst %,$(OBJDIR)/%,$(_TESTSRCOBJ))
MAINOBJ         = $(patsubst %,$(OBJDIR)/%,$(_MAINOBJ))
TESTOBJ         = $(patsubst %,$(OBJDIR)/%,$(_TESTOBJ))

OBJ             = $(MAINOBJ) $(SRCOBJ) $(TESTOBJ)

VPATH = ./src ./src/generics ./src/enigma ./src/turing ./src/ioc ./src/various ./src/examples ./src/testframe ./src/test

all: enigma test

enigma: $(MAINOBJ) $(SRCOBJ) 
	$(CC) $(LDFLAGS) -o $@ $(MAINOBJ) $(SRCOBJ) $(LIBS)

test: $(TESTOBJ) $(SRCOBJ) $(TESTSRCOBJ)
	$(CC) $(LDFLAGS) -o $@ $(TESTOBJ) $(SRCOBJ) $(TESTSRCOBJ) $(LIBS)

$(OBJDIR)/%.o:%.c
	$(CC) $(CFLAGS) -o $@ $<


clean:
	 rm -f $(OBJDIR)/*.o enigma test enigma.exe test.exe
