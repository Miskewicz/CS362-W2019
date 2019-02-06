#include "dominion.h"
#include "dominion_helpers.h"

#include "test_helpers.h"

#include <limits.h>

//empty hand/deck/discard for each player - score should be 0 for each player
void checkAllEmpty(struct gameState* gs){
    int i;
    struct gameState gsCopy;
    for (i = 0; i < MAX_PLAYERS; i++){
        gs->handCount[i] = 0;
        gs->discardCount[i] = 0;
        gs->deckCount[i] = 0;
        gsCopy = *gs;
        checkValue(scoreFor(i, gs), 0, "Empty deck, hand, and discard");
        compareGameState(gs, &gsCopy); //should be no state change
    }
}

//single card in hande
void checkSingleInHand(struct gameState* gs, int c, int expectedScore, char* msg){
    int i;
    struct gameState gsCopy;
    for (i = 0; i < MAX_PLAYERS; i++){
        gs->handCount[i] = 1;
        gs->discardCount[i] = 0;
        gs->deckCount[i] = 0;
        gs->hand[i][0] = c;
        gsCopy = *gs;
        checkValue(scoreFor(i, gs), expectedScore, msg);
        compareGameState(gs, &gsCopy); //should be no state change
    }
}

//single card in discard pile
void checkSingleInDiscard(struct gameState* gs, int c, int expectedScore, char* msg){
    int i;
    struct gameState gsCopy;
    for (i = 0; i < MAX_PLAYERS; i++){
        gs->handCount[i] = 0;
        gs->discardCount[i] = 1;
        gs->deckCount[i] = 0;
        gs->discard[i][0] = c;
        gsCopy = *gs;
        checkValue(scoreFor(i, gs), expectedScore, msg);
        compareGameState(gs, &gsCopy); //should be no state change
    }
}

//single card in deck
void checkSingleInDeck(struct gameState* gs, int c, int expectedScore, char* msg){
    int i;
    struct gameState gsCopy;
    for (i = 0; i < MAX_PLAYERS; i++){
        gs->handCount[i] = 0;
        gs->discardCount[i] = 0;
        gs->deckCount[i] = 1;
        gs->deck[i][0] = c;
        gsCopy = *gs;
        checkValue(scoreFor(i, gs), expectedScore, msg);
        compareGameState(gs, &gsCopy); //should be no state change
    }
}

//all full of estates - score should be MAX_HAND + MAX_DECK*2
void checkAllFull(struct gameState* gs, int c, int expectedScore, char* msg){
    int i;
    int j;
    struct gameState gsCopy;

    for (i = 0; i < MAX_PLAYERS; i++){
        gs->handCount[i] = MAX_HAND;
        gs->discardCount[i] = MAX_DECK;
        gs->deckCount[i] = MAX_DECK;

        for (j = 0; j < gs->handCount[i]; j++){
            gs->hand[i][j] = estate;
        }
        for (j = 0; j < gs->discardCount[i]; j++){
            gs->discard[i][j] = estate;
        }
        for (j = 0; j < gs->deckCount[i]; j++){
            gs->deck[i][j] = estate;
        }

        gsCopy = *gs;
        checkValue(scoreFor(i, gs), expectedScore, msg);
        compareGameState(gs, &gsCopy); //should be no state change
    }
}

//Unit tests for the scoreFor function
int main(){
    int i; //iterator
    int j;
    
    //initialize gameState, 2 player game
    int kCards[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState gs;
    struct gameState gsOrig;
    struct gameState gsCopy;
    initializeGame(2, kCards, 1, &gs);
    gsOrig = gs;

    checkAllEmpty(&gs); //check with all empty piles - score should be 0
    checkSingleInHand(&gs, estate, 1, "Single estate in hand"); //single estate in hand - score should be 1
    checkSingleInHand(&gs, duchy, 3, "Single duchy in hand"); //single estate in hand - score should be 3
    checkSingleInHand(&gs, province, 6, "Single province in hand"); //single estate in hand - score should be 6
    checkSingleInHand(&gs, curse, -1, "Single curse in hand"); //single estate in hand - score should be -1
    checkSingleInHand(&gs, great_hall, 1, "Single great hall in hand"); //single estate in hand - score should be 1
    checkSingleInHand(&gs, gardens, 0, "Single gardens in hand"); //single gardens in hand - score should be 0

    checkSingleInDiscard(&gs, estate, 1, "Single estate in discard pile"); //single estate in discard pile - score should be 1
    checkSingleInDiscard(&gs, duchy, 3, "Single duchy in discard pile"); //single estate in discard pile - score should be 3
    checkSingleInDiscard(&gs, province, 6, "Single province in discard pile"); //single estate in discard pile - score should be 6
    checkSingleInDiscard(&gs, curse, -1, "Single curse in discard pile"); //single estate in discard pile - score should be -1
    checkSingleInDiscard(&gs, great_hall, 1, "Single great hall in discard pile"); //single estate in discard pile - score should be 1
    checkSingleInDiscard(&gs, gardens, 0, "Single gardens in discard pile"); //single gardens in discard pile - score should be 0

    checkSingleInDeck(&gs, estate, 1, "Single estate in deck"); //single estate in deck - score should be 1
    checkSingleInDeck(&gs, duchy, 3, "Single duchy in deck"); //single estate in deck - score should be 3
    checkSingleInDeck(&gs, province, 6, "Single province in deck"); //single estate in deck - score should be 6
    checkSingleInDeck(&gs, curse, -1, "Single curse in deck"); //single estate in deck - score should be -1
    checkSingleInDeck(&gs, great_hall, 1, "Single great hall in deck"); //single estate in deck - score should be 1
    checkSingleInDeck(&gs, gardens, 0, "Single gardens in deck"); //single gardens in deck - score should be 0

    gs = gsOrig;
    checkAllFull(&gs, estate, MAX_HAND + MAX_DECK*2, "All full of estates");


    return 0;
}