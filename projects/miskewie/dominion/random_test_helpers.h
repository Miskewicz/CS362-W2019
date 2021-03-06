#ifndef _RANDOM_TEST_HELPERS_H
#define _RANDOM_TEST_HELPERS_H

#include "dominion.h"



int randBetween(int, int);
int getRandomCard();

int addCardToHandRandom(struct gameState* gs, int player, int card);
int randomActivePlayer(struct gameState* gs);

void randomizeSupply(struct gameState*);
void randomizePlayedCards(struct gameState*);
void randomizeDiscard(struct gameState*, int);
void randomizeHand(struct gameState*, int);
void randomizeDeck(struct gameState*, int);

//generate a completely random gameState
void randomizeGameState(struct gameState*);

#endif