CC=gcc
CFLAGS=-g -Wall -Werror
LD=-lSDL2 -lSDL2main -lSDL2_ttf -lSDL2_image -lm
BINPATH=bin
SRCPATH=src

build: $(BINPATH) $(BINPATH)/cellular_automaton

$(BINPATH):
	mkdir $(BINPATH)

$(BINPATH)/cellular_automaton: $(BINPATH)/main.o 
	$(CC) $(BINPATH)/*.o -o $(BINPATH)/cellular_automaton ${LD}

$(BINPATH)/main.o: $(SRCPATH)/main.* $(BINPATH)/automaton.o
	$(CC) -c ${CFLAGS} $(SRCPATH)/main.c -o $(BINPATH)/main.o

$(BINPATH)/automaton.o: $(SRCPATH)/automaton.* $(BINPATH)/draw.o
	$(CC) -c ${CFLAGS} $(SRCPATH)/automaton.c -o $(BINPATH)/automaton.o

$(BINPATH)/draw.o: $(SRCPATH)/draw.*
	$(CC) -c ${CFLAGS} $(SRCPATH)/draw.c -o $(BINPATH)/draw.o

clean:
	rm -r $(BINPATH)