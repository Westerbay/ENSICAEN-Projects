/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * This file is owned by ENSICAEN students. No portion of this
 * document may be reproduced, copied or revised without written
 * permission of the authors.
 */

/**
 * @file test_util.c
 * 
 * @brief Tests of util functions.
 * 
 * @author Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <util.h>
#include <math.h>
#include <assert.h>

#define N 1000000


static void testIsPositionReachable() {
	printf("Testing isPositionReachable ...\n");
	assert(isPositionReachable(0, 1));
	assert(isPositionReachable(0, 7));
	assert(!isPositionReachable(0, 6));
	assert(!isPositionReachable(0, -1));
}


static void testRandomDoubleBetween() {
	printf("Testing randomDoubleBetween ...\n");
	double mean = 0;
	for (int i = 0; i < N; i ++) {
		mean += randomDoubleBetween(0, 2) / N;
	}
	double trustFivePercentErrorA = 1 - 1.13/sqrt(N);
	double trustFivePercentErrorB = 1 + 1.13/sqrt(N);
	assert(mean >= trustFivePercentErrorA && mean <= trustFivePercentErrorB);
}


static void testIsTrueWithProbabilityOf() {
	printf("Testing isTrueWithProbabilityOf ...\n");
	unsigned nbTrue = 0;
	for (int i = 0; i < N; i ++) {
		nbTrue += isTrueWithProbabilityOf(0.7);
	}
	double mean = N * 0.7;
	double deviation = sqrt(mean * 0.3);
	double trustFivePercentErrorA = mean - 1.96 * deviation;
	double trustFivePercentErrorB = mean + 1.96 * deviation;
	assert(nbTrue >= trustFivePercentErrorA && nbTrue <= trustFivePercentErrorB);
}


static void testRandomIntBetween() {
	printf("Testing randomIntBetween ...\n");
	int tab[3] = {0, 0, 0};
	double mean = 0;
	for (int i = 0; i < N; i ++) {
		int choice = randomIntBetween(0, 3);
		mean += (double) choice / N;
		tab[choice] ++;
	}
	double trustFivePercentErrorA = 1 - 1.13/sqrt(N);
	double trustFivePercentErrorB = 1 + 1.13/sqrt(N);
	assert(mean >= trustFivePercentErrorA && mean <= trustFivePercentErrorB);
	assert(tab[0] && tab[1] && tab[2]);
}



static void testRoundInt() {
	printf("Testing roundInt ...\n");
	assert(roundInt(3.4) == 3);
	assert(roundInt(3.0) == 3);
	assert(roundInt(3.6) == 4);
}

void launchTest() {
	printf("Testing Util Functions ...\n");
	testIsPositionReachable();
	testRandomDoubleBetween();
	testIsTrueWithProbabilityOf();
	testRandomIntBetween();
	testRoundInt();
	printf("Util Functions tests ended ...\n");
}