/* wc - count lines, words and characters	Author: David Messer */
/*test*/
#include "wc.h"
#include <stdio.h>
#include <stdlib.h>
#define isspace(c) (c==' ' || c=='\t' || c=='\n' || c=='\f' || c=='\r')

void count(FILE* f,countWord* stats)
{
  register int c;
  register int word = 0;

  stats->lcount = 0;
  stats->wcount = 0;
  stats->ccount = 0L;

  while ((c = getc(f)) != EOF) {
	stats->ccount++;

	if (isspace(c)) {
		if (word){
			stats->wcount++;
			word = 0;
		} 
	} else {
		word = 1;
	}

	if (c == '\n' || c == '\f'){
		stats->lcount++;
	} 
  }
  stats->ltotal += stats->lcount;
  stats->wtotal += stats->wcount;
  stats->ctotal += stats->ccount;
}

void usage(void)
{
  fprintf(stderr, "Usage: wc [-lwc] [name ...]\n");
  exit(1);
}
