#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "gp.h"
#include "machine.h"
#include "fitexternal.h"

#define MAXSTEPSMACHINE   100

static void saveChampion( Individuo *ind) {
  FILE *f;
  int i;

  /* escreve dados para programa externo */
  if ((f = fopen("champion.ind", "wt")) == NULL){
    fprintf(stderr, "Falha ao abrir arquivo gen.ind\n");
    exit(1);
  }
  for(i = 0 ; i < ind->tam; i++)
    fprintf(f, "%d\n", ind->data[i]);
  
  fclose(f);
}


static double getFitExternal(Individuo *ind, int verbose){
  FILE *f;
  int i, pid;
  char buffer[1024];
  double result;

  /* escreve dados para programa externo */
  if ((f = fopen("gen.ind", "wt")) == NULL){
    fprintf(stderr, "Falha ao abrir arquivo gen.ind\n");
    exit(1);
  }
  for(i = 0 ; i < ind->tam; i++)
    fprintf(f, "%d\n", ind->data[i]);
  
  fclose(f);

  /* chamada de programa externo - no linux deve ser com fork */
  /*if ( (pid = fork()) == -1) {
    fprintf(stderr, "Erro no fork\n");
  } else 
    if (pid == 0 ) { // processo filho 
      if (verbose) fprintf(stdout, "Processo Filho - Exec programa %s\n", programname);
      if( execlp(programname, programname, NULL) == -1) {
	fprintf(stderr, "Erro no execlp\n");
	exit(1);
      }
    }
  if (verbose) fprintf(stdout, "Processo Pai - Esperando\n");
  wait();*/

  strcpy(buffer, "./");
  strcat(buffer, programname);
  system(buffer);
  
  /* Le dados gerados pelo programa externo*/
  if ((f = fopen("result.ind", "rt")) == NULL){
    fprintf(stderr, "Falha ao abrir arquivo result.ind\n");
    exit(1);
  }

  if(fgets(buffer,1024,f) == NULL) {
    fprintf(stderr, "Falha ao Ler arquivo result.ind\n");
    exit(1);
  }
  
  result = atof(buffer);

  fclose(f);
  

  return result;
}

void fitexternal(Populacao *pop, int i1, int i2, int *champion, int *loser, int verbose){
   statemachine *m;
   double nota1 = 0;
   double nota2 = 0;
   int i,j;
   static double melhornota = 0;
   static long count = 0;

   nota1 = getFitExternal(pop->data[i1],verbose);
   nota2 = getFitExternal(pop->data[i2],verbose);

   /* corrige notas */
   nota1 *= bloatfactor(pop->data[i1]->tam);
   nota2 *= bloatfactor(pop->data[i2]->tam);


   /* imprime maquinas e notas */
   if( nota1 > melhornota) {  
      fprintf(stdout, "NOTA = %.2f ", nota1);
      saveChampion(pop->data[i1]);
      melhornota = nota1;
   }

   if (  nota2 > melhornota ) {  
      fprintf(stdout, "NOTA = %.2f ", nota2);
      saveChampion(pop->data[i2]);
      melhornota = nota2;
   }

   //if(! (count % 1000)) 
      if (verbose ) fprintf(stderr,"%d %.2f %.2f\n", count++, nota1, melhornota);
   //if(! (count % 1000)) 
      if (verbose) fprintf(stderr,"%d %.2f %.2f\n", count++, nota2, melhornota);

   if(  nota1 >= nota2 ) {
      *champion = i1;
      *loser = i2;
   } else {
      *champion = i2;
      *loser = i1;
   }
}

