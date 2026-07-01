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
  

  if (!counter.lflag && !counter.wflag && !counter.cflag && !counter.sflag) {
	counter.lflag = 1;
	counter.wflag = 1;
	counter.cflag = 1;

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
		if (counter.lflag) 	printf("%-25s%6ld\n", "Nombre de lignes :", counter.lcount);
		if (counter.wflag) printf("%-25s%6ld\n", "Nombre de mots :", counter.wcount);
		if (counter.cflag) printf("%-25s%6ld\n", "Nombre de caractères :", counter.ccount);
		if (counter.sflag) printf("%-25s%6ld\n", "Nombre de mots uniques :", counter.scount);
		
		
		
		printf("%-25s%6s\n", "Nom du fichier :", argv[optind]);
		fclose(f);
	}
	optind++;

  }
  if (tflag) {
	if (counter.lflag) 	printf("%-25s%6ld\n", "Nombre de lignes :", counter.lcount);
	if (counter.wflag) printf("%-25s%6ld\n", "Nombre de mots :", counter.wcount);
	if (counter.cflag) printf("%-25s%6ld\n", "Nombre de caractères :", counter.ccount);
	if (counter.sflag) printf("%-25s%6ld\n", "Nombre de mots uniques :", counter.scount);
	printf(" total\n");
  }
  fflush(stdout);
  exit(0);
}