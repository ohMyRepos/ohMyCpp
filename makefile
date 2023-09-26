CC=g++
CFLAG=-w -g -m64
LIBS=-lldap -lpthread -lrt -fno-builtin -llber
CFLAG_DEBG=-g -m64 -L:./

OBJECTS=main.o fork.o io.o thread.o pNc.o philosopher.o shm.o

APPNAME=ohMyCpp

VPATH=./src

${APPNAME}: $(OBJECTS)
	$(CC) $(CFLAG) -o ${APPNAME} $(OBJECTS) $(LIBS)
debug:
	$(CC) $(CFLAG_DEBG) -o ${APPNAME} $(OBJECTS) $(LIBS)

# suffix rule
.cpp.o:
	$(CC) -c $(CFLAG) $< -o $@

# static rule
$(OBJECTS): %.o: %.cpp
	$(CC) -c $(CFLAG) $< -o $@

clean:
	rm -rf *.o *.txt ${APPNAME}
