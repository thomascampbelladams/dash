CC=gcc
SVNVER:= -D'SVNVER="$(shell svnversion -n .)"'
CFLAGS= $(SVNVER) -g -I. -Wall
#CFLAGS= -O  -Wall 
LFLAGS=-lpthread -lreadline -lncurses
JFLAGS=
OBJECTS=List.o Node.o Job.o keyBindings.o command.o shellUtil.o signals.o buddySystem.o

PROGS= dash tl buddy-test buddySystem buddySystem.so

all: dash

dash: dash.o error.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ dash.o error.o $(OBJECTS) $(LFLAGS)
	
buddySystem: buddySystem.o
	$(CC) $(CFLAGS) -fPIC -o $@ buddySystem.o

buddy-test: buddy-test.o buddySystem.o
	$(CC) $(CFLAGS) -o $@ buddy-test.o buddySystem.o

libbuddySystem.so: buddySystem.o
	$(LD) -fPIC -shared -L$(CURDIR) -o $@ buddySystem.o

TestList: TestList.o List.o Node.o Job.o buddySystem.o
	$(CC) $(CFLAGS) -o tl TestList.o List.o Node.o Job.o buddySystem.o

clean:
	/bin/rm -f *.o $(PROGS) a.out core *.class junk log logfile loop test.txt
