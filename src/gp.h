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

inline Individuo *makeIndividuo ( int t );
inline Individuo *makeIndividuoRand (int t);
inline void freeIndividuo(Individuo *ind);
inline void printIndividuo(FILE *file, Individuo *ind);


inline Populacao *makePopulacao (int t, double txc, double txm);
inline Populacao *makePopulacaoRand (int t, int tind, double txc, double txm);
inline void freePopulacao(Populacao *p);
inline void printPopulacao(FILE *file, Populacao *pop);

inline double bloatfactor (int tam);

#endif

