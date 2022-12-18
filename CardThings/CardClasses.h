#ifndef CARD_DEFINITIONS
#define CARD_DEFINITIONS

typedef enum {
  A = 1,
  V = 11,
  D = 12,
  R = 13,
  C = 14
} Numero;

typedef enum {
  OUROS,
  COPAS,
  PAUS,
  ESPADAS
} Naipe;

typedef struct {
  Numero numero;
  Naipe naipe;
} Carta;

#endif
