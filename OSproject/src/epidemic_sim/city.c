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
 * @file city.c
 *
 * @brief Initialise the city and the people on it.
 *
 * @author Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>
 * @author Salic Valentin <valentin.salic@ecole.ensicaen.fr>
 */

#include <city.h>
#include <people.h>
#include <util.h>
#include <stdio.h>
#include <stdlib.h>



static void contaminationNeighbourLand(Cell* origin, Cell* neighbour, double probabilityContamination) {

	double riseRate;
	double differenceContamination;

	if (isTrueWithProbabilityOf(probabilityContamination)) {
		differenceContamination = origin -> contaminationRate - neighbour -> contaminationRate;
		riseRate = randomDoubleBetween(MIN_CONTAMINATION_RISE, MAX_CONTAMINATION_RISE);
		neighbour -> newContaminationRate += differenceContamination * riseRate;
	}
}


/**
 * @brief Initialisation of the city.
 * @param city City to initialise.
 */
void initialiseMap(City* city) {
	
	unsigned numberOfContaminatedLand;
	numberOfContaminatedLand = roundInt(NUMBER_LAND * CONTAMINATED_LAND_RATE);

	for (unsigned position = 0; position < GRID_SIZE; position++) {

		if (isFireStation(position)) {
			initialiseCell(getCell(city, position), FIRE_STATION, NO_CONTAMINATION, MAXIMAL_NUMBER_PEOPLE_ON_FIRE_STATION, NO_PEOPLE);
		} 
		
		else if (isHospital(position)) {
			initialiseCell(getCell(city, position), HOSPITAL, NO_CONTAMINATION, MAXIMAL_NUMBER_PEOPLE_ON_HOSTPITAL, NO_PEOPLE);
		} 
		
		else {
			initialiseCell(getCell(city, position), LAND, NO_CONTAMINATION, MAXIMAL_NUMBER_PEOPLE_ON_LAND, NO_PEOPLE);
		}
	}    

	addHouses(city, NUMBER_HOUSES);
	contaminateLands(city, numberOfContaminatedLand);
}


/**
 * @brief Initialisation of a cell.
 * @param cell Cell to initialise.
 * @param type Type of cell.
 * @param contaminationRate The initial contamination rate.
 * @param MaximalNumberOfPeople Cannot welcome more people than this parameter.
 * @param numberOfPeople Inital number of people.
 */
void initialiseCell(Cell* cell, typeOfCell type, double contaminationRate, unsigned MaximalNumberOfPeople, unsigned numberOfPeople) {

	Cell newCell = {
		type,
		contaminationRate,
		NO_CONTAMINATION,
		MaximalNumberOfPeople,
		numberOfPeople
	};

	*cell = newCell;
}


/**
 * @brief Randomly contaminate 10% of land cells.
 * @param city The city.
 * @param maximalNumberOfContaminatedLand The number of land cells to be contaminated.
 */
void contaminateLands(City* city, int maximalNumberOfContaminatedLand) {
	
	int numberOfContaminatedLand = 0;

	while (numberOfContaminatedLand < maximalNumberOfContaminatedLand) {

		int position = randomIntBetween(0, GRID_SIZE);
		Cell* cell = getCell(city, position);

		if (cell -> type == LAND && cell -> contaminationRate == NO_CONTAMINATION) {
			cell -> contaminationRate = randomDoubleBetween(MINIMAL_RATE, MAXIMAL_RATE);
			numberOfContaminatedLand++;
		}
	}
}


/**
 * @brief Place houses in the city.
 * @param city The city.
 * @param maximalNumberOfHouses Number of houses to place.
 */
void addHouses(City* city, int maximalNumberOfHouses) {

	int numberOfHouses = 0;

	while (numberOfHouses < maximalNumberOfHouses) {

		int position = randomIntBetween(0, GRID_SIZE);

		if (getTypeOfCell(city, position) == LAND) {
			initialiseCell(getCell(city, position), HOUSE, NO_CONTAMINATION, MAXIMAL_NUMBER_PEOPLE_ON_HOUSE, NO_PEOPLE);
			numberOfHouses++;
		}
	}
}


/**
 * @brief Display the type of cells in the city.
 * @param city The city.
 */
void displayCity(City* city) {

	for (unsigned position = 0; position < GRID_SIZE; position++) {
    
		if (position % GRID_LENGTH == 0) printf("\n");

		switch (getTypeOfCell(city, position)) {
			case LAND:
				printf("%c", LAND_CHAR);
				break;

			case HOUSE:
				printf("%c", HOUSE_CHAR);
				break;

			case HOSPITAL:
				printf("%c", HOSPITAL_CHAR);
				break;

			case FIRE_STATION:
				printf("%c", FIRE_CHAR);
				break;
		}
		printf("    ");
	}
	printf("\n");
}


/**
 * @brief Display the contamination rate of cells in the city.
 * @param city The city.
 */
void displayCityContamination(City* city) {

	for (unsigned position = 0; position < GRID_SIZE; position++) {

		if (position % GRID_LENGTH == 0) printf("\n");

		printf("%.2f ", getCell(city, position) -> contaminationRate);
	}
	printf("\n");
}


/**
 * @brief Simulate a new day in the city.
 * @param city The city.
 */
void newTurnCity(City* city) {	

	updateCells(city);

	for (unsigned position = 0; position < GRID_SIZE; position++) {
		if (getTypeOfCell(city, position) == LAND) {
			updateLands(city, position);
		}
	}	
}


/**
 * @brief Update the data of cells.
 * @param city The city.
 */
void updateCells(City* city) {

	for (int position = 0; position < GRID_SIZE; position++) {
		getCell(city, position) -> contaminationRate += getCell(city, position) -> newContaminationRate;
		getCell(city, position) -> newContaminationRate = NO_CONTAMINATION;
	}
}


/**
 * @brief Update the contamination rate of lands near a specified land.
 * @param city The city.
 * @param position The position of the specified land.
 */
void updateLands(City* city, unsigned position) {
	
	Cell* cell;
	Cell* neighbour;
	double probabilityContamination;
	
	for (int index = 0; index < NUMBER_DIRECTION; index++) {

		unsigned newPosition = position + DIRECTIONS[index];

		if (!isPositionReachable(newPosition, position)) continue;

		cell = getCell(city, position);
		neighbour = getCell(city, newPosition);

		if (cell -> contaminationRate <= neighbour -> contaminationRate) continue;

		if (getTypeOfCell(city, newPosition) == LAND) {  

			if (index == SOUTH_EAST || index == EAST) {
				probabilityContamination = CONTAMINATION_SOUTHEAST_EAST;
			} 
			
			else if (index == NORTH_EAST) {
				probabilityContamination = CONTAMINATION_NORTHEAST;
			} 
			
			else if (index == NORTH || index == SOUTH) {
				probabilityContamination = CONTAMINATION_NORTH_SOUTH;
			} 
			
			else {
				probabilityContamination = DEFAULT_CONTAMINATION;
			}
			
			contaminationNeighbourLand(cell, neighbour, probabilityContamination);
		}
	}
}




