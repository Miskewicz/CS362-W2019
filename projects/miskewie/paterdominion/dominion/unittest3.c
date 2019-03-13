#include "dominion.h"
#include "dominion_helpers.h"

#include "test_helpers.h"

#include <limits.h>
#include <stdio.h>

//updateCoins test
int main(){
    int i;
    int j;
    int player;
    int coinValue;
    char buffer [128];

    //initialize gameState, 2 player game
    int kCards[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState gs;
    struct gameState gsOrig;
    struct gameState gsCopy;
    initializeGame(2, kCards, 1, &gs);
    gsOrig = gs;

    printf("\nTest 1: Coins in deck/discard - none in hand\n");
    printf("========================================\n");
    //coins in deck+discard, none in hand - should be 0 coins
    gs.deckCount[0] = 2;
    gs.deck[0][0] = copper;
    gs.deck[0][1] = silver;
    gs.handCount[0] = 2;
    gs.hand[0][0] = smithy;
    gs.hand[0][1] = smithy;
    gs.discardCount[0] = 1;
    gs.discard[0][0] = gold;
    gsCopy = gs;
    checkValue(updateCoins(0, &gs, 0), 0, "function returns success", 0);
    checkValue(gs.coins, 0, "coins in deck+discard, none in hand", 1);
    gsCopy.coins = gs.coins;
    compareGameState(&gs, &gsCopy); //should be no state change

    printf("\nTest 2: Non-coin cards in hand\n");
    printf("========================================\n");
    //one of every non coin card in hand - should be 0 coins
    gs = gsOrig;
    gs.handCount[0] = 0;
    for (i = curse; i <= treasure_map; i++){
        if (i != copper && i != silver && i != gold){
            gs.hand[0][gs.handCount[0]] = i;
            gs.handCount[0]++;
        }
    }
    gsCopy = gs;
    checkValue(updateCoins(0, &gs, 0), 0, "function returns success", 0);
    checkValue(gs.coins, 0, "every non coin card in hand", 1);
    gsCopy.coins = gs.coins;
    compareGameState(&gs, &gsCopy); //should be no state change

    printf("\nTest 3: Empty hand\n");
    printf("========================================\n");
    //completely empty hand - should be 0 coins
    gs = gsOrig;
    gs.handCount[0] = 0;
    gsCopy = gs;
    checkValue(updateCoins(0, &gs, 0), 0, "function returns success", 0);
    checkValue(gs.coins, 0, "empty hand", 1);
    gsCopy.coins = gs.coins;
    compareGameState(&gs, &gsCopy); //should be no state change

    printf("\nTest 4: Coins in hand\n");
    printf("========================================\n");
    //1 coin in hand of each type - should be value of the respective coin
    coinValue = 1;
    for(i = copper; i <=gold; i++){
        gs = gsOrig;
        gs.hand[0][0] = i;
        gs.handCount[0] = 1;
        gsCopy = gs;
        checkValue(updateCoins(0, &gs, 0), 0, "function returns success", 0);
        sprintf(buffer, "single coin in hand id: %i", i);
        checkValue(gs.coins, coinValue++, buffer, 1);
        gsCopy.coins = gs.coins;
        compareGameState(&gs, &gsCopy); //should be no state change
    }

    printf("\nTest 5: Coins in hand - each player\n");
    printf("========================================\n");
    //1 of each coin in hand - each player
    for(player = 0; player < MAX_PLAYERS; player++){
        gs = gsOrig;
        gs.handCount[player] = 3;
        j = 0;
        for(i = copper; i <=gold; i++){
            gs.hand[player][j++] = i;
        }
        gsCopy = gs;
        checkValue(updateCoins(player, &gs, 0), 0, "function returns success", 0);
        sprintf(buffer, "1 of each coin player %i", player);
        checkValue(gs.coins, 6, buffer, 1);
        gsCopy.coins = gs.coins;
        compareGameState(&gs, &gsCopy); //should be no state change
    }

    printf("\nTest 6: MAX_HAND full of copper\n");
    printf("========================================\n");
    //MAX_HAND full of copper
    gs = gsOrig;
    gs.handCount[0] = MAX_HAND;
    for(i = 0; i < MAX_HAND; i++){
        gs.hand[0][i] = copper;
    }
    gsCopy = gs;
    checkValue(updateCoins(0, &gs, 0), 0, "function returns success", 0);
    checkValue(gs.coins, MAX_HAND, "hand full of copper", 1);
    gsCopy.coins = gs.coins;
    compareGameState(&gs, &gsCopy); //should be no state change

    return 0;
}