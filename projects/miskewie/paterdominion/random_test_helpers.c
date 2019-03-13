#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//get a random integer between (inclusive) the specified min and max
int randBetween(int min, int max){
    return(rand() % (max - min + 1) + min);
}

//get a random card from all possible cards
int getRandomCard(){
    return(randBetween(curse, treasure_map));
}

//set a random player to the whoseTurn attribute of the gameState, and return the
//player selected.
int randomActivePlayer(struct gameState* gs){
    int activePlayer;

    activePlayer = randBetween(0, gs->numPlayers - 1);
    gs->whoseTurn = activePlayer;

    return activePlayer;
}

//Adds a specific card to a specific player's hand in a random position.
//if the player's hand is empty, will add the card. If the player's hand
//is not empty, will replace a random card in the hand.
int addCardToHandRandom(struct gameState* gs, int player, int card){
    int handPos;

    //if there are NO cards in hand, add it to the player's hand as the only card
    if (gs->handCount[player] == 0){
        gs->handCount[player] = 1;
        gs->hand[player][0] = card;
        return 0;
    }
    //otherwise replace a random card in the hand with the desired card
    else {
        handPos = randBetween(0, gs->handCount[player] - 1);
        gs->hand[player][handPos] = card;
        return handPos;
    }
}

//generate random supply numbers for ALL card types.
//using a random number between 0 and MAX_DECK.
void randomizeSupply(struct gameState* gs){
    int i;
    for(i = curse; i <= treasure_map; i++){
        if(i >= copper && i <= gold){
            gs->supplyCount[i] = randBetween(0, 60);
        }
        else if(i == curse){
            gs->supplyCount[i] = randBetween(0, 30);
        }
        else if(i >= estate && i <= province){
            gs->supplyCount[i] = randBetween(0, 24);
        }
        else {
            gs->supplyCount[i] = randBetween(0, 10);
        }
    }
}

//generate a completely random played cards pile.
//size will be a random number between 0 and MAX_HAND.
void randomizePlayedCards(struct gameState* gs){
    int i;

    //played card count
    gs->playedCardCount = randBetween(0, MAX_DECK);

    //populate played cards
    for(i = 0; i < gs->playedCardCount; i++){
        gs->playedCards[i] = getRandomCard();
    }
}

//generate a completely random discard pile for a specific player.
//will use a random discard size between 0 and MAX_DECK
void randomizeDiscard(struct gameState* gs, int player){
    int i;

    //discard size
    gs->discardCount[player] = randBetween(0, MAX_DECK);

    //populate discard pile
    for(i = 0; i < gs->discardCount[player]; i++){
        gs->discard[player][i] = getRandomCard();
    }
}

//generate a completely random hand for a specific player.
//will use a random deck size between 0 and MAX_HAND
void randomizeHand(struct gameState* gs, int player){
    int i;

    //hand size
    gs->handCount[player] = randBetween(0, MAX_HAND);

    //populate hand
    for(i = 0; i < gs->handCount[player]; i++){
        gs->hand[player][i] = getRandomCard();
    }
}

//generate a completely random deck for a specific player.
//will use a random deck size between 0 and MAX_DECK
void randomizeDeck(struct gameState* gs, int player){
    int i;
    
    //deck size
    gs->deckCount[player] = randBetween(0, MAX_DECK);

    //populate deck
    for(i = 0; i < gs->deckCount[player]; i++){
        gs->deck[player][i] = getRandomCard();
    }
}


//generate a completely random gameState that contains
//values for all attributes within reason.
void randomizeGameState(struct gameState* gs){

    int i;

    //zero gameState
    memset(gs, 0, sizeof(struct gameState));

    //random players
    gs->numPlayers = randBetween(2, MAX_PLAYERS);

    //for each player
    for(i = 0; i < gs->numPlayers; i++){
        //random deck
        randomizeDeck(gs, i);

        //random hand
        randomizeHand(gs, i);

        //random discard
        randomizeDiscard(gs, i);
    }

    //played cards
    randomizePlayedCards(gs);

    //supply
    randomizeSupply(gs);

    //randomize other gameState values
    gs->numActions = randBetween(0, 25);
    gs->numBuys = randBetween(0, 25);
    gs->coins = randBetween(0, 100);
}