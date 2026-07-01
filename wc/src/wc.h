#ifndef WC_H
#define WC_H
#include <stdio.h>

struct _count {
    int lflag;          /* Count lines */
    int wflag;          /* Count words */
    int cflag;          /* Count characters */
    int sflag;          /* Count of uniq words*/

    long lcount;        /* Count of lines */
    long wcount;        /* Count of words */
    long ccount;        /* Count of characters */
    long scount;        /* Count of uniq words*/

    long ltotal;        /* Total count of lines */
    long wtotal;        /* Total count of words */
    long ctotal;        /* Total count of characters */
};

typedef struct _count countWord;

void count_file(FILE* f, countWord* stats);
void usage(void);



#endif

