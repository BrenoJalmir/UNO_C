#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../GameThings/GameFunctions.c"
#include "CardClasses.h"

// função que retorna se é possível jogar uma determinada carta de acordo com a carta da mesa
int can_be_played(Carta carta, Carta mesa) {
  // Ases e Coringas podem ser jogados a qualquer momento
  // Cartas de mesmo naipe ou número podem ser jogadas
  return mesa.naipe == carta.naipe || mesa.numero == carta.numero || carta.numero == A || carta.numero == C;
}

// função que lê o naipe complementar quando um Ás ou Coringa é jogado
Naipe read_suit(char *msg) {
  if (strcmp(msg, "♦") == 0)
    return OUROS;
  else if (strcmp(msg, "♣") == 0)
    return PAUS;
  else if (strcmp(msg, "♥") == 0)
    return COPAS;
  return ESPADAS;
}

// função que lê o naipe de cartas dadas pelo gerenciadore retorna o enum equivalente
Naipe get_suit(char *msg) {
  int pos = 1;
  if (msg[0] == '1') {
    pos++;
  }
  // pos serve para pular o valor que identifica a figura da carta e comparar apenas os naipes
  if (strcmp(msg + pos, "♦") == 0)
    return OUROS;
  else if (strcmp(msg + pos, "♣") == 0)
    return PAUS;
  else if (strcmp(msg + pos, "♥") == 0)
    return COPAS;
  return ESPADAS;
}

// função que lê o número de cartas dadas pelo gerenciador e retorna o enum equivalente
Numero get_number(char *msg) {
  if (msg[0] == 'A')
    return A;
  else if (msg[0] == '1')
    return 10;
  else if (msg[0] == 'V')
    return V;
  else if (msg[0] == 'D')
    return D;
  else if (msg[0] == 'R')
    return R;
  else if (msg[0] == 'C')
    return C;

  // msg[0] - '0' retorna o valor numérico da string como inteiro
  return msg[0] - '0';
}

// função que lê a string Carta dada pelo gerenciador e retorna uma Carta
Carta get_card(char *msg) {
  Carta carta;
  carta.numero = get_number(msg);

  char *naipe = malloc(sizeof(char));
  // fazendo a cópia da string para quando o ponteiro original for mudado não afetar a carta
  strcpy(naipe, msg);

  carta.naipe = get_suit(naipe);

  return carta;
}

// função que compra uma Carta e a adiciona ao array de Cartas
Carta *buy_card(Carta *cartas, int *qtd_cartas, char *msg) {
  (*qtd_cartas)++;
  cartas = realloc(cartas, (*qtd_cartas) * sizeof(cartas));

  cartas[(*qtd_cartas) - 1] = get_card(msg);
  return cartas;
}

// função que recebe do gerenciador as cartas iniciais e retorna um array de Cartas
Carta *get_initial_hand(char *msg, int *qtd_cartas) {
  Carta *cartas;
  for (int i = 0; msg[i] != '\0'; i++) {
    // contando o número de cartas a partir dos espaços
    if (msg[i] == ' ') {
      (*qtd_cartas)++;
    }
  }

  cartas = malloc((*qtd_cartas) * sizeof(Carta));
  char *aux = malloc(strlen(msg) * sizeof(char));
  strcpy(aux, msg);

  // strtok separa a string em tokens a partir do delimitador " "
  char *carta = strtok(aux, " ");
  for (int i = 0; i < (*qtd_cartas); i++) {
    // pegando os valores das cartas 1 a 1
    carta = strtok(NULL, " ");
    cartas[i] = get_card(carta);
  }
  return cartas;
}

// função que atualiza a mão após o descarte de uma carta
Carta *update_cards(Carta *cartas, int *qtd_cartas, int index) {
  for (int i = 0; i < (*qtd_cartas); i++) {
    if (i == index) {
      for (int j = i; j < (*qtd_cartas); j++) {
        cartas[j] = cartas[j + 1];
      }
    }
  }

  (*qtd_cartas)--;
  cartas = realloc(cartas, (*qtd_cartas) * sizeof(Carta));

  return cartas;
}

// função que trata o que deve acontecer quando dada carta for descartada e retorna a mão atualizada
Carta *discard_and_update_cards(Carta *cartas, Carta descartada, int *qtd_cartas, int index, int *joguei_pra_comprar, Carta *na_mesa) {
  if (descartada.numero == A || descartada.numero == C) {
    int index_prox_naipe = index == ((*qtd_cartas) - 1) ? 0 : index + 1;
    // mandando para os outros bots lerem
    printf("DISCARD %s%s %s\n", numeros[descartada.numero - 1], naipes[descartada.naipe], naipes[cartas[index_prox_naipe].naipe]);
    na_mesa->naipe = read_suit(naipes[cartas[index_prox_naipe].naipe]);
    cartas = update_cards(cartas, qtd_cartas, index);
    *joguei_pra_comprar = 1;
  } else {
    // mandando para os outros bots lerem
    printf("DISCARD %s%s\n", numeros[descartada.numero - 1], naipes[descartada.naipe]);
    cartas = update_cards(cartas, qtd_cartas, index);
    *joguei_pra_comprar = 0;
  }

  return cartas;
}

// função que verifica se uma carta jogada por outro bot muda o naipe e, portanto, tem 2 complemento
int card_changes_suit(Carta carta) {
  if (carta.numero == A || carta.numero == C) {
    return 1;
  }
  return 0;
}
