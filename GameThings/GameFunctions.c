#include <stdio.h>
#include <stdlib.h>

#include "../CardThings/CardClasses.h"

// arrays que guardam os valores de cada carta como string para serem impressas
char numeros[14][4] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "V", "D", "R", "C"};
char naipes[4][4] = {"♦", "♥", "♣", "♠"};

void debug(char *message) {
  fprintf(stderr, "Print do J: %s\n", message);
}

// função auxiliar para debug da mão do bot
void print_cards(Carta *cartas, int *qtd_cartas) {
  char *debugar = malloc(sizeof(char));
  debug("Printando cartas");
  for (int i = 0; i < (*qtd_cartas); i++) {
    sprintf(debugar, " %s%s", numeros[(cartas[i].numero - 1)], naipes[cartas[i].naipe]);
    debug(debugar);
  }
  printf("\n");
}