#include "dominion.h"
#include "dominion_helpers.h"
#include "random_test_helpers.h"
#include "test_helpers.h"

#include <stdio.h>
#include <stdlib.h>


#define NUM_TESTS 10000

// print relevant values of the gameState input parameters
void printTestInfo(struct gameState* gs, int testNum, int success, int trashCardPos, int gainCard){
    int trashCard = gs->hand[whoseTurn(gs)][trashCardPos];
    
    if (success){
        //printf("Test Success #%i : ", testNum);
        return;
    } else {
        printf("******* Test Failed #%i : ", testNum);
    }
    printf("Input HandCount = %i | ", gs->handCount[whoseTurn(gs)]);
    printf("Input trash Card = %i | ", trashCard);
    printf("Input gain Card = %i | ", gainCard);
    printf("Input gain Card Supply Count = %i\n", gs->supplyCount[gainCard]);
}

// basic simulation of mine effect
int simulateMineEffect(struct gameState* gs, int handPos, int trashCardPos, int gainCard){
    int player = whoseTurn(gs);
    int trashCard = gs->hand[player][trashCardPos];

    //both trash and gain cards must be treasure cards
    if(trashCard < copper || trashCard > gold) return -1;
    if(gainCard < copper || gainCard > gold) return -1;
    //card to gain must not cost more than three more than card to trash
    if(getCost(gainCard) - getCost(trashCard) > 3) return -1;
    //gain card must be in supply
    if(gs->supplyCount[gainCard] <= 0) return -1;

    //add the gain card to the hand - overwrite the trash card position
    gs->hand[player][trashCardPos] = gainCard;
    //lower supply of gaincard
    gs->supplyCount[gainCard]--;

    //play the mine card
    gs->playedCards[gs->playedCardCount++] = mine;

    //remove mine card from player hand
    if(handPos == gs->handCount[player] - 1){
        gs->handCount[player]--;
    } else {
        gs->hand[player][handPos] = gs->hand[player][--gs->handCount[player]];
    }

    return 0;
}

//random test for mine card
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
    int trashSelection;
    int gainSelection;
    int actualResult;
    int expectedResult;

    for(i = 0; i < NUM_TESTS; i++){

        //get a random gameState where the activePlayer's hand is not 0
        do{
            //randomize gameState
            randomizeGameState(&gs);

            //set the active player
            activePlayer = randomActivePlayer(&gs);
        } while(gs.handCount[activePlayer] == 0);

        //add mine to a random position in players hand
        handPos = addCardToHandRandom(&gs, activePlayer, mine);

        //copy the gameState prior to effect
        gsCopy = gs;
        gsOrig = gs;

        //get a random card selection that is NOT the mine being played
        do{
            trashSelection = randBetween(0, gs.handCount[activePlayer]);
        } while (trashSelection == handPos);
        //pick a random card to gain
        gainSelection = getRandomCard();

        //run the effect
        actualResult = cardEffect(mine, trashSelection, gainSelection, 0, &gs, handPos, &bonus);

        //simulate effect in copy
        expectedResult = simulateMineEffect(&gsCopy, handPos, trashSelection, gainSelection);

        //check if function gave correct result
        if (actualResult != expectedResult){
            success = 0;
            printTestInfo(&gsOrig, i, success, trashSelection, gainSelection);
        } else {
            //if function gave correct result - check gameState
            success = compareGameState(&gs, &gsCopy);
            printTestInfo(&gsOrig, i, success, trashSelection, gainSelection);
        }

        if (success) successNum++;
    }

    printf("================================\n");
    printf("======== TESTS COMPLETE ========\n");
    printf("  %i / %i Passed\n", successNum, NUM_TESTS);
    printf("================================\n\n");

    return 0;
}