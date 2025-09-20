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
 * @file citizen_manager.h 
 * 
 * @brief Prototype of functions for person_manager.
 * 
 * @author Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>
 * @author Leconte Audrey <audrey.leconte@ecole.ensicaen.fr>
 * @author Lucie Chauvet <lucie.chauvet@ecole.ensicaen.fr>
 * @author Salic Valentin <valentin.salic@ecole.ensicaen.fr>
 */

#include <stddef.h>
#include <time.h>
#include <mqueue.h>
#include <city.h>
#include <people.h>
#include <util.h>
#include <memory_shared.h>
#include <file_manager.h>


#ifndef __CITIZEN_MANAGER_H__
#define __CITIZEN_MANAGER_H__

#define MOVING_PROBABILITY 0.4

#define NO_NEIGHBORS 0

#define CONTAMINATION_FACTOR_WHILE_MOVING 0.02
#define CONTAMINATION_FACTOR_WHILE_STATIONARY 0.05

#define CELL_CONTAMINATION_MOVEMENT_FACTOR 0.01
#define HOSPITAL_CONTAMINATION_MOVEMENT_FACTOR 0.0025

#define MORTALITY_RISK_PER_DAY 0.05

#define FACTOR_MORTALITY_HOSPITAL 4
#define FACTOR_MORTALITY_DOCTOR_PRESENT 2

#define REDUCTION_INFORMATION_FOR_DEAD_BODIES 0.35
#define REDUCTION_INFORMATION_FOR_BURNED_BODIES 0.35
#define REDUCTION_INFORMATION_FOR_SICK_CITIZENS 0.1
#define JOURNALIST_COMMUNICATION_RATE 0.8

#define SUIT_PROTECTION_FACTOR 0.1
#define PROBABILITY_CONTAMINATED_WITH_SUIT 0.3

#define REDUCTION_CONTAMINATION_HOSPITAL_FACTOR -0.1
#define REDUCTION_CONTAMINATION_FIRE_STATION_FACTOR -0.2

#define PROBABILITY_CONTAMINE_SAME_POSITION 0.1
#define PROBABILITY_CONTAMINE_NEARBY_POSITION 0.01

#define CONTAMINATION_BY_SPREAD 0.01

#define BUFFER_SIZE 10


typedef struct {

    unsigned value[NUMBER_DIRECTION];
    unsigned size;

} NeighborsPositionList;


typedef struct {

    City* city;
    Person* person;
	
} ThreadData;


/**
 * @brief retrieves the cell in the city grid where the specified person is located
 * @param city the city grid
 * @param person the specified person
 * @return a pointer to the cell in the city grid where the person is located
 */
static inline Cell* getPersonCell(City* city, Person* person) {

    return getCell(city, person -> position);
}


/**
 * @brief Updates all people in the city.
 * @param memoryShared The shared memory containing the city information.
 */
void updateAll(MemoryShared* memoryShared);


/**
 * @brief Updates all people in the city by creating threads for each individual.
 * @param city The city object containing all the people.
 */
void updatePeople(City* city);


/**
 * @brief The task performed by each thread to update a specific person's status.
 * @param arg Pointer to the thread data which contains information about the person.
 * @return NULL after completing the task.
 */
void* personTask(void* arg);


/**
 * @brief Updates the person's information, including their position, contamination, mortality risk, health state.
 * @param data The thread data containing the person to update.
 */
void updatePerson(ThreadData* data);


/**
 * @brief Executes specialized tasks based on the person's type (fireman, doctor, reporter).
 * @param data The thread data containing the person to perform the specialized task for.
 */
void specialisedTask(ThreadData* data);


/**
 * @brief Updates the position of the person based on movement rules.
 * @param data The thread data containing the person to update.
 */
void updatePersonPosition(ThreadData* data);


/**
 * @brief Updates the contamination of a person based on surrounding factors and cell type.
 * @param data The thread data containing the person to update.
 */
void updateContamination(ThreadData* data);


/**
 * @brief Updates the mortality risk for a person based on their health state and other factors.
 * @param data The thread data containing the person to update.
 */
void updateMortalityRisk(ThreadData* data);


/**
 * @brief Updates the health state of a person (healthy, sick, or dead) based on probabilities.
 * @param data The thread data containing the person to update.
 */
void updateHealthState(ThreadData* data);


/**
 * @brief Updates the number of sick days for a person if they are sick.
 * @param data The thread data containing the person to update.
 */
void updateNumberSickDay(ThreadData* data);


/**
 * @brief Tests if the person has to move.
 * @param person The person being tested.
 * @return True if the person has to move, false otherwise.
 */
static inline boolean hasToMove(Person* person) {

    return person -> type != DOCTOR && person -> healthState == HEALTHY && person -> wasOnHospitalLastTurn;
}

/**
 * @brief Attempts to move a person to an accessible position.
 * @param city The city structure where the person wants to move.
 * @param person The person who is attempting to move.
 */
void attemptToMovePerson(City* city, Person* person);


/**
 * @brief Gets the list of accessible positions around a person.
 * @param city The city structure containing the city’s layout.
 * @param person The person whose nearby accessible positions are being checked.
 * @param list A pointer to the list where accessible positions will be stored.
 */
void getPersonAccessibleNearbyPosition(City* city, Person* person, NeighborsPositionList* list);


/**
 * @brief Moves a person to a new position within the city.
 * @param city The city structure containing the city's layout.
 * @param person The person to be moved.
 * @param newPosition The position where the person will be moved to.
 */
void movePerson(City* city, Person* person, unsigned newPosition);


/**
 * @brief Increases the contamination rate of a person.
 * @param person The person whose contamination rate will be increased.
 * @param increasedContamination The amount by which the contamination rate will increase.
 */
void increasePersonContamination(Person* person, double increasedContamination);


/**
 * @brief Increases the contamination level of a cell due to a person's movement.
 * @param city The city structure containing the city’s layout.
 * @param person The person whose movement caused the contamination increase.
 */
void increaseCellContaminationDueToPersonMovement(City* city, Person* person);

							
/**
 * @brief Checks if a position is accessible to a person.
 * @param city The city structure containing the city’s layout.
 * @param person The person trying to access the position.
 * @param position The position being checked for accessibility.
 * @return True if the position is accessible, false otherwise.
 */
boolean isPositionAccessible(City* city, Person* person, unsigned position);


/**
 * @brief Checks if a hospital is accessible to a person.
 * @param hospitalCell The cell representing the hospital.
 * @param person The person attempting to enter the hospital.
 * @return True if the hospital is accessible to the person, false otherwise.
 */
boolean isHospitalAccessible(Cell* hospitalCell, Person* person);


/**
 * @brief Checks if a fire station is accessible to a person.
 * @param city The city structure containing the city’s layout.
 * @param firestationCell The cell representing the fire station.
 * @param person The person attempting to enter the fire station.
 * @param fireStationPosition The position of the fire station.
 * @return True if the fire station is accessible to the person, false otherwise.
 */
boolean isFireStationAccessible(City* city, Cell* firestationCell, Person* person, unsigned fireStationPosition);


/**
 * @brief Checks if a fireman is present at a given position.
 * @param city The city structure containing the city's layout.
 * @param position The position being checked.
 * @return true if a fireman is present, false otherwise.
 */
boolean isFiremanPresent(City* city, unsigned position);


/**
 * @brief Checks if a doctor is present at a given position.
 * @param city The city structure containing the city's layout.
 * @param position The position being checked.
 * @return True if a doctor is present, false otherwise.
 */
boolean isDoctorPresent(City* city, unsigned position);


/**
 * @brief Spreads contamination to the surrounding area of a person.
 * @param city The city structure containing the city's layout.
 * @param person The person whose surrounding area is being contaminated.
 */
void contaminateSurroundingArea(City* city, Person* person);


/**
 * @brief Gets the nearby land positions around a given position.
 * @param city The city structure containing the city's layout.
 * @param position The position being checked for nearby land positions.
 * @param list A pointer to the list where nearby land positions will be stored.
 */
void getNearbyLandPosition(City* city, unsigned position, NeighborsPositionList* list);


/**
 * @brief Contaminates people at a specified position.
 * @param city The city structure containing the city’s layout.
 * @param position The position where contamination is being spread.
 */
void contaminatePeopleOnPosition(City* city, unsigned position);


/**
 * @brief Burns people in a specified position who are already dead.
 * @param city The city structure containing the city’s layout.
 * @param position The position where the burning takes place.
 */
void burnPeople(City* city, unsigned position);


/**
 * @brief Manages the spraying activity of a fireman.
 * @param city The city structure containing the city’s layout.
 * @param fireman The fireman managing the spraying action.
 */
void sprayManagement(City* city, Person* fireman);


/**
 * @brief Manages the treatment bag usage by a doctor.
 * @param city The city structure containing the city’s layout.
 * @param doctor The doctor managing the treatment process.
 */
void treatmentBagManagement(City* city, Person* doctor);


/**
 * @brief Change the health state of a person to healthy.
 * @param person The person to be cured for.
 */
void cured(Person* person);


/**
 * @brief Finds the sickest person in a given area.
 * @param city The city structure containing the city’s layout.
 * @param position The position to check for the sickest person.
 * @param changeSickestPerson the function used to compare the severity of the disease.
 * @return The sickest person at the specified position.
 */
Person* getSickestPerson(City* city, unsigned position, void (*changeSickestPerson)(Person**, Person*, double*));


/**
 * @brief Sends information about the city’s health and contamination status to the press agency.
 * @param city The city structure containing the city’s layout and data.
 */
void sendInformationToPressAgency(City* city, Person* person);

#endif /* __CITIZEN_MANAGER_H__ */