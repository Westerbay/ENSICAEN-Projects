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
 * @file city.h 
 * 
 * @brief Prototype of data use for city.
 * 
 * @author Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>
 * @author Salic Valentin <valentin.salic@ecole.ensicaen.fr>
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>


#ifndef __CITY_H__
#define __CITY_H__

#define GRID_LENGTH 7
#define GRID_SIZE (GRID_LENGTH * GRID_LENGTH)

#define POSITION_FIRE_STATION_ONE (GRID_LENGTH - 1)
#define POSITION_FIRE_STATION_TWO (GRID_SIZE - GRID_LENGTH) 
#define POSITION_HOSPITAL ((GRID_SIZE - 1) / 2)

#define MAXIMAL_NUMBER_PEOPLE_ON_LAND 16
#define MAXIMAL_NUMBER_PEOPLE_ON_HOUSE 6
#define MAXIMAL_NUMBER_PEOPLE_ON_HOSTPITAL 12
#define MAXIMAL_NUMBER_PEOPLE_ON_FIRE_STATION 8

#define NUMBER_HOUSES 12
#define NUMBER_FIRE_STATION 2 
#define NUMBER_HOSPITAL 1
#define NUMBER_LAND (GRID_SIZE - NUMBER_FIRE_STATION - NUMBER_HOSPITAL - NUMBER_HOUSES)

#define NO_PEOPLE 0
#define NO_CONTAMINATION 0

#define CONTAMINATED_LAND_RATE 0.1
#define MINIMAL_RATE 0.2
#define MAXIMAL_RATE 0.4

#define LAND_CHAR '~'
#define HOUSE_CHAR 'X'
#define HOSPITAL_CHAR 'H'
#define FIRE_CHAR 'F'

#define CONTAMINATION_SOUTHEAST_EAST 0.25
#define CONTAMINATION_NORTHEAST 0.2
#define CONTAMINATION_NORTH_SOUTH 0.15
#define DEFAULT_CONTAMINATION 0.03
#define MIN_CONTAMINATION_RISE 0.01
#define MAX_CONTAMINATION_RISE 0.2

#define NUMBER_FIREMAN 6
#define NUMBER_DOCTOR 4
#define NUMBER_REPORTER 2
#define NUMBER_CITIZEN 25
#define NUMBER_PEOPLE (NUMBER_DOCTOR + NUMBER_FIREMAN + NUMBER_CITIZEN + NUMBER_REPORTER)

#define START_INDEX_FIREMAN 0
#define END_INDEX_FIREMAN (START_INDEX_FIREMAN + NUMBER_FIREMAN)
#define START_INDEX_DOCTOR END_INDEX_FIREMAN
#define END_INDEX_DOCTOR (START_INDEX_DOCTOR + NUMBER_DOCTOR)
#define START_INDEX_REPORTER END_INDEX_DOCTOR
#define END_INDEX_REPORTER (START_INDEX_REPORTER + NUMBER_REPORTER)
#define START_INDEX_CITIZEN END_INDEX_REPORTER
#define END_INDEX_CITIZEN (START_INDEX_CITIZEN + NUMBER_CITIZEN)

#define NUMBER_DIRECTION 8


/**
 * @brief Definition of the boolean type.
 */
typedef enum{false, true} boolean;


/**
 * @brief Definition of direction in a grid which is define in one-dimensional array.
 */
typedef enum {

    NORTH_WEST = -GRID_LENGTH - 1,
    NORTH      = -GRID_LENGTH,
    NORTH_EAST = -GRID_LENGTH + 1,
    WEST       = -1,
    EAST       = 1,
    SOUTH_WEST = GRID_LENGTH - 1,
    SOUTH      = GRID_LENGTH,
    SOUTH_EAST = GRID_LENGTH + 1

} Direction;


/**
 * @brief All directions in a grid which is define in one-dimensional array.
 */
extern const Direction DIRECTIONS[NUMBER_DIRECTION];


/**
 * @brief Different type of cells.
 */
typedef enum { LAND, HOUSE, HOSPITAL, FIRE_STATION } typeOfCell;


/**
 * @brief Structure of a cell in a city.
 */
typedef struct {

    unsigned type;
    double contaminationRate;
    double newContaminationRate;
    unsigned MaximalNumberOfPeople;
    unsigned numberOfPeople;

} Cell;


/**
 * @brief Different people who lives in the city.
 */
typedef enum { FIREMAN, DOCTOR, REPORTER, CITIZEN } PersonType;


/**
 * @brief Different state a citizen can have.
 */
typedef enum { HEALTHY, SICK, DEAD, BURNT} HealthState;


/**
 * @brief Structure representing a citizen in the city, including specialized data
 */
typedef struct {

    unsigned position;
    unsigned healthState;               // HealthState
    unsigned type;                      // PersonType
    unsigned numberSickDay;
    unsigned wasOnHospitalLastTurn;     // Boolean
    unsigned hasDevice;                 // Boolean
    double mortalityRisk;
    double contaminationRate;
    double enteringCellContaminationRate;


    union {

        struct {
            double spray;              
        } fireman;

        struct {
            unsigned treatmentBag;      
        } doctor;
        
    } speciality;

} Person;


/**
 * @brief All people on city.
 */
typedef Person PeopleList[NUMBER_PEOPLE];


/**
 * @brief The city.
 */
typedef struct {
		
    Cell grid[GRID_SIZE];  
    PeopleList list;

} City;


/**
 * @brief Tests the existence of a position.
 * @param position A position.
 * @return True if the position exists in the city.
 */
static inline boolean positionExists(unsigned position) {

    return (position < GRID_SIZE);
}


/**
 * @brief Tests the if a cell can welcome someone.
 * @param cell A cell.
 * @return True if the cell can welcome someone.
 */
static inline boolean canWelcomeSomeone(Cell* cell) {

    return (cell -> numberOfPeople) < (cell -> MaximalNumberOfPeople);
}


/**
 * @brief Return the cell of the city at a certain position.
 * @param city The city.
 * @param position The position of the cell.
 * @return The cell at the position.
 */
static inline Cell* getCell(City* city, unsigned position) {

    return city -> grid + position;
}


/**
 * @brief Return the person of the city's list at a certain index.
 * @param city The city.
 * @param index The index of the person in the list.
 * @return The person wanted.
 */
static inline Person* getPerson(City* city, unsigned index) {

    return city -> list + index;
}


/**
 * @brief Return the cell's type of the city at a certain position.
 * @param city The city.
 * @param position The position of the cell.
 * @return The cell's type at the position.
 */
static inline typeOfCell getTypeOfCell(City* city, unsigned position) {

	return getCell(city, position) -> type;
}


/**
 * @brief Tests the if a cell is a hospital.
 * @param position The position of the cell.
 * @return True if the cell is a hospital.
 */
static inline boolean isHospital(unsigned position) {

    return position == POSITION_HOSPITAL;
}


/**
 * @brief Tests the if a cell is a fire station.
 * @param position The position of the cell.
 * @return True if the cell is a fire station.
 */
static inline boolean isFireStation(unsigned position) {

    return position == POSITION_FIRE_STATION_ONE || position == POSITION_FIRE_STATION_TWO;
}


/**
 * @brief Initialisation of the city.
 * @param city City to initialise.
 */
void initialiseMap(City* city);


/**
 * @brief Initialisation of a cell.
 * @param cell Cell to initialise.
 * @param type Type of cell.
 * @param contaminationRate The initial contamination rate.
 * @param MaximalNumberOfPeople Cannot welcome more people than this parameter.
 * @param numberOfPeople Inital number of people.
 */
void initialiseCell(Cell* cell, typeOfCell type, double contaminationRate, unsigned MaximalNumberOfPeople, unsigned numberOfPeople);


/**
 * @brief Randomly contaminate 10% of land cells.
 * @param city The city.
 * @param maximalNumberOfContaminatedLand The number of land cells to be contaminated.
 */
void contaminateLands(City* city, int maximalNumberOfContaminatedLand);


/**
 * @brief Place houses in the city.
 * @param city The city.
 * @param maximalNumberOfHouses Number of houses to place.
 */
void addHouses(City* city, int maximalNumberOfHouses);


/**
 * @brief Display the type of cells in the city.
 * @param city The city.
 */
void displayCity(City* city);


/**
 * @brief Display the contamination rate of cells in the city.
 * @param city The city.
 */
void displayCityContamination(City* city);


/**
 * @brief Simulate a new day in the city.
 * @param city The city.
 */
void newTurnCity(City* city);


/**
 * @brief Update the data of cells.
 * @param city The city.
 */
void updateCells(City* city);


/**
 * @brief Update the contamination rate of lands near a specified land.
 * @param city The city.
 * @param position The position of the specified land.
 */
void updateLands(City* city, unsigned position);

#endif /* __CITY_H__ */
