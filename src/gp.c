#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
#include <time.h>

#include "gp.h"
#include "machine.h"
#include "fitmult.h"
#include "fitexternal.h"

static int tammaxestimado;

inline Individuo *makeIndividuo ( int t ) {
    Individuo *ind;

    ind = malloc(sizeof(Individuo));
    ind->tam = t;
    ind->data = malloc(sizeof(int) * t);
    
    return ind;
}


inline Individuo *makeIndividuoRand (int t){
    int i;
    Individuo *ind;
    
    
    ind = makeIndividuo( t );

    for(i = 0; i < t; i++)
       ind->data[i] = rand();
    
    return ind;
}


inline void freeIndividuo(Individuo *ind) {
   free(ind->data);
   free(ind);
}

inline void printIndividuo(FILE* file, Individuo *ind) {
   int i;
   fprintf(file, "<");
   for(i = 0; i < ind->tam; i++) 
      fprintf(file, "%d ", ind->data[i]);
   fprintf(file, ">\n");
}



inline Populacao *makePopulacao (int t, double txc, double txm){
    Populacao *pop;
    
    if (t <= 4 ) {
       fprintf(stderr,"Populacao menor que 4 individuos nao e aceita pelo algoritmo\n");
       exit(1);
    }
    pop = malloc (sizeof(Populacao));
    pop->tam = t;
    pop->txcross = txc;
    pop->txmut = txm;
    pop->data = malloc(sizeof(Individuo *) * t);
    
    return pop;
}

inline Populacao *makePopulacaoRand (int t, int tind, double txc, double txm){
    Populacao *pop;
    int i;
    
    pop = makePopulacao(t, txc, txm);

    for(i = 0; i < t; i++)
       pop->data[i] = makeIndividuoRand(tind);
    
    return pop;
}

inline void freePopulacao(Populacao *p){
   int i;
   
   for(i = 0; i < p->tam; i++)
      freeIndividuo(p->data[i]);

   free(p->data);
   free(p);
}

inline void printPopulacao(FILE *file, Populacao *pop){
   int i;
   
   fprintf(file,"<<< %d %f %f \n", pop->tam, pop->txcross, pop->txmut);
   for(i = 0; i < pop->tam; i++)
      printIndividuo(file, pop->data[i]);

   fprintf(file, " >>>");

}


///////////////////////////////////////

inline double mysorteio(){
   double ret = (double) ((double) rand())/RAND_MAX;
   return ret;
}

inline int mysorteiounico(int tampop, int i1, int i2, int i3){
   int ret = rand() % tampop;
   while(ret == i1 || ret == i2 || ret == i3)
      ret = rand() % tampop;
   return ret;      
}

inline void crossover (const Individuo *ind1, const Individuo *ind2, const double txcross,
		Individuo **rind1, Individuo **rind2) {
    int i;
    int pcorte1 = rand() % (ind1->tam + 1);
    int pcorte2 = rand() % (ind2->tam + 1);
    int tam1 = pcorte1 + ind2->tam - pcorte2;
    int tam2 = pcorte2 + ind1->tam - pcorte1;
    
    //printf("corte = %d %d tam = %d %d tx = %f aux = %d RAND_MAX = %d rate=%f\n", pcorte1, pcorte2, tam1, 
    //          tam2, txcross,aux, RAND_MAX, (double)aux/(double)RAND_MAX);
      *rind1 = makeIndividuo(tam1);
      for(i = 0; i < pcorte1 ; i++) 
	 (*rind1)->data[i] = ind1->data[i];
    
      for(i = pcorte2; i < ind2->tam; i++) 
	 (*rind1)->data[pcorte1 + i - pcorte2] = ind2->data[i];
    
      *rind2 = makeIndividuo(tam2);                    
      for(i = 0; i < pcorte2 ; i++) 
	 (*rind2)->data[i] = ind2->data[i];
    
      for(i = pcorte1; i < ind1->tam; i++) 
	 (*rind2)->data[pcorte2 + i - pcorte1] = ind1->data[i];
	    
}

inline void mutation(Individuo *ind, const double txmut){
      int i;
      
      for(i = 0; i < ind->tam; i++){
	 if (mysorteio() < txmut)
	    ind->data[i] = rand();
      }
}


void fitnessinst(Populacao *pop, int i1, int i2, int *champion, int *loser, int verbose){

   static double melhornota = 0;
   static long count = 0;
   int nota1, nota2;
   
   nota1=(pop->data[i1]->tam == 6)?1:0;
   nota2=(pop->data[i2]->tam == 6)?1:0;

   if( abs(pop->data[i1]->tam - 6) < abs(pop->data[i2]->tam - 6) ) {
      *champion = i1;
      *loser = i2;
   } else {
      *champion = i2;
      *loser = i1;
   }
  
   if (verbose ) fprintf(stderr,"%ld %d \n", count++, nota1);
   if (verbose) fprintf(stderr,"%ld %d \n", count++, nota2);


}

int torneio (Populacao *pop, void (*fitness)(Populacao *pop, int i1, int i2, int *champion, int *loser, int verb) , int verbose) {
    Individuo *rind1;
    Individuo *rind2;
    int champion1, champion2;
    int loser1, loser2;
    int i1 = mysorteiounico(pop->tam, -1,-1,-1);
    int i2 = mysorteiounico(pop->tam, i1,-1,-1);
    int i3 = mysorteiounico(pop->tam, i1,i2,-1);
    int i4 = mysorteiounico(pop->tam, i1,i2,i3);
    
    //printf("Sorteados %d %d %d %d\n", i1,i2,i3,i4);
    fitness(pop, i1, i2,&champion1, &loser1, verbose);
    fitness(pop, i3, i4,&champion2, &loser2, verbose);
    
    crossover(pop->data[champion1], pop->data[champion2], pop->txcross,
		 &rind1, &rind2);
    
    mutation(rind1, pop->txmut);
    mutation(rind2, pop->txmut);
	
    freeIndividuo(pop->data[loser1]);
    freeIndividuo(pop->data[loser2]);
    pop->data[loser1] = rind1;
    pop->data[loser2] = rind2;
}


inline double bloatfactor (int tam) {
   if ( tam <= tammaxestimado)
     return 1.0;
   else
     return ((double) tammaxestimado) / tam; 
}

void (*fitness)(Populacao *pop, int i1, int i2, int *champion, int *loser, int verb);

void initGP () {
    srand(time(NULL));
    //fitness = fitnessmult;
    //fitness = fitnessinst;
}




/*****************************************************************************


******************************************************************************/


const char* program_name;


void print_usage(FILE *stream, int exit_code){
  fprintf(stream, "Usage: %s options\n", program_name);
  fprintf(stream,
	  "  -h   --help                     Display this usage information. \n"
          "  -t   --tournament number        Number of tournamment (default 1000000)\n"
	  "  -n   --population number        Length of Population (default 100)\n"
          "  -i   --iniind number            Initial length of individual (default 40)\n"
          "  -H   --tammaxestimado number    Max Length for bloat factor (default 10)\n"
	  "  -c   --crossover number         Rate of crossover (default 0.9)\n"
          "  -m   --mutation  number         Rate of mutation (default 0.01)\n"
          "  -e   --extern                   Use external program (default = no)\n"
          "  -p   --program name             Name of external program (default = fitextern)\n"
          "  -v   --verbose                  Print verbose messages.\n");
  exit(exit_code);
}


char programname[1024];

int main(int argc, char **argv) {
  int next_option, verbose = 0;
  int i,r;
  Populacao *pop;
  Individuo *rind1;
  Individuo *rind2;
  int numtorn = 1000000;
  int iniind = 40;
  int tampop = 100;
  double rcrossover = 0.9;
  double rmutation = 0.01;
  int useexternal = 0;

  strcpy(programname, "fitextern");

  tammaxestimado = 50;
  fitness = fitnessmult;
  //fitness = fitnessinst;
  const char *short_options = "hvt:n:c:m:i:ep:H:";
  const struct option long_options[] = {
    {"help", 0, NULL, 'h'},
    {"tournament", 1, NULL, 't'},
    {"population", 1, NULL, 'n'},
    {"iniind", 1, NULL, 'i'},
    {"tammaxestimado", 1, NULL, 'H'},
    {"crossover", 1, NULL, 'c'},
    {"mutation", 1, NULL, 'm'},
    {"extern", 0, NULL, 'e'},
    {"program", 1, NULL, 'p'},
    {"verbose", 0, NULL, 'v'},
    {NULL, 0, NULL, 0}
  };
  program_name = argv[0];

  do {
    next_option = getopt_long(argc, argv, short_options, long_options, NULL);
    switch (next_option) {
    case 'h':
      print_usage(stdout, 0);
      break;
    case 'v':
      verbose = 1;
      break;
    case '?':  /* opcao invalida*/
      print_usage(stderr, 1);
      break;

    case 't':
      numtorn = atoi(optarg);
      break;

    case 'H':
      tammaxestimado = atoi(optarg);
      break;


    case 'n':
      tampop = atoi(optarg);
      break;

    case 'i':
      iniind = atoi(optarg);
      break;

    case 'c':
      rcrossover = atof(optarg);
      break;

    case 'm':
      rmutation = atof(optarg);
      break;

    case 'e':
      useexternal = 1;
      break;

    case 'p':
      strncpy(programname, optarg, 1024);
      break;


    case -1:
      break;
    default:
      abort();
    }
  } while (next_option != -1);
  
  if (useexternal) fitness = fitexternal;
  initGP();
  pop = makePopulacaoRand(tampop, iniind, rcrossover, rmutation);
  
  for(i = 0; i < numtorn; i++) 
    r = torneio(pop, fitness, verbose);
  
  //printPopulacao(stdout, pop);
  freePopulacao(pop);
  return 0;
}


