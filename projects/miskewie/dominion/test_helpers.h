#ifndef _TEST_HELPERS_H
#define _TEST_HELPERS_H

#include "dominion.h"

/* checkValue 
Simple function to compare two integers and output a message.
Takes two integers and a message string. The string will be output
with either SUCCESS or FAILURE if the the two provided integers are
the same or different.

Will return 1 if the same, 0 if different.
*/
int checkValue(int a, int b, char* testMsg, int printSuccess);

/* debugPrintPlayerHand 
Prints card indexes for the specified players hand.
*/
void debugPrintPlayerHand(struct gameState* gs, int player);

/* compareGameStatePlayerHand 
Compares every card in the specified player's hand between the two provided
gameStates. Returns 0 if there are any differences, 1 if there are no differences.
*/
int compareGameStatePlayerHand(struct gameState* a, struct gameState* b, int player);


/* compareGameState 
Compares every property of two provided gameState pointers.
If any individual property has a difference, the function will print
a failure message for each and every property that has a difference.

Will return 1 if there were no failures, 0 if there were any failures.
*/
int compareGameState(struct gameState* a, struct gameState* b);

#endif