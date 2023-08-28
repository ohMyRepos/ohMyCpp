CC=g++
CFLAG=-w -g -m64
LIBS=-lldap -lpthread -lrt -fno-builtin -llber
CFLAG_DEBG=-g -m64 -L:./

OBJS=main.o fork.o io.o thread.o

ALL: $(OBJS)
	$(CC) $(CFLAG) -o ohMyCpp $(OBJS) $(LIBS)
debug:
	$(CC) $(CFLAG_DEBG) -o ohMyCpp $(OBJS) $(LIBS)

clean:
	rm -rf *.o *.txt

.cpp.o:
	$(CC) $(CFLAG) -c $<