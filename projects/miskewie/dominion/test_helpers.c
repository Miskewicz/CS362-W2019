#include "dominion.h"
#include "test_helpers.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* checkValue 
Simple function to compare two integers and output a message.
Takes two integers and a message string. The string will be output
with either SUCCESS or FAILURE if the the two provided integers are
the same or different.

Will return 1 if the same, 0 if different.
*/
int checkValue(int a, int b, char* testMsg){
    printf("Validating %s\n", testMsg);
    if (a == b){
        printf("SUCCESS\n");
        return 1;
    }
    else {
        printf("FAILURE\n");
        return 0;
    }
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
        for(j=0;j<MAX_HAND;j++){
            if (a->hand[i][j] !== b->hand[i][j]){
                printf("FAILURE: Player %i hand changed.", i);
                failure = 1;
                break;
            }
        }
        //verify hand count
        if(a->handCount[i] !== b->handCount[i]){
            printf("FAILURE: Player %i hand count changed.", i);
            failure = 1;
            break;
        }
    }
    
    //verify decks
    for(i=0;i<MAX_PLAYERS;i++){
        for(j=0;j<MAX_DECK;j++){
            if (a->deck[i][j] !== b->deck[i][j]){
                printf("FAILURE: Player %i deck changed.", i);
                failure = 1;
                break;
            }
        }
        //verify deck count
        if(a->deckCount[i] !== b->deckCount[i]){
            printf("FAILURE: Player %i deck count changed.", i);
            failure = 1;
            break;
        }
    }

    //verify discard pile
    for(i=0;i<MAX_PLAYERS;i++){
        for(j=0;j<MAX_DECK;j++){
            if (a->discard[i][j] !== b->discard[i][j]){
                printf("FAILURE: Player %i discard changed.", i);
                failure = 1;
                break;
            }
        }
        //verify discard count
        if(a->discardCount[i] !== b->discardCount[i]){
            printf("FAILURE: Player %i discard count changed.", i);
            failure = 1;
            break;
        }
    }

    //verify Supply Count
    for(i=0;i<treasure_map+1;i++){
        if (a->supplyCount[i] !== b->supplyCount[i]){
            printf("FAILURE: Supply Count %i changed.", i);
            failure = 1;
        }
    }

    //verify embargo Tokens
    for(i=0;i<treasure_map+1;i++){
        if (a->embargoTokens[i] !== b->embargoTokens[i]){
            printf("FAILURE: Embargo Tokens %i changed.", i);
            failure = 1;
        }
    }   

    //verify played cards
    for(i=0;i<MAX_DECK;i++){
        if (a->playedCards[i] !== b->playedCards[i]){
            printf("FAILURE: Played Cards %i changed.", i);
            failure = 1;
        }    
    }
    //played cards count
    if (a->playedCardCount !== b->playedCardCount){
        printf("FAILURE: playedCardCount changed.");
        failure = 1;
    }

    //Other verifications
    if (a->numPlayers !== b->numPlayers){
        printf("FAILURE: numPlayers changed.");
        failure = 1;
    }
    if (a->outpostPlayed !== b->outpostPlayed){
        printf("FAILURE: outpostPlayed changed.");
        failure = 1;
    }
    if (a->outpostTurn !== b->outpostTurn){
        printf("FAILURE: outpostTurn changed.");
        failure = 1;
    }
    if (a->whoseTurn !== b->whoseTurn){
        printf("FAILURE: whoseTurn changed.");
        failure = 1;
    }
    if (a->phase !== b->phase){
        printf("FAILURE: phase changed.");
        failure = 1;
    }
    if (a->numActions !== b->numActions){
        printf("FAILURE: numActions changed.");
        failure = 1;
    }
    if (a->coins !== b->coins){
        printf("FAILURE: coins changed.");
        failure = 1;
    }
    if (a->numBuys !== b->numBuys){
        printf("FAILURE: numBuys changed.");
        failure = 1;
    }

    return !failure;
}