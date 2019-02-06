#include "dominion.h"
#include "dominion_helpers.h"

#include "test_helpers.h"

#include <limits.h>
#include <stdio.h>

//empty hand/deck/discard for each player - score should be 0 for each player
void checkAllEmpty(struct gameState* gs){
    int i;
    struct gameState gsCopy;
    for (i = 0; i < MAX_PLAYERS; i++){
        gs->handCount[i] = 0;
        gs->discardCount[i] = 0;
        gs->deckCount[i] = 0;
        gsCopy = *gs;
        checkValue(scoreFor(i, gs), 0, "Empty deck, hand, and discard", 1);
        compareGameState(gs, &gsCopy); //should be no state change
    }
}

//single card in hande
int checkSingleInHand(struct gameState* gs, int c, int expectedScore, char* msg){
    int i;
    struct gameState gsCopy;
    int failure = 0;
    for (i = 0; i < MAX_PLAYERS; i++){
        gs->handCount[i] = 1;
        gs->discardCount[i] = 0;
        gs->deckCount[i] = 0;
        gs->hand[i][0] = c;
        gsCopy = *gs;
        if (!checkValue(scoreFor(i, gs), expectedScore, msg, 0)) failure = 1;
        compareGameState(gs, &gsCopy); //should be no state change
    }
    return !failure;
}

//single card in discard pile
int checkSingleInDiscard(struct gameState* gs, int c, int expectedScore, char* msg){
    int i;
    struct gameState gsCopy;
    int failure = 0;
    for (i = 0; i < MAX_PLAYERS; i++){
        gs->handCount[i] = 0;
        gs->discardCount[i] = 1;
        gs->deckCount[i] = 0;
        gs->discard[i][0] = c;
        gsCopy = *gs;
        if (!checkValue(scoreFor(i, gs), expectedScore, msg, 0)) failure = 1;
        compareGameState(gs, &gsCopy); //should be no state change
    }
    return !failure;
}

//single card in deck
int checkSingleInDeck(struct gameState* gs, int c, int expectedScore, char* msg){
    int i;
    struct gameState gsCopy;
    int failure = 0;
    for (i = 0; i < MAX_PLAYERS; i++){
        gs->handCount[i] = 0;
        gs->discardCount[i] = 0;
        gs->deckCount[i] = 1;
        gs->deck[i][0] = c;
        gsCopy = *gs;
        if (!checkValue(scoreFor(i, gs), expectedScore, msg, 0)) failure = 1;
        compareGameState(gs, &gsCopy); //should be no state change
    }
    return !failure;
}

//all full of the same card - if estate, the score should be MAX_HAND + MAX_DECK*2
void checkAllFull(struct gameState* gs, int c, int expectedScore, char* msg){
    int i;
    int j;
    struct gameState gsCopy;

    for (i = 0; i < MAX_PLAYERS; i++){
        gs->handCount[i] = MAX_HAND;
        gs->discardCount[i] = MAX_DECK;
        gs->deckCount[i] = MAX_DECK;

        for (j = 0; j < gs->handCount[i]; j++){
            gs->hand[i][j] = c;
        }
        for (j = 0; j < gs->discardCount[i]; j++){
            gs->discard[i][j] = c;
        }
        for (j = 0; j < gs->deckCount[i]; j++){
            gs->deck[i][j] = c;
        }

        gsCopy = *gs;
        checkValue(scoreFor(i, gs), expectedScore, msg, 1);
        compareGameState(gs, &gsCopy); //should be no state change
    }
}

//returns the card score for a SINGLE copy of the card
int getCardScore(int c){
    switch(c){
        case estate:
            return 1;
        case duchy:
            return 3;
        case province:
            return 6;
        case curse:
            return -1;
        case great_hall:
            return 1;
        default:
            return 0;
    }
}


//Unit tests for the scoreFor function
int main(){
    int i;
    char buffer[256];

    //initialize gameState, 2 player game
    int kCards[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState gs;
    struct gameState gsOrig;
    initializeGame(2, kCards, 1, &gs);
    gsOrig = gs;

    checkAllEmpty(&gs); //check with all empty piles - score should be 0


    //check single card in hand
    for (i = curse; i <= treasure_map; i++){
        sprintf(buffer, "single card in hand id: %i", i);
        if (checkSingleInHand(&gs, i, getCardScore(i), buffer)){
            printf("%s - SUCCESS\n", buffer);
        } else {
            printf("%s - FAILURE\n", buffer);
        }
    }

    //check single card in discard
    for (i = curse; i <= treasure_map; i++){
        sprintf(buffer, "single card in discard id: %i", i);
        if (checkSingleInDiscard(&gs, i, getCardScore(i), buffer)){
            printf("%s - SUCCESS\n", buffer);
        } else {
            printf("%s - FAILURE\n", buffer);
        }
    }

    //check single card in deck
    for (i = curse; i <= treasure_map; i++){
        sprintf(buffer, "single card in deck id: %i", i);
        if (checkSingleInDeck(&gs, i, getCardScore(i), buffer)){
            printf("%s - SUCCESS\n", buffer);
        } else {
            printf("%s - FAILURE\n", buffer);
        }
    }

    gs = gsOrig;
    checkAllFull(&gs, estate, MAX_HAND + MAX_DECK*2, "All full of estates");


    return 0;
}