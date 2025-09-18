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
 * @file action.c 
 * 
 * @brief Action of people.
 * 
 * @author Leconte Audrey <audrey.leconte@ecole.ensicaen.fr>
 * @author Lucie Chauvet <lucie.chauvet@ecole.ensicaen.fr>
 * @author Salic Valentin <valentin.salic@ecole.ensicaen.fr>
 * @author Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>
 */

#include <city.h>
#include <citizen_manager.h>
#include <press_agency.h>
#include <pthread.h>


static void useSpray(double* contaminationRate, double* spray, double* quantityOfSprayUsed) {
	
    *contaminationRate *= (1 - SPRAY_USAGE_RATE);
    *quantityOfSprayUsed += SPRAY_USAGE_RATE;
    *spray -= SPRAY_USAGE_RATE;
}


static void changeSickestPersonPerDay(Person** sickestPerson, Person* personToCompare, double* dayToCompare) {

    if (personToCompare -> numberSickDay > *dayToCompare) {
        *sickestPerson = personToCompare;
        *dayToCompare = personToCompare -> numberSickDay;
    }
}


static void changeSickestPersonPerRate(Person** sickestPerson, Person* personToCompare, double* rateToCompare) {

    if (personToCompare -> contaminationRate > *rateToCompare) {
        *sickestPerson = personToCompare;
        *rateToCompare = personToCompare -> contaminationRate;
    }
}


/**
 * @brief Attempts to move a person to an accessible position.
 * @param city The city structure where the person wants to move.
 * @param person The person who is attempting to move.
 */
void attemptToMovePerson(City* city, Person* person) {

    NeighborsPositionList positionList;
    getPersonAccessibleNearbyPosition(city, person, &positionList);

    if (positionList.size == NO_NEIGHBORS) {
        increasePersonContamination(person, CONTAMINATION_FACTOR_WHILE_STATIONARY * getPersonCell(city, person) -> contaminationRate);
        return;
    }

    int index = randomIntBetween(0, positionList.size);
    movePerson(city, person, positionList.value[index]);
}


/**
 * @brief Gets the list of accessible positions around a person.
 * @param city The city structure containing the city’s layout.
 * @param person The person whose nearby accessible positions are being checked.
 * @param list A pointer to the list where accessible positions will be stored.
 */
void getPersonAccessibleNearbyPosition(City* city, Person* person, NeighborsPositionList* list) {

    unsigned position = person -> position;
    list -> size = NO_NEIGHBORS;

    for (int index = 0; index < NUMBER_DIRECTION; index++) {

        unsigned newPosition = position + DIRECTIONS[index];

        if (isPositionAccessible(city, person, newPosition)) {
            list -> value[(list -> size)++] = newPosition;
        }
    }
}


/**
 * @brief Moves a person to a new position within the city.
 * @param city The city structure containing the city's layout.
 * @param person The person to be moved.
 * @param newPosition The position where the person will be moved to.
 */
void movePerson(City* city, Person* person, unsigned newPosition) {

    person -> wasOnHospitalLastTurn = person -> position == POSITION_HOSPITAL;
    getPersonCell(city, person) -> numberOfPeople--;

    person -> position = newPosition;
    getPersonCell(city, person) -> numberOfPeople++;

    increasePersonContamination(person, CONTAMINATION_FACTOR_WHILE_MOVING * getPersonCell(city, person) -> contaminationRate);
    increaseCellContaminationDueToPersonMovement(city, person);
	
    if (isFireStation(newPosition)) {
        person -> enteringCellContaminationRate = person -> contaminationRate;
        person -> hasDevice = true;
    } 
    
    else if (isHospital(newPosition)) {
        person -> enteringCellContaminationRate = person -> contaminationRate;
    }

    else {
        person -> enteringCellContaminationRate = NO_CONTAMINATION;
    }
}


/**
 * @brief Increases the contamination rate of a person.
 * @param person The person whose contamination rate will be increased.
 * @param increasedContamination The amount by which the contamination rate will increase.
 */
void increasePersonContamination(Person* person, double increasedContamination) {

    if (person -> type == FIREMAN) increasedContamination *= SUIT_PROTECTION_FACTOR;

    person -> contaminationRate += increasedContamination;
}


/**
 * @brief Increases the contamination level of a cell due to a person's movement.
 * @param city The city structure containing the city’s layout.
 * @param person The person whose movement caused the contamination increase.
 */
void increaseCellContaminationDueToPersonMovement(City* city, Person* person) {

    Cell* personCell = getPersonCell(city, person);
    typeOfCell cellType = personCell -> type;

    if (cellType == HOSPITAL) {
        personCell -> newContaminationRate += HOSPITAL_CONTAMINATION_MOVEMENT_FACTOR * person -> contaminationRate;

    } else if (cellType != FIRE_STATION) {
        personCell -> newContaminationRate += CELL_CONTAMINATION_MOVEMENT_FACTOR * person -> contaminationRate;
    }
}


/**
 * @brief Checks if a position is accessible to a person.
 * @param city The city structure containing the city’s layout.
 * @param person The person trying to access the position.
 * @param position The position being checked for accessibility.
 * @return True if the position is accessible, false otherwise.
 */
boolean isPositionAccessible(City* city, Person* person, unsigned position) {

    if (!isPositionReachable(position, person -> position)) return false;
    
    Cell* positionCell = getCell(city, position);

    switch (positionCell -> type) {
        case HOSPITAL:
            return isHospitalAccessible(positionCell, person);

        case FIRE_STATION:
            return isFireStationAccessible(city, positionCell, person, position);

        default:
            return canWelcomeSomeone(positionCell); 
    }
}


/**
 * @brief Checks if a hospital is accessible to a person.
 * @param hospitalCell The cell representing the hospital.
 * @param person The person attempting to enter the hospital.
 * @return True if the hospital is accessible to the person, false otherwise.
 */
boolean isHospitalAccessible(Cell* hospitalCell, Person* person) {
    
    if (canWelcomeSomeone(hospitalCell)) {
        return person -> healthState == SICK || ((person -> type == DOCTOR) && !(person -> wasOnHospitalLastTurn)) || person -> type == FIREMAN;
    }
    return false;  
}


/**
 * @brief Checks if a fire station is accessible to a person.
 * @param city The city structure containing the city’s layout.
 * @param firestationCell The cell representing the fire station.
 * @param person The person attempting to enter the fire station.
 * @param fireStationPosition The position of the fire station.
 * @return True if the fire station is accessible to the person, false otherwise.
 */
boolean isFireStationAccessible(City* city, Cell* firestationCell, Person* person, unsigned fireStationPosition) {
    
    if (canWelcomeSomeone(firestationCell)) {
        return person -> type == FIREMAN || isFiremanPresent(city, fireStationPosition);
    }
    return false;  
}


/**
 * @brief Checks if a fireman is present at a given position.
 * @param city The city structure containing the city's layout.
 * @param position The position being checked.
 * @return true if a fireman is present, false otherwise.
 */
boolean isFiremanPresent(City* city, unsigned position) {
    
    for (unsigned index = START_INDEX_FIREMAN; index < END_INDEX_FIREMAN; index++) {
        if (getPerson(city, index) -> position == position) {
            return true;
        }
    }

    return false;
}


/**
 * @brief Checks if a doctor is present at a given position.
 * @param city The city structure containing the city's layout.
 * @param position The position being checked.
 * @return True if a doctor is present, false otherwise.
 */
boolean isDoctorPresent(City* city, unsigned position) {
    
    for (unsigned index = START_INDEX_DOCTOR; index < END_INDEX_DOCTOR; index++) {
        if (getPerson(city, index) -> position == position) {
            return true;
        }
    }

    return false;
}


/**
 * @brief Spreads contamination to the surrounding area of a person.
 * @param city The city structure containing the city's layout.
 * @param person The person whose surrounding area is being contaminated.
 */
void contaminateSurroundingArea(City* city, Person* person) {

    NeighborsPositionList positionList;
    getNearbyLandPosition(city,  person -> position, &positionList);

    if (positionList.size == NO_NEIGHBORS) return;

    for (unsigned index = 0; index < positionList.size; index++) {
        contaminatePeopleOnPosition(city, positionList.value[index]);
    }
    
}


/**
 * @brief Gets the nearby land positions around a given position.
 * @param city The city structure containing the city's layout.
 * @param position The position being checked for nearby land positions.
 * @param list A pointer to the list where nearby land positions will be stored.
 */
void getNearbyLandPosition(City* city, unsigned position, NeighborsPositionList* list) {

    list -> size = NO_NEIGHBORS;

    for (int index = 0; index < NUMBER_DIRECTION; index++) {

        unsigned newPosition = position + DIRECTIONS[index];

        if (isPositionReachable(newPosition, position) && getCell(city, newPosition) -> type == LAND) {
            list -> value[(list -> size)++] = newPosition;
        }
    }
}


/**
 * @brief Contaminates people at a specified position.
 * @param city The city structure containing the city’s layout.
 * @param position The position where contamination is being spread.
 */
void contaminatePeopleOnPosition(City* city, unsigned position) {
	
    boolean conditionToBeContaminatedBySpread(Person* person) {

        boolean condition = isAlive(person);
        condition &= person -> position == position;
        if (person -> type == FIREMAN) {
            condition &= isTrueWithProbabilityOf(PROBABILITY_CONTAMINATED_WITH_SUIT);
        }

        return condition;
    }

    for (unsigned index = 0; index < NUMBER_PEOPLE; index++) {
        if (conditionToBeContaminatedBySpread(city -> list + index)) {
            increasePersonContamination(getPerson(city, index), CONTAMINATION_BY_SPREAD);
        }
    }
}


/**
 * @brief Burns people in a specified position who are already dead.
 * @param city The city structure containing the city’s layout.
 * @param position The position where the burning takes place.
 */
void burnPeople(City* city, unsigned position) {
	
    for (unsigned index = 0; index < NUMBER_PEOPLE; index++) {
        if ((getPerson(city, index) -> position == position) && (getPerson(city, index) -> healthState == DEAD)) {
            getPerson(city, index) -> healthState = BURNT;
            getCell(city, position) -> numberOfPeople--;
        }
    }
}


/**
 * @brief Manages the spraying activity of a fireman.
 * @param city The city structure containing the city’s layout.
 * @param fireman The fireman managing the spraying action.
 */
void sprayManagement(City* city, Person* fireman) {
	
    if (getTypeOfCell(city, fireman -> position) == FIRE_STATION) {
        SPRAY_OF(fireman) = MAXIMAL_SPRAY;
    }

    double quantityOfSprayUsed = 0.0;

    for (unsigned index = 0; index < NUMBER_PEOPLE; index++) {
        if (SPRAY_OF(fireman) == 0 || quantityOfSprayUsed == MAXIMAL_SPRAY_PER_TURN) return;

        Person* person = getPerson(city, index);
        if (person != fireman && person -> position == fireman -> position && isAlive(person)) {
            useSpray(&(person -> contaminationRate), &SPRAY_OF(fireman), &quantityOfSprayUsed);
        }		
    }
	
    useSpray(&(getPersonCell(city, fireman) -> contaminationRate), &SPRAY_OF(fireman), &quantityOfSprayUsed);
}


/**
 * @brief Manages the treatment bag usage by a doctor.
 * @param city The city structure containing the city’s layout.
 * @param doctor The doctor managing the treatment process.
 */
void treatmentBagManagement(City* city, Person* doctor) {
	
    if (doctor -> position == POSITION_HOSPITAL && !(doctor -> wasOnHospitalLastTurn)) {
        TREATMENT_BAG_OF(doctor) += NUMBER_TREATMENT_BAG_AT_HOSPITAL;
    }

    if ((doctor -> healthState == SICK) && (TREATMENT_BAG_OF(doctor) > 0) && (doctor -> numberSickDay <= NUMBER_DAY_BEFORE_CANNOT_HEAL)) {
        cured(doctor);
        TREATMENT_BAG_OF(doctor)--;

    } else if ((doctor -> healthState == HEALTHY) && (getTypeOfCell(city, doctor -> position) == HOSPITAL)) {
        cured(getSickestPerson(city, doctor -> position, changeSickestPersonPerDay));
    
    } else if ((doctor -> healthState == HEALTHY) && (TREATMENT_BAG_OF(doctor) > 0)) {
        cured(getSickestPerson(city, doctor -> position, changeSickestPersonPerRate));
        TREATMENT_BAG_OF(doctor)--;
    }
}


/**
 * @brief Change the health state of a person to healthy.
 * @param person The person to be cured for.
 */
void cured(Person* person) {

    if (person != NULL) {
        person -> healthState = HEALTHY;
        person -> numberSickDay = NOT_SICK;
        person -> mortalityRisk = NO_MORTALITY_RISK;
    }
}


/**
 * @brief Finds the sickest person in a given area.
 * @param city The city structure containing the city’s layout.
 * @param position The position to check for the sickest person.
 * @param changeSickestPerson the function used to compare the severity of the disease.
 * @return The sickest person at the specified position.
 */
Person* getSickestPerson(City* city, unsigned position, void (*changeSickestPerson)(Person**, Person*, double*)) {

    Person* sickestPerson = NULL;
    double valueToCompare = -1;
	
    for (unsigned index = 0; index < NUMBER_PEOPLE; index++) {
        if ((getPerson(city, index) -> position == position) && (getPerson(city, index) -> healthState == SICK)) {
            changeSickestPerson(&sickestPerson, getPerson(city, index), &valueToCompare);
        }
    }
	
    return sickestPerson;
}


/**
 * @brief Sends information about the city’s health and contamination status to the press agency.
 * @param city The city structure containing the city’s layout and data.
 */
void sendInformationToPressAgency(City* city, Person* person) {
    
    mqd_t mq;
    mq = mq_open(MQ_PATH, O_RDWR | O_NONBLOCK, 0644, NULL); 
    char buffer[BUFFER_SIZE];
    dataFile data = {0, 0, 0, 0, 0, 0, 0, 0};
    double journalistContaminationRate = person -> contaminationRate;
	
    updateAllData(city, &data);

    sprintf(buffer, "%u", roundInt(data.numberOfBurnedDeadBodies * (1-REDUCTION_INFORMATION_FOR_BURNED_BODIES)));
    mq_send(mq, buffer, BUFFER_SIZE, PRIORITY_BURNED_BODIES);

    sprintf(buffer, "%u", roundInt(data.numberOfDeadPerson * (1-REDUCTION_INFORMATION_FOR_DEAD_BODIES)));
    mq_send(mq, buffer, BUFFER_SIZE, PRIORITY_DEAD_BODIES);
	
    sprintf(buffer, "%u", roundInt(data.numberOfSickPerson * (1-REDUCTION_INFORMATION_FOR_SICK_CITIZENS)));
    mq_send(mq, buffer, BUFFER_SIZE, PRIORITY_SICK_BODIES);

    sprintf(buffer, "%u", data.numberOfHealthyPerson);
    mq_send(mq, buffer, BUFFER_SIZE, PRIORITY_HEALTHY_BODIES);
	
    sprintf(buffer, "%f", data.globalContaminationRate);
    mq_send(mq, buffer, BUFFER_SIZE, PRIORITY_CITY_CONTAMINATION_RATE);

    if (journalistContaminationRate > JOURNALIST_COMMUNICATION_RATE) {
        if (getPerson(city, START_INDEX_REPORTER) == person) {
            journalistContaminationRate += 1;
        }
        sprintf(buffer, "%f", journalistContaminationRate);
        mq_send(mq, buffer, BUFFER_SIZE, PRIORITY_JOURNALIST);
    }
	
    mq_close(mq);
	
}