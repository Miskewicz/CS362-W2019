#include "dominion.h"
#include "dominion_helpers.h"

#include "test_helpers.h"

#include <limits.h>
#include <stdio.h>


void invalidMineTest(struct gameState* gs, int cardToTrash, int cardToGain){
    struct gameState gsCopy;
    int result;
    int bonus;

    //add card to hand
    gs->hand[0][gs->handCount[0]++] = cardToTrash;
    gsCopy = *gs;
    result = cardEffect(mine, gs->handCount[0] - 1, cardToGain, 0, gs, gs->handCount[0] - 2, &bonus);

    //function failure
    checkValue(result, -1, "function should error", 1);

    compareGameState(gs, &gsCopy); //should be no other state change     
}


void validMineTest(struct gameState* gs, int cardToTrash, int cardToGain){
    struct gameState gsCopy;
    int result;
    int bonus;
    //add card to hand
    gs->hand[0][gs->handCount[0]++] = cardToTrash;
    gsCopy = *gs;
    result = cardEffect(mine, gs->handCount[0] - 1, cardToGain, 0, gs, gs->handCount[0] - 2, &bonus);

    //function success
    checkValue(result, 0, "no error in function", 1);

    //card was trashed - only one played card is mine
    checkValue(gs->playedCardCount, 1, "only one card played", 1);
    checkValue(gs->playedCards[0], mine, "played card is mine", 1);

    //card gained
    checkValue(gs->handCount[0], gsCopy.handCount[0] - 1, "hand count minus 1", 1);
    checkValue(gs->hand[0][gs->handCount[0] - 1], cardToGain, "gained card is correct", 1);
    checkValue(gs->supplyCount[cardToGain], gsCopy.supplyCount[cardToGain] - 1, "1 less gained card in supply", 1);

    //mirror activity in gsCopy
    gsCopy.hand[0][gsCopy.handCount[0] - 2] = cardToGain;
    gsCopy.handCount[0]--;
    gsCopy.playedCardCount = 1;
    gsCopy.playedCards[0] = mine;
    gsCopy.supplyCount[cardToGain]--;
    compareGameState(gs, &gsCopy); //should be no other state change   
}


//test mine
int main(){
    int i;
    int bonus;
    int result;

    //initialize gameState, 2 player game
    int kCards[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState gs;
    struct gameState gsOrig;
    struct gameState gsCopy;
    initializeGame(2, kCards, 1, &gs);

    //add 1 mine to hand
    gs.whoseTurn = 0;
    gs.hand[0][gs.handCount[0]++] = mine;
    gsOrig = gs; //save copy


    printf("\nTest 1: Trash copper, gain silver - success\n");
    printf("========================================\n");
    validMineTest(&gs, copper, silver);

    gs = gsOrig;
    printf("\nTest 2: Trash silver, gain gold - success\n");
    printf("========================================\n");
    validMineTest(&gs, silver, gold);

    gs = gsOrig;
    printf("\nTest 3: Trash gold, gain copper - success\n");
    printf("========================================\n");
    validMineTest(&gs, gold, copper);

    gs = gsOrig;
    printf("\nTest 4: Trash copper, gain gold - failure\n");
    printf("========================================\n");
    invalidMineTest(&gs, copper, gold);

    gs = gsOrig;
    printf("\nTest 4: Trash non treasure, gain gold - failure\n");
    printf("========================================\n");
    for (i = curse; i <=treasure_map; i++){
        if (i != copper && i != silver && i != gold){
            invalidMineTest(&gs, i, gold);
        }
    }

    gs = gsOrig;
    printf("\nTest 5: Trash copper, gain non treasure - failure\n");
    printf("========================================\n");
    for (i = curse; i <=treasure_map; i++){
        if (i != copper && i != silver && i != gold){
            invalidMineTest(&gs, copper, i);
        }
    }

    gs = gsOrig;
    printf("\nTest 6: Trash copper, gain silver, pile empty - failure\n");
    printf("========================================\n");
    gs.supplyCount[silver] = 0;
    invalidMineTest(&gs, copper, silver);

    return 0;
}