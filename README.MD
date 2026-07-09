# USURE

Projet C composé de deux parties :

- `libdico` : une bibliothèque de dictionnaire/htable utilisée pour stocker et rechercher des mots.
- `wc` : un programme inspiré de `wc` qui compte les lignes, les mots, les caractères et les mots uniques.

Le dépôt contient aussi un benchmark dans `libdico` pour mesurer les performances du dictionnaire sur un fichier de mots.

## Prérequis

- Un compilateur C compatible GCC.
- `make`.
- Sous macOS, les Command Line Tools ou un environnement équivalent doivent être installés.

## Arborescence

- `libdico/src/` : code source de la bibliothèque.
- `wc/src/` : code source de l’exécutable `wcCompile`.
- `README.MD` : documentation du projet.

## Compilation

Depuis la racine du projet :

```bash
make
```

Cette commande compile la bibliothèque `libdico` puis l’exécutable `wcCompile`.

Pour compiler seulement une partie :

```bash
make -C libdico
make -C wc
```

## Exécution de `wc`

Après compilation, l’exécutable est généré dans `wc/bin/wcCompile`.

### Utilisation

```bash
./wc/bin/wcCompile [-l] [-w] [-c] [-S] [fichier ...]
```

Options disponibles :

- `-l` : nombre de lignes.
- `-w` : nombre de mots.
- `-c` : nombre de caractères.
- `-S` : nombre de mots uniques.

Comportement par défaut : si aucune option n’est donnée, le programme affiche `-l -w -c`.

### Exemples

```bash
./wc/bin/wcCompile fichierTextTest/pg17518.txt
./wc/bin/wcCompile -l -w fichierTextTest/test
./wc/bin/wcCompile -S fichierTextTest/test

```

Si aucun fichier n’est passé, le programme lit l’entrée standard.

## Benchmark du dictionnaire

Le module `libdico` contient un benchmark basé sur le fichier `mots_17479.txt`.

Pour le lancer :

```bash
make -C libdico test
```

Cette commande compile un programme de test dans `libdico/test_prog`, puis l’exécute.

## Nettoyage

Pour supprimer les binaires et objets générés :

```bash
make clean
```

Cette commande nettoie `libdico` et `wc`.

## Remarques

- Le projet affiche aussi des informations de dictionnaire et de tri des mots uniques via la bibliothèque.
- Les messages de sortie peuvent être très verbeux sur de gros fichiers, surtout avec l’option `-S`.

- **IMPPORTANT** : Pour l'affichage il faut se rappeller que un même mot répété plusieurs fois compte donc pour plusieurs dans le total,mais pour un seul dans les mots uniques

- on peut avoir des différences dans l'affichage des mots qui apparaissent une seul fois et le nombre de mots uniques ( sans compter les deuxièmes ou troisièmes fois où apparaissenet un mot mais seulement sa première apparition dans le fichier )
