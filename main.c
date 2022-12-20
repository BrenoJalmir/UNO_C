#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CardThings/CardClasses.h"
#include "CardThings/CardFunctions.c"

#define MAX_LINE 100
#define MAX_ACTION 10
#define MAX_ID_SIZE 10

int main() {
  char temp[MAX_LINE];
  char my_id[MAX_ID_SIZE];
  Carta *cartas_bot;
  // num_cartas começa com -1 pois o modo de leitura da quantidade é a partir da quantidade de espaços
  // na formatação, que é equivalente ao número de cartas dadas + 1
  int num_cartas = -1;
  int joguei_pra_comprar = 0;
  int comprar;

  setbuf(stdin, NULL);
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);

  scanf("PLAYERS %[^\n]\n", temp);

  scanf("YOU %s\n", my_id);

  scanf("HAND %[^\n]\n", temp);
  // todo: ler a mão inicial do bot
  // padrão 7 cartas, possível variável
  cartas_bot = get_initial_hand(temp, &num_cartas);

  scanf("TABLE %s\n", temp);

  // ler a carta da mesa
  Carta na_mesa = get_card(temp);

  char id[MAX_ID_SIZE];
  char action[MAX_ACTION];
  char previous_action[MAX_ACTION];
  char complement[MAX_LINE];

  while (1) {
    do {
      strcpy(previous_action, action);
      scanf("%s %s", action, complement);

      // verificar qual(is) a(s) ação(ões) performada(s) pelos outros bots enquanto não chega a vez do nosso
      // discartou? mudar a carta da mesa
      if (strcmp(action, "DISCARD") == 0) {
        na_mesa = get_card(complement);
        if (card_changes_suit(na_mesa)) {
          scanf("%s\n", complement);
          na_mesa.naipe = read_suit(complement);
        }
      }
      // comprou? ler a carta comprada pelo outro bot : não é lida
      if (strcmp(action, "BUY") == 0) {
        // não precisa fazer nada aqui
      }

    } while (strcmp(action, "TURN") || strcmp(complement, my_id));

    // tudo acima debugado, esperançosamente 🙏

    // depois

    // se o bot anterior jogou uma carta de ação de compra, comprar a quantidade especificada
    comprar = na_mesa.numero == V ? 2 : na_mesa.numero == C ? 4
                                                            : 0;
    comprar = strcmp(previous_action, "BUY") == 0 ? 0 : comprar;
    if (comprar > 0 && joguei_pra_comprar == 0 && strcmp(previous_action, "BUY") != 0) {
      if (comprar == 4) {
        printf("SAY explodi\n");
      }
      // mandando para os outros bots lerem
      printf("BUY %d\n", comprar);
      for (int i = 0; i < comprar; i++) {
        scanf(" %[^\n]\n", complement);
        cartas_bot = buy_card(cartas_bot, &num_cartas, complement);
      }
    } else {
      // se o nosso bot jogar V ou C, mudar joguei_pra_comprar pra 1 se não 0
      comprar = 1;

      for (int i = 0; i < num_cartas; i++) {
        // verificar se há cartas na mão que podem ser jogadas na mesa
        if (can_be_played(cartas_bot[i], na_mesa)) {
          // a carta da mesa vira a descartada
          na_mesa = cartas_bot[i];
          // se houver, descarta uma carta e atualiza a mão
          cartas_bot = discard_and_update_cards(cartas_bot, cartas_bot[i], &num_cartas, i, &joguei_pra_comprar, &na_mesa);
          // como o bot jogou não há necessidade de comprar
          if (num_cartas == 1)
            printf("SAY Essa aqui é caixa já\n");
          else if (num_cartas >= 20)
            printf("SAY Aqui virou bagunça\n");
          comprar = 0;
          break;
        }
      }
      if (comprar > 0) {
        // compra 1 carta pois não possui cartas na mão que possam ser jogadas
        // mandando para os outros bots lerem
        printf("BUY 1\n");
        scanf(" %[^\n]\n", complement);
        cartas_bot = buy_card(cartas_bot, &num_cartas, complement);
      }
    }
  }

  free(cartas_bot);
  return 0;
}
