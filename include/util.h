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
 * @file util.h
 * 
 * @brief Set of functions used by more than one program.
 * 
 * @author Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>
 * @author Chauvet Lucie <lucie.chauvet@ecole.ensicaen.fr>
 * @author Salic Valentin <valentin.salic@ecole.ensicaen.fr>
 * @author Leconte Audrey <audrey.leconte@ecole.ensicaen.fr>
 */

#include <city.h>
#include <file_manager.h>
#include <people.h>

#ifndef __UTIL_H__
#define __UTIL_H__

#define SIGTURN SIGUSR1
#define SIGEND SIGUSR2
#define DEBUG 1
#define TRASH "/dev/null"
#define LOG "/resources/log.txt"


/**
 * @brief Tells if a position is reachable or not in the city map.
 * @param newPosition The position the citizen is willing to go to.
 * @param currentPosition The current position of the citizen.
 * @return If the citizen can reach the new position or not.
 */
boolean isPositionReachable(unsigned newPosition, unsigned currentPosition);


/**
 * @brief Return a random float between two values [a, b].
 * @param minimalValue a.
 * @param maximalValue b.
 * @return A random double between a and b.
 */
double randomDoubleBetween(double minimalValue, double maximalValue);


/**
 * @brief Return a random int between two values [a, b[.
 * @param minimalValue a.
 * @param maximalValue b.
 * @return A random int between a and b.
 */
int randomIntBetween(int minimalValue, int maximalValue);


/**
 * @brief Return true with a probability p.
 * @param probability p.
 * @return True with a probability p.
 */
boolean isTrueWithProbabilityOf(double probability);


/**
 * @brief Display an errorMessage before exiting the program.
 * @param errorMessage The message to print.
 */
void error(char * errorMessage);


/**
 * @brief calculates the absolute difference between two unsigned integers
 * @param x the first unsigned integer
 * @param y the second unsigned integer
 * @return the absolute difference between x and y
 */
static inline unsigned unsignedAbsoluteDifference(unsigned x, unsigned y) {    

    return (x > y) ? x - y : y - x;
}


/**
 * @brief Percentage change a double.
 * @param rate The double to change in percentage.
 * @return A double corresponding to rate in percentage.
 */
static inline double toPercent(double rate) {

	return rate * 100;
}


/**
 * @brief Round double in int.
 * @param value The double to round.
 * @return The rounded integer.
 */
int roundInt(double value);


/**
 * @brief Update all the data of the city.
 * @param city The city whose data needs to be updated.
 * @param data The set of data to be updated and transmitted.
 */
void updateAllData(City* city, dataFile* data);


/**
 * @brief Update all the data of the city.
 * @param person a citizen whose data needs to be updated.
 * @param data The set of data to be updated and transmitted.
 */
void updateData(Person* person, dataFile* data);

#endif /* __UTIL_H__ */
