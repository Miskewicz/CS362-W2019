#define _XOPEN_SOURCE //suggestion from https://stackoverflow.com/questions/6491019/struct-sigaction-incomplete-error

#include "dominion.h"
#include "dominion_helpers.h"

#include "test_helpers.h"

#include <limits.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void sig_segv_handler(int sig_num){
    write(1, "SIGSEGV received running program\n", 33);
    //exit function suggestion from:
    //https://stackoverflow.com/questions/37829429/sigsegv-handler-can-not-exit
    _exit(0);
}


//smithy test
int main(){

    int bonus;
    int i;

    //initialize gameState, 2 player game
    int kCards[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState gs;
    struct gameState gsOrig;
    struct gameState gsCopy;

    // register handler for SIGSEGV
    //code suggestions from piazza post
    struct sigaction *sa;
    sa = malloc(sizeof(struct sigaction));
    sa->sa_handler = sig_segv_handler;
    sigaction(SIGSEGV, sa, NULL);

    //init game
    initializeGame(2, kCards, 1, &gs);
    
    //add smithy to player 1 hand
    gs.whoseTurn = 0;
    gs.hand[0][gs.handCount[0]++] = smithy;
    gsOrig = gs;

    //play smithy with more than three cards in deck
    printf("\nTest 1: Normal smithy play - deck has more than 3 cards\n");
    printf("========================================\n");
    //ensure deck has more than three cards
    while (gs.deckCount[0] < 3){
        gs.deck[0][gs.deckCount[0]++] = copper;
    }
    gsCopy = gs;
    //printf("Starting hand:\n"); //debug printing
    //debugPrintPlayerHand(&gs, 0);
    checkValue(cardEffect(smithy, 0, 0, 0, &gs, gs.handCount[0]-1, &bonus), 0, "function returned success", 0);

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
        gsCopy.hand[0][gsCopy.handCount[0]++] = gsCopy.deck[0][gsCopy.deckCount[0]-1]; //add card from deck into hand
        gsCopy.deckCount[0]--;
    }

    //check cards in hand (that they match the top three cards of the draw pile)
    checkValue(compareGameStatePlayerHand(&gs, &gsCopy, 0), 1, "correct cards drawn", 1);

    //hands checked already - so zero out hands and check all else in game state hasn't changed
    gsCopy.handCount[0] = 0;
    gs.handCount[0] = 0;
    compareGameState(&gs, &gsCopy);

    printf("\nTest 2: No cards in deck - three cards in discard\n");
    printf("========================================\n");
    //empty deck and add three copper to discard
    gs = gsOrig;
    gs.deckCount[0] = 0;
    gs.discardCount[0] = 3;
    for (i = 0; i < 3; i ++){
        gs.discard[0][i] = copper;
    }
    gsCopy = gs;
    checkValue(cardEffect(smithy, 0, 0, 0, &gs, gs.handCount[0]-1, &bonus), 0, "function returned success", 0);

    //check handCount up 2 (+3 cards - 1 Smithy)
    checkValue(gs.handCount[0], gsCopy.handCount[0] + 2, "hand count up 2", 1);

    //cards drawn from discard being reshuffled
    checkValue(gs.discardCount[0], 0, "discard now empty", 1);

    //three cards drawn are copper
    checkValue(gs.hand[0][gs.handCount[0] - 1], copper, "first drawn card is copper", 1);
    checkValue(gs.hand[0][gs.handCount[0] - 2], copper, "second drawn card is copper", 1);
    checkValue(gs.hand[0][gs.handCount[0] - 3], copper, "third drawn card is copper", 1);

    return 0;
}