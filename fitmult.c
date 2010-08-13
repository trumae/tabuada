#include "gp.h"
#include "machine.h"
#include "fitmult.h"

#define MAXSTEPSMACHINE   100

void fitnessmult(Populacao *pop, int i1, int i2, int *champion, int *loser, int verbose){
   statemachine *m;
   double nota1 = 0;
   double nota2 = 0;
   int i,j;
   static double melhornota = 0;
   static long count = 0;

   for ( i = 1; i < 10; i++)
     for ( j = 1; j < 10; j++) {
	m = makeStatemachine(pop->data[i1], 3);
	
	/* preparando memoria */
	m->mem[0] = i;
	m->mem[1] = j;
	m->mem[2] = rand() % 16;
	/* Executando simulador */
	//printStatemachine(m);
	executemachine(m, MAXSTEPSMACHINE);
	
	/*verificando resultados */
	if (m->mem[2] == i * j) nota1 += 100;
	if (m->flaghalt && !m->flagerr) nota1 ++;
	
	freeStatemachine(m);
     }

   for ( i = 1; i < 10; i++)
     for ( j = 1; j < 10; j++) {
	m = makeStatemachine(pop->data[i2], 3);
	
	/* preparando memoria */
	m->mem[0] = i;
	m->mem[1] = j;
	m->mem[2] = rand() % 16;
	/* Executando simulador */
	//printStatemachine(m);
	executemachine(m, MAXSTEPSMACHINE);
	
	/*verificando resultados */
	if (m->mem[2] == i * j) nota2 += 100;
	if (m->flaghalt && !m->flagerr ) nota2 ++;

	
	freeStatemachine(m);
     }

   /* corrige notas */
   nota1 *= bloatfactor(pop->data[i1]->tam);
   nota2 *= bloatfactor(pop->data[i2]->tam);


   /* imprime maquinas e notas */
   if( nota1 > melhornota) {
      m = makeStatemachine(pop->data[i1], 3);  
      fprintf(stdout, "NOTA = %.2f ", nota1);
      printStatemachine(m);
      freeStatemachine(m);
      melhornota = nota1;
   }

   if (  nota2 > melhornota ) {  
      m = makeStatemachine(pop->data[i2], 3);
      fprintf(stdout, "NOTA = %.2f ", nota2);
      printStatemachine(m);
      freeStatemachine(m);
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

