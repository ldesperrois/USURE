#include "wc.h"
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
  int tflag;
  // On initialise les champs de la structure
  countWord  counter={0};
  int opt;
  while ((opt = getopt(argc,argv,"lwcS"))!=-1) {
	switch (opt) {
		case 'l':	counter.lflag++;	break;
		case 'w':	counter.wflag++;	break;
		case 'c':	counter.cflag++;	break;
		case 'S' :  counter.sflag++;		break;
		default:	usage();
	}	
  }
  

  if (!counter.lflag && !counter.wflag && !counter.cflag) {
	counter.lflag = 1;
	counter.wflag = 1;
	counter.cflag = 1;
	counter.sflag = 1;
  }
  // Si on a deux fichiers
  tflag = (argc - optind) >= 2;		

  if (optind >= argc) {
	count_file(stdin,&counter);
	if (counter.lflag) printf(" %6ld", counter.lcount);
	if (counter.wflag) printf(" %6ld", counter.wcount);
	if (counter.cflag) printf(" %6ld", counter.ccount);
	if (counter.sflag) printf(" %6ld",counter.scount);
	printf(" \n");
	fflush(stdout);
	exit(0);
  }
  while (optind < argc) {
	FILE *f ;
	if ((f = fopen(argv[optind], "r")) == (FILE *) NULL) {
		fprintf(stderr, "wc: cannot open %s\n", argv[optind]);
	} else {
		count_file(f, &counter);
		if (counter.lflag) printf("\nNombre de lignes :%6ld\n", counter.lcount);
		if (counter.wflag) printf("Nombre de mots :%6ld\n", counter.wcount);
		if (counter.cflag) printf("Nombre de caractères:%6ld\n", counter.ccount);
		if (counter.sflag) printf("Nombre de mots uniques%6ld\n",counter.scount);
		printf("Nom du fichier: %s\n", argv[optind]);
		fclose(f);
	}
	optind++;

  }
  if (tflag) {
	if (counter.lflag) printf("\nNombre de lignes :%6ld\n ", counter.ltotal);
	if (counter.wflag) printf("Nombre de mots :%6ld\n", counter.wtotal);
	if (counter.cflag) printf("Nombre de caractères:%6ld\n", counter.ctotal);
	if (counter.sflag) printf("Nombre de mots uniques%6ld\n",counter.scount);
	printf(" total\n");
  }
  fflush(stdout);
  exit(0);
}