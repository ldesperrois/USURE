CC = gcc
# Notez le -fPIC ici ! Obligatoire pour une bibliothèque dynamique.
CFLAGS = -Wall -Wextra -Werror -g -fPIC

# Liste des sources
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

# Nom de la lib (Doit commencer par 'lib' et finir par '.so')
LIB = lib/libdico.so

all: $(LIB)

# Création de la bibliothèque dynamique (Shared Object .so)
# Notez le flag -shared ici !
$(LIB): $(OBJ)
	@mkdir -p lib
	$(CC) -shared -o $@ $(OBJ)

# Compilation des .o (La règle est identique, mais CFLAGS contient -fPIC)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(LIB)
	rm -rf $(OBJ) $(LIB) test_prog *.dSYM

.PHONY: all clean

test: $(LIB) src/benchmark.c
	$(CC) $(CFLAGS) src/benchmark.c -L./lib -ldico -o test_prog
	./test_prog