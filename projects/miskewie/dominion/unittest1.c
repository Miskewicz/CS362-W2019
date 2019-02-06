#include "dominion.h"
#include "dominion_helpers.h"

#include "test_helpers.h"

//Unit test for the isGameOver function
int main(){
    int i; //iterator

    //initialize gameState, 2 player game
    int kCards[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall,};
    struct gameState gs;
    struct gameState gsOrig;
    struct gameState gsCopy;
    initializeGame(2, kCards, 1, &gs);
    gsOrig = gs;

    //beginning of game - isGameOver should return false
    gsCopy = gs;
    checkValue(isGameOver(&gs), 0, "Game Beginning");
    compareGameState(&gs, &gsCopy); //should be no state change

    //set no province left - isGameOver should return True
    gs.supplyCount[province] = 0;    
    gsCopy = gs;
    checkValue(isGameOver(&gs), 1, "No Estates");
    compareGameState(&gs, &gsCopy); //should be no state change

    //set 1 empty kingdom card pile - isGameOver should return false
    gs = gsOrig; //reset gameState
    gs.supplyCount[adventurer] = 0;
    gsCopy = gs;
    checkValue(isGameOver(&gs), 0, "1 Empty Kingdom Pile");
    compareGameState(&gs, &gsCopy); //should be no state change

    //set 2 empty kingdom card pile - isGameOver should return false
    gs.supplyCount[council_room] = 0;
    gsCopy = gs;
    checkValue(isGameOver(&gs), 0, "2 Empty Kingdom Pile");
    compareGameState(&gs, &gsCopy); //should be no state change

    //set 3 empty kingdom card pile - isGameOver should return true
    gs.supplyCount[feast] = 0;
    gsCopy = gs;
    checkValue(isGameOver(&gs), 1, "3 Empty Kingdom Pile");
    compareGameState(&gs, &gsCopy); //should be no state change

    //1 empty non kingdom supply pile - isGameOver should return false
    gs = gsOrig; //reset gameState
    gs.supplyCount[curse] = 0;
    gsCopy = gs;
    checkValue(isGameOver(&gs), 0, "Empty Curse Pile");
    compareGameState(&gs, &gsCopy); //should be no state change

    //2 empty non kingdom supply pile - isGameOver should return false
    gs.supplyCount[copper] = 0;
    gsCopy = gs;
    checkValue(isGameOver(&gs), 0, "+Empty Copper Pile");
    compareGameState(&gs, &gsCopy); //should be no state change

    //3 empty non kingdom supply pile - isGameOver should return true
    gs.supplyCount[duchy] = 0;
    gsCopy = gs;
    checkValue(isGameOver(&gs), 1, "+Empty Duchy Pile");
    compareGameState(&gs, &gsCopy); //should be no state change

    //4 empty supply piles - isGameOver should return true
    gs.supplyCount[silver] = 0;
    gsCopy = gs;
    checkValue(isGameOver(&gs), 1, "+Empty silver Pile");
    compareGameState(&gs, &gsCopy); //should be no state change

    //all empty supply piles - isGameOver should return true
    for (i=curse; i<=treasure_map; i++){
        gs.supplyCount[i]=0;
    }
    gsCopy = gs;
    checkValue(isGameOver(&gs), 1, "All empty piles");
    compareGameState(&gs, &gsCopy); //should be no state change

    return 0;
}
