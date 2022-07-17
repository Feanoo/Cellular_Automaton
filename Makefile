CC=gcc
CFLAGS=-g -Wall -Werror
LD=-lSDL2 -lSDL2main -lSDL2_ttf -lSDL2_image -lm
BINPATH=bin
SRCPATH=src

build: $(BINPATH) $(BINPATH)/cellular_automaton

run:
	make build
	./$(BINPATH)/cellular_automaton

$(BINPATH):
	mkdir $(BINPATH)

$(BINPATH)/cellular_automaton: $(BINPATH)/main.o 
	$(CC) $(BINPATH)/*.o -o $(BINPATH)/cellular_automaton ${LD}

$(BINPATH)/main.o: $(SRCPATH)/main.* $(BINPATH)/automaton.o
	$(CC) -c ${CFLAGS} $(SRCPATH)/main.c -o $(BINPATH)/main.o

$(BINPATH)/automaton.o: $(SRCPATH)/automaton.* $(BINPATH)/draw.o $(BINPATH)/particles.o
	$(CC) -c ${CFLAGS} $(SRCPATH)/automaton.c -o $(BINPATH)/automaton.o

$(BINPATH)/draw.o: $(SRCPATH)/draw.* $(BINPATH)/particles.o
	$(CC) -c ${CFLAGS} $(SRCPATH)/draw.c -o $(BINPATH)/draw.o

$(BINPATH)/particles.o: $(SRCPATH)/particles.*
	$(CC) -c ${CFLAGS} $(SRCPATH)/particles.c -o $(BINPATH)/particles.o

clean:
	rm -r $(BINPATH)