#ifndef __GP__
#define __GP__

#include <stdio.h>

/*
   Tipos definidos
*/

typedef struct structind {
   int tam;
   int *data;
   
} Individuo ;


typedef struct structamb {
  int tam;
  double txcross;
  double txmut;
  Individuo **data;
} Populacao;


extern char programname[1024];

/*
   Funcoes definidas
*/

Individuo *makeIndividuo ( int t );
Individuo *makeIndividuoRand (int t);
void freeIndividuo(Individuo *ind);
void printIndividuo(FILE *file, Individuo *ind);


Populacao *makePopulacao (int t, double txc, double txm);
Populacao *makePopulacaoRand (int t, int tind, double txc, double txm);
void freePopulacao(Populacao *p);
void printPopulacao(FILE *file, Populacao *pop);

double bloatfactor (int tam);

#endif

