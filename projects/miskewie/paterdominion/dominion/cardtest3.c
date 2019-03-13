#include "dominion.h"
#include "dominion_helpers.h"

#include "test_helpers.h"

#include <limits.h>
#include <stdio.h>

//test village
int main(){
    int bonus;

    //initialize gameState, 2 player game
    int kCards[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState gs;
    struct gameState gsOrig;
    struct gameState gsCopy;
    initializeGame(2, kCards, 1, &gs);

    //add 1 village to hand
    gs.whoseTurn = 0;
    gs.hand[0][gs.handCount[0]++] = village;
    gsOrig = gs; //save copy

    printf("\nTest 1: province on top of deck - 0 actions\n");
    printf("========================================\n");
    gs.numActions = 0;
    //add province to top of deck
    gs.deck[0][gs.deckCount[0]++] = province;
    gsCopy = gs;
    checkValue(cardEffect(village, 0, 0, 0, &gs, gs.handCount[0] - 1, &bonus), 0, "function returned success", 0);

    //hand count unchanged (+ 1 card drawn, -1 card played)
    checkValue(gs.handCount[0], gsCopy.handCount[0], "hand count unchanged", 1);

    //deck count - 1
    checkValue(gs.deckCount[0], gsCopy.deckCount[0] - 1, "deck count - 1", 1);

    //actions = 2
    checkValue(gs.numActions, 2, "# of actions is now 2", 1);

    //drawn card is province
    checkValue(gs.hand[0][gs.handCount[0] - 1], province, "card drawn is province", 1);

    //played card is village
    checkValue(gs.playedCardCount, 1, "one played card", 1);
    checkValue(gs.playedCards[0], village, "played card is village", 1);

    //mirror activity in copy
    gsCopy.hand[0][gsCopy.handCount[0] - 1] = province; //replace village with province
    gsCopy.deckCount[0]--;
    gsCopy.numActions += 2;
    gsCopy.playedCardCount = 1;
    gsCopy.playedCards[0] = village;
    compareGameState(&gs, &gsCopy); //make sure no other state changes

    gs = gsOrig; //save copy
    printf("\nTest 2: 999 actions\n");
    printf("========================================\n");
    gs.numActions = 999;
    checkValue(cardEffect(village, 0, 0, 0, &gs, gs.handCount[0] - 1, &bonus), 0, "function returned success", 0);
    checkValue(gs.numActions, 1001, "num actions = 1001", 1);

    gs = gsOrig; //save copy
    printf("\nTest 3: empty deck, province in discard\n");
    printf("========================================\n");
    gs.deckCount[0] = 0;
    gs.discardCount[0] = 1;
    gs.discard[0][0] = province;

    gsCopy = gs;
    checkValue(cardEffect(village, 0, 0, 0, &gs, gs.handCount[0] - 1, &bonus), 0, "function returned success", 0);

    //deck count is zero
    checkValue(gs.deckCount[0], 0, "zero deck count", 1);

    //discard count is zero
    checkValue(gs.discardCount[0], 0, "zero discard count", 1);

    //drawn card is province
    checkValue(gs.hand[0][gs.handCount[0] - 1], province, "card drawn is province", 1);

    //hand count unchanged (+ 1 card drawn, -1 card played)
    checkValue(gs.handCount[0], gsCopy.handCount[0], "hand count unchanged", 1);

    return 0;
}