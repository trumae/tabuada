#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
#include <time.h>

#include "src/gp.h"
#include "src/machine.h"
#include "src/fitmult.h"
#include "src/fitexternal.h"

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


