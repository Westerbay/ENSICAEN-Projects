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
 * @file people.h 
 * 
 * @brief Prototype of function use for initialise people.
 * 
 * @author Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>
 * @author Salic Valentin <valentin.salic@ecole.ensicaen.fr>
 */

#include <stdlib.h>
#include <city.h>


#ifndef __PEOPLE_H__
#define __PEOPLE_H__

#define NUMBER_TREATMENT_BAG_AT_HOSPITAL 10
#define INITIAL_NUMBER_TREATMENT_BAG 5

#define SPRAY_USAGE_RATE 0.2
#define MAXIMAL_SPRAY 10
#define MAXIMAL_SPRAY_PER_TURN 1
#define INITIAL_SPRAY (MAXIMAL_SPRAY * 0.5)

#define NO_MORTALITY_RISK 0
#define NOT_SICK 0

#define DAY_WITHOUT_MORTALITY_RISK 4
#define NUMBER_DAY_BEFORE_CANNOT_HEAL 10

#define NO_SPECIALITY NULL
#define SPRAY_OF(person) (person -> speciality.fireman.spray)
#define TREATMENT_BAG_OF(person) (person -> speciality.doctor.treatmentBag)
 
 
static inline boolean isAlive(Person* person) {

    return person -> healthState < DEAD;
}


static inline boolean isBurnt(Person* person) {

    return person -> healthState == BURNT;
}


/**
 * @brief Initializes all people in the city.
 * @param city The city where people will be initialized.
 */
void initialisePeople(City* city);


/**
 * @brief Initializes firemen in the city.
 * @param city The city where firemen will be initialized.
 */
void initialiseFiremen(City* city);


/**
 * @brief Initializes doctors in the city.
 * @param city The city where doctors will be initialized.
 */
void initialiseDoctor(City* city);


/**
 * @brief Initializes reporters in the city.
 * @param city The city where reporters will be initialized.
 */
void initialiseReporter(City* city);


/**
 * @brief Initializes citizens in the city.
 * @param city The city where citizens will be initialized.
 */
void initialiseCitizen(City* city);


/**
 * @brief Initializes a person in the city.
 * @param city The city where the person will be initialized.
 * @param person A pointer to the person to initialize.
 * @param type The type of person (e.g., fireman, doctor, reporter, citizen).
 * @param position The position of the person in the city grid.
 * @param specializedData Specialized data (e.g., spray capacity for firemen, 
 * treatment bag number for doctors). This can be NULL if not applicable.
 */
void initialisePerson(City* city, Person* person, PersonType type, unsigned position, const void* specializedData);

#endif /* __PEOPLE_H__ */