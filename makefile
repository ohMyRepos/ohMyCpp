CC=g++
CFLAG=-g -m64
LIBS=-lldap -lpthread -lrt -fno-builtin -llber
CFLAG_DEBG=-g -m64 -L:./

OBJS=main.o

ALL: $(OBJS)
	$(CC) $(CFLAG) -o ohMyCpp $(OBJS) $(LIBS)

debug:
	$(CC) $(CFLAG_DEBG) -o ohMyCpp $(OBJS) $(LIBS)
clean:
	rm -rf *.o

.cpp.o:
	$(CC) $(CFLAG) -c $<