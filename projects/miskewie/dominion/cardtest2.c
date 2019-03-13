#include "dominion.h"
#include "dominion_helpers.h"

#include "test_helpers.h"

#include <limits.h>
#include <stdio.h>


//adventurer test
int main(){
    int i;
    int bonus;

    //initialize gameState, 2 player game
    int kCards[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState gs;
    struct gameState gsOrig;
    struct gameState gsCopy;
    initializeGame(2, kCards, 1, &gs);

    //add 1 adventurer to hand
    gs.whoseTurn = 0;
    gs.hand[0][gs.handCount[0]++] = adventurer;
    gsOrig = gs; //save copy

    printf("\nTest 1: Two treasure cards on top of deck\n");
    printf("========================================\n");
    for(i = copper; i <= gold; i++){
        gs = gsOrig; //reset to original
        //add two to top of deck
        gs.deck[0][gs.deckCount[0]++] = i;
        gs.deck[0][gs.deckCount[0]++] = i;

        gsCopy = gs;
        checkValue(cardEffect(adventurer, 0, 0, 0, &gs, gs.handCount[0] - 1, &bonus), 0, "function returned success", 0);

        //check hand + 1 cards (+2 treasure, -1 adventurer)
        checkValue(gs.handCount[0], gsCopy.handCount[0] + 1, "hand increased by 1", 1);

        //check deck - 2 cards
        checkValue(gs.deckCount[0], gsCopy.deckCount[0] - 2, "deck down 2 cards", 1);

        //check two cards in hand equal expected treasure type
        checkValue(gs.hand[0][gs.handCount[0] - 1], i, "first card is expected treasure", 1);
        checkValue(gs.hand[0][gs.handCount[0] - 2], i, "second card is expected treasure", 1);

        //no cards discarded
        checkValue(gs.discardCount[0], gsCopy.discardCount[0], "discard count the same", 1);

        //played card is adventurer
        checkValue(gs.playedCardCount, 1, "one played card", 1);
        checkValue(gs.playedCards[0], adventurer, "played card is adventurer", 1);

        //build copy to be expected game state
        gsCopy.deckCount[0] = gsCopy.deckCount[0] - 2;
        gsCopy.handCount[0] = gsCopy.handCount[0] + 1;
        gsCopy.hand[0][gsCopy.handCount[0] - 1] = i;
        gsCopy.hand[0][gsCopy.handCount[0] - 2] = i;
        gsCopy.playedCardCount = 1;
        gsCopy.playedCards[0] = adventurer;

        //make sure no other changes
        compareGameState(&gs, &gsCopy);
    }

    gs = gsOrig;
    printf("\nTest 2: 1 non treasure card on top of deck, then 2 treasure cards\n");
    printf("========================================\n");
    //add a copper
    gs.deck[0][gs.deckCount[0]++] = copper;
    //add a copper
    gs.deck[0][gs.deckCount[0]++] = copper;
    //add a duchy
    gs.deck[0][gs.deckCount[0]++] = duchy;

    gsCopy = gs; //store copy
    checkValue(cardEffect(adventurer, 0, 0, 0, &gs, gs.handCount[0] - 1, &bonus), 0, "function returned success", 0); //play card

    //check hand + 1 cards (+2 treasure, -1 adventurer)
    checkValue(gs.handCount[0], gsCopy.handCount[0] + 1, "hand increased by 1", 1);

    //check deck - 3 cards
    checkValue(gs.deckCount[0], gsCopy.deckCount[0] - 3, "deck down 3 cards", 1);

    //check two cards in hand equal expected treasure type
    checkValue(gs.hand[0][gs.handCount[0] - 1], copper, "first card is expected treasure", 1);
    checkValue(gs.hand[0][gs.handCount[0] - 2], copper, "second card is expected treasure", 1);

    //1 card discarded
    checkValue(gs.discardCount[0], gsCopy.discardCount[0] + 1, "discard up 1 card", 1);

    //played card is adventurer
    checkValue(gs.playedCardCount, 1, "one played card", 1);
    checkValue(gs.playedCards[0], adventurer, "played card is adventurer", 1);

    gs = gsOrig;
    printf("\nTest 3: One treasure card on top of deck, then 20 non treasure cards, then treasure card\n");
    printf("========================================\n");
    //add a copper
    gs.deck[0][gs.deckCount[0]++] = copper;
    //add 20 duchy
    for (i = 0; i < 20; i++){
        gs.deck[0][gs.deckCount[0]++] = duchy;
    }
    //add a copper
    gs.deck[0][gs.deckCount[0]++] = copper;

    gsCopy = gs; //store copy
    checkValue(cardEffect(adventurer, 0, 0, 0, &gs, gs.handCount[0] - 1, &bonus), 0, "function returned success", 0); //play card

    //check hand + 1 cards (+2 treasure, -1 adventurer)
    checkValue(gs.handCount[0], gsCopy.handCount[0] + 1, "hand increased by 1", 1);

    //check deck - 22 cards
    checkValue(gs.deckCount[0], gsCopy.deckCount[0] - 22, "deck down 22 cards", 1);

    //check two cards in hand equal expected treasure type
    checkValue(gs.hand[0][gs.handCount[0] - 1], copper, "first card is expected treasure", 1);
    checkValue(gs.hand[0][gs.handCount[0] - 2], copper, "second card is expected treasure", 1);

    //20 cards discarded
    checkValue(gs.discardCount[0], gsCopy.discardCount[0] + 20, "discard up 20 cards", 1);

    //played card is adventurer
    checkValue(gs.playedCardCount, 1, "one played card", 1);
    checkValue(gs.playedCards[0], adventurer, "played card is adventurer", 1);


    return 0;
}