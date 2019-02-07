#include "dominion.h"
#include "dominion_helpers.h"

#include "test_helpers.h"

#include <limits.h>
#include <stdio.h>

//smithy test
int main(){

    int bonus;
    int i;

    //initialize gameState, 2 player game
    int kCards[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState gs;
    //struct gameState gsOrig;
    struct gameState gsCopy;
    initializeGame(2, kCards, 1, &gs);
    //gsOrig = gs;

    //add smithy to player 1 hand
    gs.whoseTurn = 0;
    gs.hand[0][gs.handCount[0]++] = smithy;

    //play smithy with more than three cards in deck
    while (gs.deckCount[0] < 3){
        gs.deck[0][gs.deckCount[0]++] = copper;
    }
    gsCopy = gs;

    printf("Starting hand:\n");
    debugPrintPlayerHand(&gs, 0);

    cardEffect(smithy, 0, 0, 0, &gs, gs.handCount[0]-1, &bonus);

    //check handCount up 2 (+3 cards - 1 Smithy)
    checkValue(gs.handCount[0], gsCopy.handCount[0] + 2, "hand count up 2", 1);

    //check deckCount
    checkValue(gs.deckCount[0], gsCopy.deckCount[0] - 3, "deck down 3 cards", 1);

    //check smithy was added to played crds
    checkValue(gs.playedCardCount, 1, "one card played", 1);
    checkValue(gs.playedCards[0], smithy, "played card is smithy", 1);

    //recreate effect of smithy in gameState copy
    gsCopy.playedCards[gsCopy.playedCardCount++] = smithy; //add a smithy to played cards
    gsCopy.handCount[0]--; //remove smithy from hand
    for (i = 0; i < 3; i++){
        gsCopy.hand[0][gsCopy.handCount[0]++] = gsCopy.deck[0][gsCopy.deckCount[0]--]; //add card from deck into hand
    }

    //check cards in hand (that they match the top three cards of the draw pile)
    checkValue(compareGameStatePlayerHand(&gs, &gsCopy, 0), 1, "correct cards drawn", 1);

    //hands checked already - so zero out hands and check all else in game state hasn't changed
    gsCopy.handCount[0] = 0;
    gs.handCount[0] = 0;
    compareGameState(&gs, &gsCopy);

    return 0;
}