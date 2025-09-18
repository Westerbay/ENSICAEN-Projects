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
 * @file people.c
 * 
 * @brief Functions for initialising people on the city.
 * 
 * @author Salic Valentin <valentin.salic@ecole.ensicaen.fr>
 */

#include <people.h>
#include <city.h>
#include <util.h>


/**
 * @brief Choose a cell which can welcome people.
 * @param city The city.
 * @return The position of the choosen cell.
 */
static unsigned getValidPosition(City* city) {

    unsigned position;

    do {
        position = randomIntBetween(0, GRID_SIZE);
    } while (!canWelcomeSomeone(getCell(city, position)));

    return position;
}


/**
 * @brief Initializes all people in the city.
 * @param city The city where people will be initialized.
 */
void initialisePeople(City* city) {

    initialiseFiremen(city);
    initialiseDoctor(city);
    initialiseReporter(city);
    initialiseCitizen(city);
}


/**
 * @brief Initializes firemen in the city.
 * @param city The city where firemen will be initialized.
 */
void initialiseFiremen(City* city) {

    unsigned index = START_INDEX_FIREMAN;
    double sprayCapacity = MAXIMAL_SPRAY;
    
    initialisePerson(city, getPerson(city, index++), FIREMAN, POSITION_FIRE_STATION_ONE, (const void*)&sprayCapacity);
    initialisePerson(city, getPerson(city, index++), FIREMAN, POSITION_FIRE_STATION_TWO, (const void*)&sprayCapacity);

    sprayCapacity = INITIAL_SPRAY;
    for (; index < END_INDEX_FIREMAN; index++) {
        unsigned position = getValidPosition(city);
        initialisePerson(city, getPerson(city, index), FIREMAN, position, (const void*)&sprayCapacity);
    }
}


/**
 * @brief Initializes doctors in the city.
 * @param city The city where doctors will be initialized.
 */
void initialiseDoctor(City* city) {

    unsigned index = START_INDEX_DOCTOR;
    unsigned numberTreatmentBag = NUMBER_TREATMENT_BAG_AT_HOSPITAL;
    
    initialisePerson(city, getPerson(city, index++), DOCTOR, POSITION_HOSPITAL, (const void*)&numberTreatmentBag);

    numberTreatmentBag = INITIAL_NUMBER_TREATMENT_BAG;
    for (; index < END_INDEX_DOCTOR; index++) {
        unsigned position = getValidPosition(city);
        initialisePerson(city, getPerson(city, index), DOCTOR, position, (const void*)&numberTreatmentBag);
    }
}


/**
 * @brief Initializes reporters in the city.
 * @param city The city where reporters will be initialized.
 */
void initialiseReporter(City* city) {

    for (unsigned index = START_INDEX_REPORTER; index < END_INDEX_REPORTER; index++) {
        unsigned position = getValidPosition(city);
        initialisePerson(city, getPerson(city, index), REPORTER, position, NULL);
    }
}


/**
 * @brief Initializes citizens in the city.
 * @param city The city where citizens will be initialized.
 */
void initialiseCitizen(City* city) {

    for (unsigned index = START_INDEX_CITIZEN; index < END_INDEX_CITIZEN; index++) {
        unsigned position = getValidPosition(city);
        initialisePerson(city, getPerson(city, index), CITIZEN, position, NULL);
    }
}


/**
 * @brief Initializes a person in the city.
 * @param city The city where the person will be initialized.
 * @param person A pointer to the person to initialize.
 * @param type The type of person (e.g., fireman, doctor, reporter, citizen).
 * @param position The position of the person in the city grid.
 * @param specializedData Specialized data (e.g., spray capacity for firemen, 
 * treatment bag number for doctors). This can be NULL if not applicable.
 */
void initialisePerson(City* city, Person* person, PersonType type, unsigned position, const void* specializedData) {

    person -> position = position;
    person -> healthState = HEALTHY;
    person -> type = type;    
    person -> numberSickDay = NOT_SICK;        
    person -> wasOnHospitalLastTurn = false;
    person -> hasDevice = (type == FIREMAN) ? true : false;
    person -> mortalityRisk = NO_MORTALITY_RISK;
    person -> contaminationRate = NO_CONTAMINATION;
    person -> enteringCellContaminationRate = NO_CONTAMINATION;

    if (type == FIREMAN && specializedData != NO_SPECIALITY) {
        SPRAY_OF(person) = *((double*)specializedData);
    } 
    
    else if (type == DOCTOR && specializedData != NO_SPECIALITY) {
        TREATMENT_BAG_OF(person) = *((unsigned*)specializedData);
    } 

    getCell(city, position) -> numberOfPeople++;
}