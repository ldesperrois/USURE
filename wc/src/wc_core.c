/* wc - count lines, words and characters	Author: David Messer */
/*test*/
#include "wc.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "dico.h"

void count_file(FILE* f,countWord* stats)
{
  char mot[512];
  int motTaille = 0;
  int c;
  int word = 0;
  dict_t* dict = dict_create();
  stats->lcount = 0;
  stats->wcount = 0;
  stats->ccount = 0L;
  stats->scount=0;

  while ((c = getc(f)) != EOF) {
	stats->ccount++;

	if (isspace(c)) {
		if (word){
      // On termine le mot pas le caractère de fin de chaine ( marqueur de fin)
      mot[motTaille] = '\0';
      dict_status_t status = dict_add(dict, mot, motTaille+1, NULL, 0);  
      // Si un nouveau mot unique est ajouté
      if(status==DICT_OK){
        stats->scount++;
      }
			stats->wcount++;
      mot[0]='\0';
      motTaille=0;
			word = 0;
		} 
	} else {
      mot[motTaille] = c;
      motTaille++;
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
