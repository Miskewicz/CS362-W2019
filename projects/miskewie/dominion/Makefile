CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

test_helpers.o: test_helpers.h test_helpers.c dominion.o
	gcc -c test_helpers.c dominion.o rngs.o

unittestresults: unittest1.c unittest2.c unittest3.c unittest4.c cardtest1.c cardtest2.c cardtest3.c cardtest4.c dominion.o rngs.o test_helpers.o
	gcc -o unittest1 unittest1.c dominion.o rngs.o test_helpers.o $(CFLAGS)
	echo RUNNING UNIT TEST 1 - isGameOver >unittestresults.out
	./unittest1 >>unittestresults.out
	gcc -o unittest2 unittest2.c dominion.o rngs.o test_helpers.o $(CFLAGS)
	echo RUNNING UNIT TEST 2 - scoreFor >>unittestresults.out
	./unittest2 >>unittestresults.out
	gcc -o unittest3 unittest3.c dominion.o rngs.o test_helpers.o $(CFLAGS)
	echo RUNNING UNIT TEST 3 - updateCoins >>unittestresults.out
	./unittest3 >>unittestresults.out
	gcc -o unittest4 unittest4.c dominion.o rngs.o test_helpers.o $(CFLAGS)
	echo RUNNING UNIT TEST 4 - fullDeckCount >>unittestresults.out
	./unittest4 >>unittestresults.out
	gcc -o cardtest1 cardtest1.c dominion.o rngs.o test_helpers.o $(CFLAGS)
	echo RUNNING CARD TEST 1 - smithy >>unittestresults.out
	./cardtest1 >>unittestresults.out
	gcc -o cardtest2 cardtest2.c dominion.o rngs.o test_helpers.o $(CFLAGS)
	echo RUNNING CARD TEST 2 - adventurer >>unittestresults.out
	./cardtest2 >>unittestresults.out
	gcc -o cardtest3 cardtest3.c dominion.o rngs.o test_helpers.o $(CFLAGS)
	echo RUNNING CARD TEST 3 - village >>unittestresults.out
	./cardtest3 >>unittestresults.out
	gcc -o cardtest4 cardtest4.c dominion.o rngs.o test_helpers.o $(CFLAGS)
	echo RUNNING CARD TEST 4 - mine >>unittestresults.out
	./cardtest4 >>unittestresults.out
	echo GATHERING CODE COVERAGE >>unittestresults.out
	gcov dominion.c
	cat dominion.c.gcov >>unittestresults.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
