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
 * @file util.c
 * 
 * @brief Code of functions used by more than one program.
 * 
 * @author Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>
 * @author Chauvet Lucie <lucie.chauvet@ecole.ensicaen.fr>
 * @author Salic Valentin <valentin.salic@ecole.ensicaen.fr>
 * @author Leconte Audrey <audrey.leconte@ecole.ensicaen.fr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <util.h>


const Direction DIRECTIONS[NUMBER_DIRECTION] = {
    NORTH_WEST, NORTH, NORTH_EAST,
    WEST,       EAST,
    SOUTH_WEST, SOUTH, SOUTH_EAST
};


/**
 * @brief Tells if a position is reachable or not in the city map.
 * @param newPosition The position the citizen is willing to go to.
 * @param currentPosition The current position of the citizen.
 * @return If the citizen can reach the new position or not.
 */
boolean isPositionReachable(unsigned newPosition, unsigned currentPosition) {

	if (!positionExists(newPosition)) return false;
    
	unsigned currentRow = currentPosition / GRID_LENGTH;
	unsigned currentColumn = currentPosition % GRID_LENGTH;
	unsigned newRow = newPosition / GRID_LENGTH;
	unsigned newColumn = newPosition % GRID_LENGTH;

	return unsignedAbsoluteDifference(currentRow, newRow) <= 1 && 
		unsignedAbsoluteDifference(currentColumn, newColumn) <= 1;
}


/**
 * @brief Return a random float between two values [a, b].
 * @param minimalValue a.
 * @param maximalValue b.
 * @return A random double between a and b.
 */
double randomDoubleBetween(double minimalValue, double maximalValue) {

	if (minimalValue > maximalValue) {
		double temporaryValue = minimalValue;
		minimalValue = maximalValue;
		maximalValue = temporaryValue;
	}

	double scale = rand() / (double) RAND_MAX;

	return minimalValue + scale * (maximalValue - minimalValue);
}


/**
 * @brief Return a random int between two values [a, b[.
 * @param minimalValue a.
 * @param maximalValue b.
 * @return A random int between a and b.
 */
int randomIntBetween(int minimalValue, int maximalValue) {

	return rand() % (maximalValue - minimalValue) + minimalValue;
}


/**
 * @brief Return true with a probability p.
 * @param probability p.
 * @return True with a probability p.
 */
boolean isTrueWithProbabilityOf(double probability) {

	double prob = rand() / (double) RAND_MAX;
	return prob < probability;
}


/**
 * @brief Display an errorMessage before exiting the program.
 * @param errorMessage The message to print.
 */
void error(char* message) {

	fprintf(stderr, "%s\n", message);
	exit(EXIT_FAILURE);
}


/**
 * @brief Round double in int.
 * @param value The double to round.
 * @return The rounded integer.
 */
int roundInt(double value) {
	if (value - (int) value >= 0.5) {
		return (int) value + 1;
	}
	return (int) value;
}


/**
 * @brief Update all the data of the city.
 * @param city The city whose data needs to be updated.
 * @param data The set of data to be updated and transmitted.
 */
void updateAllData(City* city, dataFile* data) {
	
	unsigned numberAlive = 0;

	for (unsigned index = 0; index < NUMBER_PEOPLE; index++) {
		updateData(getPerson(city, index), data);
		if (isAlive(getPerson(city, index))) numberAlive ++;
	}
	data -> globalContaminationRate /= numberAlive;
}


/**
 * @brief Update all the data of the city.
 * @param person The person whose data is being analysed.
 * @param data The set of data to be updated.
 */
void updateData(Person* person, dataFile* data) {
    
	switch (person -> healthState) {
		case HEALTHY:
			(data -> numberOfHealthyPerson)++;
			break;

		case SICK:
			(data -> numberOfSickPerson)++;
			break;

		case DEAD:
			(data -> numberOfDeadPerson)++;
			break;
            
		case BURNT:
			(data -> numberOfBurnedDeadBodies)++;
			break;

		default:
			break;
	}

	data -> globalContaminationRate += person -> contaminationRate;
}