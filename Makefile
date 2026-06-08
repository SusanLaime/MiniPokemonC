CC = gcc
CFLAGS = -Wall -g -I.
SRC = ejecutar.c src/tokens.c src/scannerPokemon.c src/scannerConfig.c \
      src/ll1/gramatica.c src/ll1/first_follow.c src/ll1/select.c src/ll1/arbol.c \
      src/parser.c src/visualizer/tablasGraph.c src/visualizer/arbolGraph.c
OBJ = $(patsubst %.c,obj/%.o,$(notdir $(SRC)))

vpath %.c src:src/ll1:src/visualizer:.

all: MiniPokemonC

MiniPokemonC: $(OBJ)
	$(CC) $(CFLAGS) -o ejecutar $(OBJ)

obj/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f obj/*.o ejecutar
	rm -f images/arboles/* images/tablas/*
