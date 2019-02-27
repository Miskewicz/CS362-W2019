#include "dominion.h"
#include "dominion_helpers.h"
#include "random_test_helpers.h"
#include "test_helpers.h"

#include <stdio.h>
#include <stdlib.h>

#define NUM_TESTS 10000

//count the total treasure cards in the deck+discard of the gameState provided as input
int treasureCountTotal(struct gameState* gs){
    int i;
    int player = whoseTurn(gs);
    int treasureCount = 0;
    for (i = 0; i < gs->deckCount[player]; i++){
        if (gs->deck[player][i] >= copper && gs->deck[player][i] <= gold){
            treasureCount++;
        }
    }
    for (i = 0; i < gs->discardCount[player]; i++){
        if (gs->discard[player][i] >= copper && gs->discard[player][i] <= gold){
            treasureCount++;
        }
    } 
    return (treasureCount);
}

// print relevant values of the gameState input parameters
void printTestInfo(struct gameState* gs, int testNum, int success){
    if (success){
        printf("Test Success #%i : ", testNum);
    } else {
        printf("******* Test Failed #%i : ", testNum);
    }

    printf("Treasure Count in deck+discard = %i | ", treasureCountTotal(gs));
    printf("Input HandCount = %i | ", gs->handCount[whoseTurn(gs)]);
    printf("Input DeckCount = %i\n", gs->deckCount[whoseTurn(gs)]);
}

//check a gameState for expected changes when playing an adventurer card 
//from an original gameState. Can only check some changes as the card effect
//is not completely predictive because of the potential for a deck shuffle.
//This function returns true if ALL checks were successful.
//verifies the following:
//  1. handsize increased correctly
//  2. adventure card was added to played cards correctly
//  3. deck/discard decreased in size correctly
//  4. treasure cards in hand increased by the correct amount
int checkGameStateBasic(struct gameState* gsNew, struct gameState* gsOrig){
    int player = whoseTurn(gsNew);
    int success = 1;
    int treasureCount = 0;
    int i;
    int treasureInHand = 0;
    int treasureInHandOrig = 0;

    //count treasures in deck+discard
    treasureCount = treasureCountTotal(gsOrig);

    //Number of treasure to draw
    if (treasureCount > 2) treasureCount = 2;

    //check handsize increased by treasureCount - 1
    success = checkValue(gsNew->handCount[player], gsOrig->handCount[player] + treasureCount - 1, "Hand size", 0) && success;
    
    //played card increased by 1
    success = checkValue(gsNew->playedCardCount, gsOrig->playedCardCount + 1, "Card added to played cards", 0) && success;

    //check played card is adventurer
    success = checkValue(gsNew->playedCards[gsNew->playedCardCount - 1], adventurer, "Played card is adventurer", 0) && success;

    //total deck/discard decreased by treasureCount
    success = checkValue(gsNew->deckCount[player] + gsNew->discardCount[player], 
                                    gsOrig->deckCount[player] + gsOrig->discardCount[player] - treasureCount, 
                                    "Total deck size decreased by drawn treasures", 0) && success;

    //count treasure cards in hand of new gameState
    for (i = 0; i < gsNew->handCount[player]; i++){
        if (gsNew->hand[player][i] >= copper && gsNew->hand[player][i] <= gold){
            treasureInHand++;
        }
    }
    //count treasure cards in hand of original gameState
    for (i = 0; i < gsOrig->handCount[player]; i++){
        if (gsOrig->hand[player][i] >= copper && gsOrig->hand[player][i] <= gold){
            treasureInHandOrig++;
        }
    }
    
    //check treasures in hand increased by treasureCount
    success = checkValue(treasureInHand, treasureInHandOrig + treasureCount, "Additional treasures in hand", 0) && success;

    return(success);
}


//random test for adventurer card
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

    //signal(SIGALRM, timeoutCatch);//set a handler to catch alarms for infinite loops

    for(i = 0; i < NUM_TESTS; i++){
        //randomize gameState
        randomizeGameState(&gs);

        //set the active player
        activePlayer = randomActivePlayer(&gs);
        
        //add adventurer to a random position in players hand
        handPos = addCardToHandRandom(&gs, activePlayer, adventurer);

        //copy the gameState prior to effect
        gsCopy = gs;
        gsOrig = gs;

        cardEffect(adventurer, 0, 0, 0, &gs, handPos, &bonus);

        success = checkGameStateBasic(&gs, &gsCopy);
        printTestInfo(&gsOrig, i, success);

        if (success) successNum++;
    }

    printf("================================\n");
    printf("======== TESTS COMPLETE ========\n");
    printf("  %i / %i Passed\n", successNum, NUM_TESTS);
    printf("================================\n\n");

    return 0;
}