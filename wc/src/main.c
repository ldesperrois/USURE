#include "wc.h"
#include <stdlib.h>

int main(int argc, char* argv[])
{
  int k;
  char *cp;
  int tflag, files;
  // On initialise les champs de la structure
  countWord  counter={0};
  files = argc - 1;
  k = 1;
  cp = argv[1];
  if (argc > 1 && *cp++ == '-') {
	files--;
	k++;			/* points to first file */
	while (*cp != 0) {
		switch (*cp) {
		    case 'l':	counter.lflag++;	break;
		    case 'w':	counter.wflag++;	break;
		    case 'c':	counter.cflag++;	break;
		    default:	usage();
		}
		cp++;
	}
  }

  /* If no flags are set, treat as wc -lwc. */
  if (!counter.lflag && !counter.wflag && !counter.cflag) {
	counter.lflag = 1;
	counter.wflag = 1;
	counter.cflag = 1;
  }

  /* Process files. */
  tflag = files >= 2;		/* set if # files > 1 */

  /* Check to see if input comes from std input. */
  if (k >= argc) {
	count_file(stdin,&counter);

	if (counter.lflag) printf(" %6ld", counter.lcount);
	if (counter.wflag) printf(" %6ld", counter.wcount);
	if (counter.cflag) printf(" %6ld", counter.ccount);
	printf(" \n");
	fflush(stdout);
	exit(0);
  }

  /* There is an explicit list of files.  Loop on files. */
  while (k < argc) {
	FILE *f ;
	if ((f = fopen(argv[k], "r")) == (FILE *) NULL) {
		fprintf(stderr, "wc: cannot open %s\n", argv[k]);
	} else {
		count_file(f, &counter);
		if (counter.lflag) printf("Nombre de lignes :%6ld\n", counter.lcount);
		if (counter.wflag) printf("Nombre de mots :%6ld\n", counter.wcount);
		if (counter.cflag) printf("Nombre de caractères:%6ld\n", counter.ccount);
		printf("Nom du fichier: %s\n", argv[k]);
		fclose(f);
	}
	k++;
  }

  if (tflag) {
	if (counter.lflag) printf("Nombre de lignes :%6ld\n ", counter.ltotal);
	if (counter.wflag) printf("Nombre de mots :%6ld\n", counter.wtotal);
	if (counter.cflag) printf("Nombre de caractères:%6ld\n", counter.ctotal);
	printf(" total\n");
  }
  fflush(stdout);
  exit(0);
}