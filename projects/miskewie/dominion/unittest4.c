#include "dominion.h"
#include "dominion_helpers.h"

#include "test_helpers.h"

#include <limits.h>
#include <stdio.h>

//fullDeckCount test
int main(){
    int i;

    //initialize gameState, 2 player game
    int kCards[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState gs;
    struct gameState gsOrig;
    struct gameState gsCopy;
    initializeGame(2, kCards, 1, &gs);
    gsOrig = gs;

    //All empty piles - should return 0 for every card type
    gs.handCount[0] = 0;
    gs.discardCount[0] = 0;
    gs.deckCount[0] = 0;
    gsCopy = gs;
    checkValue(fullDeckCount(0, adventurer, &gs), 0, "all empty piles", 1);
    compareGameState(&gs, &gsCopy); //should be no state change

    //one copy of card in hand - should return 1
    gs = gsOrig;
    gs.handCount[0] = 1;
    gs.discardCount[0] = 0;
    gs.deckCount[0] = 0;
    gs.hand[0][0] = adventurer;
    gsCopy = gs;
    checkValue(fullDeckCount(0, adventurer, &gs), 1, "one copy of card in hand", 1);
    compareGameState(&gs, &gsCopy); //should be no state change

    //one copy of card in discard - should return 1
    gs = gsOrig;
    gs.handCount[0] = 0;
    gs.discardCount[0] = 1;
    gs.deckCount[0] = 0;
    gs.discard[0][0] = adventurer;
    gsCopy = gs;
    checkValue(fullDeckCount(0, adventurer, &gs), 1, "one copy of card in discard", 1);
    compareGameState(&gs, &gsCopy); //should be no state change

    //one copy of card in deck - should return 1
    gs = gsOrig;
    gs.handCount[0] = 0;
    gs.discardCount[0] = 0;
    gs.deckCount[0] = 1;
    gs.deck[0][0] = adventurer;
    gsCopy = gs;
    checkValue(fullDeckCount(0, adventurer, &gs), 1, "one copy of card in deck", 1);
    compareGameState(&gs, &gsCopy); //should be no state change

    //one copy in hand+discard+deck - should return 3
    gs = gsOrig;
    gs.handCount[0] = 1;
    gs.discardCount[0] = 1;
    gs.deckCount[0] = 1;
    gs.hand[0][0] = adventurer;
    gs.discard[0][0] = adventurer;
    gs.deck[0][0] = adventurer;
    gsCopy = gs;
    checkValue(fullDeckCount(0, adventurer, &gs), 3, "one copy of card in hand+discard+deck", 1);
    compareGameState(&gs, &gsCopy); //should be no state change

    //MAX_DECK deck/discard + MAX_HAND hand - should return MAX_DECK*2 + MAX_HAND
    gs = gsOrig;
    gs.handCount[0] = MAX_HAND;
    gs.discardCount[0] = MAX_DECK;
    gs.deckCount[0] = MAX_DECK;
    for (i = 0; i < MAX_HAND; i++){
        gs.hand[0][i] = adventurer;
    }
    for (i = 0; i < MAX_DECK; i++){
        gs.discard[0][i] = adventurer;
        gs.deck[0][i] = adventurer;
    }
    gsCopy = gs;
    checkValue(fullDeckCount(0, adventurer, &gs), MAX_DECK*2 + MAX_HAND, "one copy of card in hand+discard+deck", 1);
    compareGameState(&gs, &gsCopy); //should be no state change

    //cards in hand/discard/deck, check for card not present - should return 0
    gs = gsOrig;
    gs.handCount[0] = 0;
    gs.discardCount[0] = 0;
    gs.deckCount[0] = 0;
    for (i = curse; i <= treasure_map; i++){
        if (i != adventurer){
            gs.hand[0][gs.handCount[0]++] = i;
            gs.discard[0][gs.discardCount[0]++] = i;
            gs.deck[0][gs.deckCount[0]++] = i;
        }
    }
    gsCopy = gs;
    checkValue(fullDeckCount(0, adventurer, &gs), 0, "one every card in hand+discard+deck except check card", 1);
    compareGameState(&gs, &gsCopy); //should be no state change

    //cards in hand/discard/deck, one copy of card to check for in each - should return 3
    gs.hand[0][gs.handCount[0]++] = adventurer;
    gs.discard[0][gs.discardCount[0]++] = adventurer;
    gs.deck[0][gs.deckCount[0]++] = adventurer;
    gsCopy = gs;
    checkValue(fullDeckCount(0, adventurer, &gs), 3, "one every card in hand+discard+deck except check card", 1);
    compareGameState(&gs, &gsCopy); //should be no state change

    return 0;
}