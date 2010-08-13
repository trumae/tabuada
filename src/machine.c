#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "machine.h"


inline statemachine * makeStatemachine(Individuo *ind, int tm){
    statemachine *st;
    
    st = malloc(sizeof(statemachine));
    st->pc = 0;
    st->flaghalt = 0;
    st->flagerr = 0;
    st->tammem = tm;
    st->mem = malloc(sizeof(short) * tm);
    st->code =  ind;
    return st;
}


inline void freeStatemachine(statemachine *st){
   free(st->mem);
   free(st);
}


inline static void move(statemachine *st){
   int oper1 = st->code->data[3 * st->pc + 1];
   int oper2 = st->code->data[3 * st->pc + 2];
   
   st->mem[oper1 % st->tammem] = st->mem[oper2 % st->tammem];
   st->pc++;
}

inline static void movi(statemachine *st){
   int oper1 = st->code->data[3 * st->pc + 1];
   int oper2 = st->code->data[3 * st->pc + 2];
   
   st->mem[oper1 % st->tammem] = oper2 % TAMWORD;
   st->pc++;

}

inline static void add(statemachine *st){
   int oper1 = st->code->data[3 * st->pc + 1];
   int oper2 = st->code->data[3 * st->pc + 2];
   
   st->mem[oper1 % st->tammem] = st->mem[oper1 % st->tammem] + 
				 st->mem[oper2 % st->tammem];
   st->pc++;

}

inline static void subi(statemachine *st){
   int oper1 = st->code->data[3 * st->pc + 1];
   int oper2 = st->code->data[3 * st->pc + 2];
   
   st->mem[oper1 % st->tammem] = st->mem[oper1 % st->tammem] - (oper2 % TAMWORD);
   st->pc++;
}

inline static void jumpz(statemachine *st){
   int oper1 = st->code->data[3 * st->pc + 1];
   int oper2 = st->code->data[3 * st->pc + 2];
   
   if (st->mem[oper1 % st->tammem] == 0) {
       st->pc = oper2 % ((int) st->code->tam / 3) ;
       return;
   }
   st->pc++;
}

inline static void jump(statemachine *st){
   int oper1 = st->code->data[3 * st->pc + 1];
   
   
   if (st->code->tam <= 2 ) {
      st->flagerr = 1;
      st->flaghalt = 1;
      return;
   }
   
   st->pc = oper1 % ((int) st->code->tam / 3);
   return;
}

inline static void ret(statemachine *st){
   st->flaghalt = 1;
   return;
}


void executemachine(statemachine *st, int n) {
   int i;

   for(i = 0 ; i < n; i++) {
      if (st->flaghalt)
	return; 

      if(st->code->tam <= st->pc) {
	 return;
      }


      if(st->pc == st->code->tam / 3) return;
//      assert(3 * st->pc < st->code->tam);

      switch (st->code->data[3 * st->pc] % NUMINSTR) {
	 case INSTRMOVE:
	    move(st);
	 break;
	 case INSTRMOVI:
	    movi(st);
	 break;
	 case INSTRADD:
	    add(st);
	 break;
	 case INSTRSUBI:
	    subi(st);
	 break;
	 case INSTRJUMPZ:
	    jumpz(st);
	 break;
	 case INSTRJUMP:
	    jump(st);
	 break;
	 case INSTRRET:
	   ret(st);
	 break;
	 default:
	   fprintf(stderr, "Codigo de instrucao inesperado");
      }
   
   }
   return;
}

inline void printStatemachine(statemachine *st){
   int i;
   fprintf (stdout, "(pc %d, halt %d, err %d, code ", 
	st->pc, st->flaghalt, st->flagerr);
   for(i = 0; i < (st->code->tam / 3) ; i++) {
      fprintf(stdout, ", ");
      switch (st->code->data[3 * i] % NUMINSTR) {
	 case INSTRMOVE:
	    fprintf(stdout, " MOVE %d %d", 
		st->code->data[3 * i + 1] % st->tammem, 
		st->code->data[3 * i + 2] % st->tammem);
	 break;
	 case INSTRMOVI:
	    fprintf(stdout, " MOVI %d %d", 
		st->code->data[3 * i + 1] % st->tammem, 
		st->code->data[3 * i + 2] % TAMWORD);
	 break;
	 case INSTRADD:
	    fprintf(stdout, " ADD %d %d", 
		st->code->data[3 * i + 1] % st->tammem, 
		st->code->data[3 * i + 2] % st->tammem);            
	 break;
	 case INSTRSUBI:
	    fprintf(stdout, " SUBI %d %d", 
		st->code->data[3 * i + 1] % st->tammem, 
		st->code->data[3 * i + 2] % TAMWORD);
	 break;
	 case INSTRJUMPZ:
	    fprintf(stdout, " JUMPZ %d %d", 
		st->code->data[3 * i + 1] % st->tammem, 
		st->code->data[3 * i + 2] % ((int) st->code->tam / 3)  );
	 break;
	 case INSTRJUMP:
	    fprintf(stdout, " JUMP %d", 
		st->code->data[3 * i + 1] % ((int) st->code->tam / 3)  );
	 break;
	 case INSTRRET:
	    fprintf(stdout, " RET " );
	 break;
      }
   }
   
   fprintf(stdout, " )\n");

}







