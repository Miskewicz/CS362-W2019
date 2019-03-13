#include "dominion.h"
#include "dominion_helpers.h"

#include "test_helpers.h"

#include <limits.h>
#include <stdio.h>

//Function that checks the mine results given the provided card to trash and
// card to gain. Should be used only with parameters that would lead to an INCORRECT
// use of the Mine card.
void invalidMineTest(struct gameState* gs, int cardToTrash, int cardToGain, int player){
    struct gameState gsCopy;
    int result;
    int bonus;

    //add card to hand
    gs->hand[player][gs->handCount[player]++] = cardToTrash;
    gs->whoseTurn = player;
    gsCopy = *gs;
    result = cardEffect(mine, gs->handCount[player] - 1, cardToGain, 0, gs, gs->handCount[player] - 2, &bonus);

    //function failure
    checkValue(result, -1, "function should error", 1);

    //if function correctly failed - check and make sure no other state change
    if (result == -1) compareGameState(gs, &gsCopy);  
}

//Function that checks the mine results given the provided card to trash and
// card to gain. Should be used only with parameters that would yield a SUCCESSFUL
// use of the Mine card.
void validMineTest(struct gameState* gs, int cardToTrash, int cardToGain, int player){
    struct gameState gsCopy;
    int result;
    int bonus;

    printf("Running valid mine test for player %i\n", player);

    //add card to hand
    gs->hand[player][gs->handCount[player]++] = cardToTrash;
    gs->whoseTurn = player;
    gsCopy = *gs;
    result = cardEffect(mine, gs->handCount[player] - 1, cardToGain, 0, gs, gs->handCount[player] - 2, &bonus);

    //function success
    checkValue(result, 0, "no error in function", 1);

    //card was trashed - only one played card is mine
    checkValue(gs->playedCardCount, 1, "only one card played", 1);
    checkValue(gs->playedCards[0], mine, "played card is mine", 1);

    //card gained
    checkValue(gs->handCount[player], gsCopy.handCount[player] - 1, "hand count minus 1", 1);
    checkValue(gs->hand[player][gs->handCount[player] - 1], cardToGain, "gained card is correct", 1);
    checkValue(gs->supplyCount[cardToGain], gsCopy.supplyCount[cardToGain] - 1, "1 less gained card in supply", 1);

    //mirror activity in gsCopy
    gsCopy.hand[player][gsCopy.handCount[0] - 2] = cardToGain;
    gsCopy.handCount[player]--;
    gsCopy.playedCardCount = 1;
    gsCopy.playedCards[0] = mine;
    gsCopy.supplyCount[cardToGain]--;
    compareGameState(gs, &gsCopy); //should be no other state change   
}


//test mine
int main(){
    int i;

    //initialize gameState, 2 player game
    int kCards[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState gs;
    struct gameState gsOrig;
    initializeGame(2, kCards, 1, &gs);

    //add 1 mine to hand
    gs.whoseTurn = 0;
    gs.hand[0][gs.handCount[0]++] = mine;
    gsOrig = gs; //save copy


    printf("\nTest 1: Trash copper, gain silver - each player\n");
    printf("========================================\n");
    for (i = 0; i < MAX_PLAYERS; i++){
        validMineTest(&gs, copper, silver, i);
    }

    gs = gsOrig;
    printf("\nTest 2: Trash silver, gain gold\n");
    printf("========================================\n");
    validMineTest(&gs, silver, gold, 0);

    gs = gsOrig;
    printf("\nTest 3: Trash gold, gain copper\n");
    printf("========================================\n");
    validMineTest(&gs, gold, copper, 0);

    gs = gsOrig;
    printf("\nTest 4: Trash copper, gain gold\n");
    printf("========================================\n");
    invalidMineTest(&gs, copper, gold, 0);

    gs = gsOrig;
    printf("\nTest 5: Trash non treasure, gain gold\n");
    printf("========================================\n");
    for (i = curse; i <=treasure_map; i++){
        if (i != copper && i != silver && i != gold){
            invalidMineTest(&gs, i, gold, 0);
        }
    }

    gs = gsOrig;
    printf("\nTest 6: Trash copper, gain non treasure\n");
    printf("========================================\n");
    for (i = curse; i <=treasure_map; i++){
        if (i != copper && i != silver && i != gold){
            invalidMineTest(&gs, copper, i, 0);
        }
    }

    gs = gsOrig;
    printf("\nTest 7: Trash copper, gain silver, pile empty\n");
    printf("========================================\n");
    gs.supplyCount[silver] = 0;
    invalidMineTest(&gs, copper, silver, 0);

    return 0;
}