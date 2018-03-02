#ifndef __MACHINE__
#define __MACHINE__

#include "gp.h"


#define NUMINSTR     7
#define TAMWORD     16

#define INSTRMOVE    0
#define INSTRMOVI    1
#define INSTRADD     2
#define INSTRSUBI    3
#define INSTRJUMPZ   4
#define INSTRJUMP    5
#define INSTRRET     6


typedef struct structmachine {
   int pc; /* program counter */
   int tammem;
   short *mem;
   char flaghalt;
   char flagerr;
   Individuo *code;
} statemachine;

statemachine * makeStatemachine(Individuo *ind, int tm);
void executemachine(statemachine *st,int n);
void freeStatemachine(statemachine *st);
void printStatemachine(statemachine *st);

#endif

