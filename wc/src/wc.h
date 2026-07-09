#ifndef WC_H
#define WC_H
#include <stdio.h>

struct _count {
    int lflag;          /* Indicateur pour compter les lignes */
    int wflag;          /* Indicateur pour compter les mots */
    int cflag;          /* Indicateur pour compter les caractères */
    int sflag;          /* Indicateur pour compter les mots uniques */

    long lcount;        /* Nombre de lignes */
    long wcount;        /* Nombre de mots */
    long ccount;        /* Nombre de caractères */
    long scount;        /* Nombre de mots uniques */

    long ltotal;        /* Nombre total de lignes */
    long wtotal;        /* Nombre total de mots */
    long ctotal;        /* Nombre total de caractères */
    long stotal;        /* Nombre total de mots uniques */
};

typedef struct _count countWord;

void count_file(FILE* f, countWord* stats);
void usage(void);



#endif

