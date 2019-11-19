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
