# nom du compilateurs
CC = gcc
# Options de compilations 
CFLAGS = -Wall -Wextra -Werror -g -I../libdico/src  

LDFLAGS = -L../libdico/lib -ldico -Wl,-rpath,$(abspath ../libdico/lib)

# On récupère tous les fichiers du du dossier src/
SRC = $(wildcard src/*.c)
# Dans la variable OBJ on transforme tous les chemins de fichiers finissant par .c
# en .O ( évite de tout )
OBJ = $(SRC:.c=.o)

# Chemin du fichier executable à la fin
EXEC = bin/wcCompile

all: $(EXEC)

# On execute la cible 
$(EXEC): $(OBJ)
	@mkdir -p bin
	$(CC) $(OBJ) -o $@ $(LDFLAGS)


# 4. Compilation (Création des .o)
# Cette règle générique dit : "Pour faire un .o à partir d'un .c..."
# $< remplace la dépendance courant (ici .c) / $@ le nom de la cible avec .c devenu .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 5. Nettoyage
clean:
	rm -f $(OBJ) $(EXEC)















