CFLAGS= -Wall -fpic -coverage -lm -std=c99

defualt: testme

testme: testme.c
	gcc -o testme testme.c ${CFLAGS}
	./testme

all: testme

clean:
	rm *.gcda *.gcno ./testme -f