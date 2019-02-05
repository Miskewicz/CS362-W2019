#include "dominion.h"
#include "dominion_helpers.h"

#include "test_helpers.h"

//test the isGameOver function
int main(){

    //initialize gameState, 2 player game
    int kCards[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall,};
    struct gameState gs;
    struct gameState gsCopy;
    initializeGame(2, kCards, 0, &gs);
    gsCopy = gs;

    //beginning of game - isGameOver should return false
    checkValue(isGameOver(&gs), 0, "Game Beginning");
    compareGameState(&gs, &gsCopy); //should be no state change


    //set no province left - isGameOver should return True
    gs.supplyCount[province] = 0;
    checkValue(isGameOver(&gs), 1, "No Estates");


    //The Province Supply pile is empty (or the Colony Supply pile is empty, in a game with the Colony card from the Prosperity expansion)
    //Any three Supply piles are empty. (Any four piles when playing with five or six players.) This includes all the Supply piles, not just the 10 Kingdom card piles that are selected for each game. So, for instance, if the Estate pile, the Curse pile, and one of the Kingdom card piles is empty, the game ends.


    return 0;
}
