#include "dominion.h"
#include "dominion_helpers.h"
#include "random_test_helpers.h"
#include "test_helpers.h"

#include <stdio.h>
#include <stdlib.h>


void simulateVillageEffect(struct gameState* gs, int handPos){
    int player = whoseTurn(gs);

    //draw card
    //replace the village card slot with the top card of deck
    //can do this since the comparison function doesn't care about order of cards in hand
    gs->hand[player][handPos] = gs->deck[player][--gs->deckCount[player]];

    //add two actions
    gs->numActions += 2;

    //add village to played cards
    gs->playedCards[gs->playedCardCount] = village;
    gs->playedCardCount++;
}

//random test for village card
int main(){

    int i;
    int NUM_TESTS = 10;
    struct gameState gs;
    struct gameState gsCopy;
    struct gameState gsOrig;
    int activePlayer;
    int handPos;
    int bonus;

    for(i = 0; i < NUM_TESTS; i++){
        //randomize gameState
        randomizeGameState(&gs);

        //set the active player
        activePlayer = randomActivePlayer(&gs);
        
        //add village to a random position in players hand
        handPos = addCardToHandRandom(&gs, activePlayer, village);

        //copy the gameState prior to effect
        gsCopy = gs;
        gsOrig = gs;

        //run the effect
        cardEffect(village, 0, 0, 0, &gs, handPos, &bonus);

        //simulate effect in copy
        simulateVillageEffect(&gsCopy, handPos);

        //verify gameStates match
        if (!compareGameState(&gs, &gsCopy)){
            printf("========= Test Failed #%i ==========\n", i);
            printf("Input #Actions = %i\n", gsOrig.numActions);
            printf("Input HandCount = %i\n", gsOrig.handCount[activePlayer]);
            printf("Input DeckCount = %i\n", gsOrig.deckCount[activePlayer]);
            printf("====================================\n");
        }
    }

    return 0;
}