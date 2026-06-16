#include "wc.h"

int main(int argc, char* argv[])
{
  int k;
  char *cp;
  int tflag, files;
  countWord counter;
  /* Get flags. */
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
	count.lflag = 1;
	count.wflag = 1;
	count.cflag = 1;
  }

  /* Process files. */
  tflag = files >= 2;		/* set if # files > 1 */

  /* Check to see if input comes from std input. */
  if (k >= argc) {
	count(stdin);
	if (counter.lflag) printf(" %6ld", counter.lcount);
	if (counter.wflag) printf(" %6ld", wcount);
	if (cflag) printf(" %6ld", ccount);
	printf(" \n");
	fflush(stdout);
	exit(0);
  }

  /* There is an explicit list of files.  Loop on files. */
  while (k < argc) {
	FILE *f;

	if ((f = fopen(argv[k], "r")) == (FILE *) NULL) {
		fprintf(stderr, "wc: cannot open %s\n", argv[k]);
	} else {
		count(f);
		if (lflag) printf(" %6ld", lcount);
		if (wflag) printf(" %6ld", wcount);
		if (cflag) printf(" %6ld", ccount);
		printf(" %s\n", argv[k]);
		fclose(f);
	}
	k++;
  }

  if (tflag) {
	if (lflag) printf(" %6ld", ltotal);
	if (wflag) printf(" %6ld", wtotal);
	if (cflag) printf(" %6ld", ctotal);
	printf(" total\n");
  }
  fflush(stdout);
  exit(0);
}