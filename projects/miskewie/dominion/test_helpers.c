#include "dominion.h"
#include "test_helpers.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* checkValue 
Simple function to compare two integers and output a message.
Takes two integers and a message string. The string will be output
with either SUCCESS or FAILURE if the two provided integers are
the same or different.

Will return 1 if the same, 0 if different.
*/
int checkValue(int a, int b, char* testMsg, int printSuccess){
    if (a == b){
        if (printSuccess) printf("Validating %s - SUCCESS\n", testMsg);
        return 1;
    }
    else {
        printf("Validating %s - FAILURE - Actual: %i Expected: %i\n", testMsg, a, b);
        return 0;
    }
}


/* debugPrintPlayerHand 
Prints card indexes for the specified players hand.
*/
void debugPrintPlayerHand(struct gameState* gs, int player){
    int i;
    for(i = 0; i < gs->handCount[player]; i++){
        printf("%i: %i\n", i, gs->hand[player][i]);
    }
}

/* compareGameStatePlayerHand 
Compares every card in the specified player's hand between the two provided
gameStates. Returns 0 if there are any differences, 1 if there are no differences.
*/
int compareGameStatePlayerHand(struct gameState* a, struct gameState* b, int player){
    int i;
    int allCardsA[treasure_map + 1] = { 0 }; //store card counts for gameState a
    int allCardsB[treasure_map + 1] = { 0 }; //store card counts for gameState b

    //if hand sizes different return 0
    if (a->handCount[player] != b->handCount[player]){
        return 0;
    }

    //get card counts for a and b gameState
    for (i = 0; i < a->handCount[player]; i++){
        allCardsA[a->hand[player][i]]++;
    }
    for (i = 0; i < b->handCount[player]; i++){
        allCardsB[b->hand[player][i]]++;
    }

    //compare card counts
    for (i=0;i<=treasure_map;i++){
        if(allCardsA[i] != allCardsB[i]){
            printf("FAILURE - card count in hand different for %i | Actual: %i, Expected: %i\n", i, allCardsA[i], allCardsB[i]);
            //printf("Actual Hand:\n");
            //debugPrintPlayerHand(a, player);
            //printf("Expected Hand:\n");
            //debugPrintPlayerHand(b, player);
            return 0;
        }
    }
    return 1;
}


/* compareGameState 
Compares every property of two provided gameState pointers.
If any individual property has a difference, the function will print
a failure message for each and every property that has a difference.

Will return 1 if there were no failures, 0 if there were any failures.
*/
int compareGameState(struct gameState* a, struct gameState* b){
    
    //iterators
    int i;
    int j;
    int failure = 0; //track if there were any failures

    //verify player hands
    for(i=0;i<MAX_PLAYERS;i++){
        if (!compareGameStatePlayerHand(a, b, i)){
            printf("FAILURE: Player %i hand changed.\n", i);
            failure = 1;
        }
    }
    
    //verify decks
    for(i=0;i<MAX_PLAYERS;i++){
        if (a->deckCount[i] != b->deckCount[i]){
            printf("FAILURE: Player %i deck changed.\n", i);
            failure = 1;
        }else{
            for(j=0;j<a->deckCount[i];j++){
                if (a->deck[i][j] != b->deck[i][j]){
                    printf("FAILURE: Player %i deck changed.\n", i);
                    failure = 1;
                    break;
                }
            }
        }
    }

    //verify decks
    for(i=0;i<MAX_PLAYERS;i++){
        if (a->discardCount[i] != b->discardCount[i]){
            printf("FAILURE: Player %i discard changed.\n", i);
            failure = 1;
        }else{
            for(j=0;j<a->discardCount[i];j++){
                if (a->discard[i][j] != b->discard[i][j]){
                    printf("FAILURE: Player %i discard changed.\n", i);
                    failure = 1;
                    break;
                }
            }
        }
    }

    //verify Supply Count
    for(i=0;i<treasure_map+1;i++){
        if (a->supplyCount[i] != b->supplyCount[i]){
            printf("FAILURE: Supply Count %i changed.\n", i);
            failure = 1;
        }
    }

    //verify embargo Tokens
    for(i=0;i<treasure_map+1;i++){
        if (a->embargoTokens[i] != b->embargoTokens[i]){
            printf("FAILURE: Embargo Tokens %i changed.\n", i);
            failure = 1;
        }
    }   

    //verify played cards
    if (a->playedCardCount != b->playedCardCount){
        printf("FAILURE: playedCardCount changed.\n");
        failure = 1;
    } else {
        for(i=0;i<a->playedCardCount;i++){
            if (a->playedCards[i] != b->playedCards[i]){
                printf("FAILURE: Played Cards %i changed.\n", i);
                failure = 1;
            }    
        }
    }

    //Other verifications
    if (a->numPlayers != b->numPlayers){
        printf("FAILURE: numPlayers changed.\n");
        failure = 1;
    }
    if (a->outpostPlayed != b->outpostPlayed){
        printf("FAILURE: outpostPlayed changed.\n");
        failure = 1;
    }
    if (a->outpostTurn != b->outpostTurn){
        printf("FAILURE: outpostTurn changed.\n");
        failure = 1;
    }
    if (a->whoseTurn != b->whoseTurn){
        printf("FAILURE: whoseTurn changed.\n");
        failure = 1;
    }
    if (a->phase != b->phase){
        printf("FAILURE: phase changed.\n");
        failure = 1;
    }
    if (a->numActions != b->numActions){
        printf("FAILURE: numActions changed.\n");
        failure = 1;
    }
    if (a->coins != b->coins){
        printf("FAILURE: coins changed.\n");
        failure = 1;
    }
    if (a->numBuys != b->numBuys){
        printf("FAILURE: numBuys changed.\n");
        failure = 1;
    }

    return !failure;
}