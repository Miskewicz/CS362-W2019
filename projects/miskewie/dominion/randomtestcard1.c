#include "dominion.h"
#include "dominion_helpers.h"
#include "random_test_helpers.h"
#include "test_helpers.h"

#include <stdio.h>
#include <stdlib.h>


#define NUM_TESTS 10000

// print relevant values of the gameState input parameters
void printTestInfo(struct gameState* gs, int testNum, int success){
    if (success){
        printf("Test Success #%i : ", testNum);
    } else {
        printf("******* Test Failed #%i : ", testNum);
    }
    printf("Input #Actions = %i | ", gs->numActions);
    printf("Input HandCount = %i | ", gs->handCount[whoseTurn(gs)]);
    printf("Input DeckCount = %i\n", gs->deckCount[whoseTurn(gs)]);
}

// check that the basic functionality of village matches how the simulated gameState
// was altered. This function is used when the deckCount is 0, so the drawn card must
// be taken from a reshuffled discard. Given that the reshuffling is randomized - we can
// only verify these basic values in this instance. 
int checkGameStateBasic(struct gameState* gsNew, struct gameState* gsOrig){
    int player = whoseTurn(gsNew);
    int success = 1;

    //check handsize matches simulated handsize
    success = checkValue(gsNew->handCount[player], gsOrig->handCount[player], "Hand size", 0) && success;
    
    //check total deck+discard count matches simulated deck+discard count
    success = checkValue(gsNew->deckCount[player] + gsNew->discardCount[player], 
            gsOrig->deckCount[player]+gsOrig->discardCount[player], "total deck+discard", 0) && success;
    
    //check numActions matches simulated numActions
    success = checkValue(gsNew->numActions, gsOrig->numActions, "Number of Actions", 0) && success;

    return(success);
}

// basic simulation of village effect
// does not handle reshuffling discard pile
void simulateVillageEffect(struct gameState* gs, int handPos){
    int player = whoseTurn(gs);

    //draw card
    //only draw if less than MAX_HAND
    if (gs->handCount[player] < MAX_HAND){
        if (gs->deckCount[player] > 0){
            //replace the village card slot with the top card of deck
            //can do this since the comparison function doesn't care about order of cards in hand
            gs->hand[player][handPos] = gs->deck[player][--gs->deckCount[player]];
        } else {
            //no cards in deck - just take the top card of discard
            if(gs->discardCount[player] > 0){
                gs->hand[player][handPos] = gs->discard[player][--gs->discardCount[player]];
            }
        }
    }

    //add two actions
    gs->numActions += 2;

    //add village to played cards
    gs->playedCards[gs->playedCardCount] = village;
    gs->playedCardCount++;
}

//random test for village card
int main(){

    int i;
    struct gameState gs;
    struct gameState gsCopy;
    struct gameState gsOrig;
    int activePlayer;
    int handPos;
    int bonus;
    int successNum = 0;
    int success;

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

        //verify gameStates match - can do a more complete check if no reshuffle was necessary
        if (gsOrig.deckCount[activePlayer] > 0){
            success = compareGameState(&gs, &gsCopy);
            printTestInfo(&gsOrig, i, success);
        } else {
            success = checkGameStateBasic(&gs, &gsCopy);
            printTestInfo(&gsOrig, i, success);
        }
        if (success) successNum++;
    }

    printf("================================\n");
    printf("======== TESTS COMPLETE ========\n");
    printf("  %i / %i Passed\n", successNum, NUM_TESTS);
    printf("================================\n\n");

    return 0;
}