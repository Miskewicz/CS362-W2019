#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int randBetween(int min, int max){
    return(rand() % (max - min + 1) + min);
}

int getRandomCard(){
    return(randBetween(curse, treasure_map));
}


int randomActivePlayer(struct gameState* gs){
    int activePlayer;

    activePlayer = randBetween(0, gs->numPlayers - 1);
    gs->whoseTurn = activePlayer;

    return activePlayer;
}


int addCardToHandRandom(struct gameState* gs, int player, int card){
    int handPos;

    //if there are NO cards in hand, add it to the player's hand as the only card
    if (gs->handCount[player] == 0){
        gs->handCount[player] = 1;
        gs->hand[player][0] = card;
        return 0;
    }
    //otherwise replace a random card in the hand with the desired card
    else {
        handPos = randBetween(0, gs->handCount[player] - 1);
        gs->hand[player][handPos] = card;
        return handPos;
    }
}


void randomizeSupply(struct gameState* gs){
    int i;
    for(i = curse; i <= treasure_map; i++){
        gs->supplyCount[i] = randBetween(0, MAX_DECK);
    }
}

void randomizePlayedCards(struct gameState* gs){
    int i;

    //played card count
    gs->playedCardCount = randBetween(0, MAX_HAND);

    //populate played cards
    for(i = 0; i < gs->playedCardCount; i++){
        gs->playedCards[i] = getRandomCard();
    }
}

void randomizeDiscard(struct gameState* gs, int player){
    int i;

    //discard size
    gs->discardCount[player] = randBetween(0, MAX_DECK);

    //populate discard pile
    for(i = 0; i < gs->discardCount[player]; i++){
        gs->discard[player][i] = getRandomCard();
    }
}

void randomizeHand(struct gameState* gs, int player){
    int i;

    //hand size
    gs->handCount[player] = randBetween(0, MAX_HAND);

    //populate hand
    for(i = 0; i < gs->handCount[player]; i++){
        gs->hand[player][i] = getRandomCard();
    }
}

void randomizeDeck(struct gameState* gs, int player){
    int i;
    
    //deck size
    gs->deckCount[player] = randBetween(0, MAX_DECK);

    //populate deck
    for(i = 0; i < gs->deckCount[player]; i++){
        gs->deck[player][i] = getRandomCard();
    }
}


//generate a completely random gameState
void randomizeGameState(struct gameState* gs){

    int i;

    //zero gameState
    memset(gs, 0, sizeof(struct gameState));

    //random players
    gs->numPlayers = randBetween(2, MAX_PLAYERS);

    //for each player
    for(i = 0; i < gs->numPlayers; i++){
        //random deck
        randomizeDeck(gs, i);

        //random hand
        randomizeHand(gs, i);

        //random discard
        randomizeDiscard(gs, i);
    }

    //played cards
    randomizePlayedCards(gs);

    //supply
    randomizeSupply(gs);

    //randomize other gameState values
    gs->numActions = randBetween(0, 25);
    gs->numBuys = randBetween(0, 25);
    gs->coins = randBetween(0, 100);
}