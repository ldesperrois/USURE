CC = gcc
# -fPIC est obligatoire partout pour une bibliothèque dynamique.
CFLAGS = -Wall -Wextra -Werror -g -fPIC

# Liste des sources
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

# Nom de la lib (On garde .so qui est le standard Linux, mais géré par macOS)
LIB = lib/libdico.so

# ---------------------------------------------------------
# DÉTECTION DU SYSTÈME D'EXPLOITATION
# ---------------------------------------------------------
# La commande "uname -s" renvoie "Darwin" sur Mac et "Linux" sur Linux.
UNAME_S := $(shell uname -s)

# On prépare les options de compilation de la bibliothèque (SHARED_FLAGS)
ifeq ($(UNAME_S),Darwin)
    # Si on est sur macOS (Darwin), on ajoute la commande Apple spécifique
    SHARED_FLAGS = -shared -install_name @rpath/libdico.so
else
    # Si on est sur Linux, on met juste -shared (le comportement standard)
    SHARED_FLAGS = -shared
endif
# ---------------------------------------------------------

all: $(LIB)

# Création de la bibliothèque dynamique avec les flags adaptés au système
$(LIB): $(OBJ)
	@mkdir -p lib
	$(CC) $(SHARED_FLAGS) -o $@ $(OBJ)

# Compilation des .o 
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) lib/ test_prog *.dSYM

.PHONY: all clean test

# Cible de test (le RPATH configuré ainsi fonctionne sur Linux ET macOS)
test: $(LIB) src/benchmark.c
	$(CC) $(CFLAGS) src/benchmark.c -L./lib -ldico -Wl,-rpath,$(shell pwd)/lib -o test_prog
	./test_prog