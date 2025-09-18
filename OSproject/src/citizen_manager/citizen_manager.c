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
 * @file person_manager.c 
 * 
 * @brief Functions for managing people on city.
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


pthread_mutex_t lock;
pthread_barrier_t barrierPosition;
pthread_barrier_t barrierContamination;
pthread_barrier_t barrierMortalityRisk;
pthread_barrier_t barrierSickDay;
pthread_barrier_t barrierHealthState;


static void reduceContaminationDueToCell(Cell* cell, Person* person) {

	double reductionFactor = 0;

	if (cell -> type == FIRE_STATION) {
		reductionFactor = REDUCTION_CONTAMINATION_FIRE_STATION_FACTOR;
	}

	else if (cell -> type == HOSPITAL) {
		reductionFactor = REDUCTION_CONTAMINATION_HOSPITAL_FACTOR;
	}

	if (reductionFactor == 0) return;

	if (person -> contaminationRate > cell -> contaminationRate) {
		increasePersonContamination(person, person -> enteringCellContaminationRate * reductionFactor);

		if (person -> contaminationRate < cell -> contaminationRate) {
			person -> contaminationRate = cell -> contaminationRate;
		}
	}
}


static void initialisePthreadBarrier() {

	pthread_mutex_init(&lock, NULL);

	pthread_barrier_init(&barrierPosition, NULL, NUMBER_PEOPLE);
	pthread_barrier_init(&barrierContamination, NULL, NUMBER_PEOPLE);
	pthread_barrier_init(&barrierMortalityRisk, NULL, NUMBER_PEOPLE);
	pthread_barrier_init(&barrierSickDay, NULL, NUMBER_PEOPLE);
	pthread_barrier_init(&barrierHealthState, NULL, NUMBER_PEOPLE);
}


static void destroyPthreadBarrier() {

	pthread_barrier_destroy(&barrierPosition);	
	pthread_barrier_destroy(&barrierContamination);	
	pthread_barrier_destroy(&barrierMortalityRisk);	
	pthread_barrier_destroy(&barrierHealthState);	
	pthread_barrier_destroy(&barrierSickDay);

	pthread_mutex_destroy(&lock);
}


static void waitPthreadBarrier() {

	pthread_barrier_wait(&barrierPosition);
	pthread_barrier_wait(&barrierContamination);
	pthread_barrier_wait(&barrierMortalityRisk);
	pthread_barrier_wait(&barrierHealthState);	
	pthread_barrier_wait(&barrierSickDay);
}


static void runTaskWithSynchronization(void (*task)(ThreadData*), ThreadData* data, pthread_barrier_t* barrier) {

	pthread_mutex_lock(&lock);
	task(data);
	pthread_mutex_unlock(&lock);
	if (barrier != NULL) {
		pthread_barrier_wait(barrier);
	}
}


/**
 * @brief Updates all people in the city.
 * @param memoryShared The shared memory containing the city information.
 */
void updateAll(MemoryShared* memoryShared) {

	updatePeople(&memoryShared -> city);

	if (!memoryShared -> pressPID) return;	
	kill(memoryShared -> pressPID, SIGTURN);
}


/**
 * @brief Updates all people in the city by creating threads for each individual.
 * @param city The city object containing all the people.
 */
void updatePeople(City* city) {

	unsigned id = 0;
	pthread_t threads[NUMBER_PEOPLE];
	ThreadData threadData[NUMBER_PEOPLE]; 

	initialisePthreadBarrier();

	for (unsigned index = 0; index < NUMBER_PEOPLE; index++) {

		threadData[index].city = city;
		threadData[index].person = getPerson(city, index);

		pthread_create(threads + (id++), NULL, personTask, threadData + index);
	}

	for (unsigned index = 0; index < id; index++) {
		pthread_join(threads[index], NULL);
	}

	destroyPthreadBarrier();
}


/**
 * @brief The task performed by each thread to update a specific person's status.
 * @param arg Pointer to the thread data which contains information about the person.
 * @return NULL after completing the task.
 */
void* personTask(void* arg) {

	ThreadData* data = (ThreadData*)arg;

	if (isBurnt(data -> person)) {
		waitPthreadBarrier();
		return NULL;
	}

	updatePerson(data);
	runTaskWithSynchronization(specialisedTask, data, NULL);
	return NULL;
}


/**
 * @brief Updates the person's information, including their position, contamination, mortality risk, health state.
 * @param data The thread data containing the person to update.
 */
void updatePerson(ThreadData* data) {	
	
	runTaskWithSynchronization(updatePersonPosition, data, &barrierPosition);
	runTaskWithSynchronization(updateContamination, data, &barrierContamination);
	runTaskWithSynchronization(updateMortalityRisk, data, &barrierMortalityRisk);
	runTaskWithSynchronization(updateHealthState, data, &barrierHealthState);
	runTaskWithSynchronization(updateNumberSickDay, data, &barrierSickDay);
}


/**
 * @brief Executes specialized tasks based on the person's type (fireman, doctor, reporter).
 * @param data The thread data containing the person to perform the specialized task for.
 */
void specialisedTask(ThreadData* data) {

	City* city = data -> city;
	Person* person = data -> person;

	if (isAlive(person)) {
		switch (person -> type) {
			case FIREMAN:
				burnPeople(city, person -> position);
				sprayManagement(city, person);
				break;

			case DOCTOR:
				treatmentBagManagement(city, person);
				break;

			case REPORTER:
				sendInformationToPressAgency(city, person);
				break;

			default:
				break;
		}
	}
}


/**
 * @brief Updates the position of the person based on movement rules.
 * @param data The thread data containing the person to update.
 */
void updatePersonPosition(ThreadData* data) {

	City* city = data -> city;
	Person* person = data -> person;

	if (!isAlive(person)) return;
	
	if (hasToMove(person)) {
		attemptToMovePerson(city, person);
		return;
	}    

	if (isTrueWithProbabilityOf(MOVING_PROBABILITY)) {
		attemptToMovePerson(city, person);
	} else {
		increasePersonContamination(person, CONTAMINATION_FACTOR_WHILE_STATIONARY * getPersonCell(city, person) -> contaminationRate);
	}
}


/**
 * @brief Updates the contamination of a person based on surrounding factors and cell type.
 * @param data The thread data containing the person to update.
 */
void updateContamination(ThreadData* data) {

	City* city = data -> city;
	Person* person = data -> person;
	Cell* personCell = getPersonCell(city, person);

	if (isAlive(person)) {
		reduceContaminationDueToCell(personCell, person);
	}
	
	if (person -> healthState == SICK || person -> healthState == DEAD) {
		if (isTrueWithProbabilityOf(PROBABILITY_CONTAMINE_SAME_POSITION)) {
			contaminatePeopleOnPosition(city, person -> position);
		}

		if (personCell -> type == LAND && isTrueWithProbabilityOf(PROBABILITY_CONTAMINE_NEARBY_POSITION)) {
			contaminateSurroundingArea(city, person);
		}
	}
}


/**
 * @brief Updates the mortality risk for a person based on their health state and other factors.
 * @param data The thread data containing the person to update.
 */
void updateMortalityRisk(ThreadData* data) {

	City* city = data -> city;
	Person* person = data -> person;

	if ((person -> healthState == SICK) && (person -> numberSickDay > DAY_WITHOUT_MORTALITY_RISK)) {
        
		person -> mortalityRisk = (person -> numberSickDay - DAY_WITHOUT_MORTALITY_RISK) * MORTALITY_RISK_PER_DAY;
        
		if (person -> position == POSITION_HOSPITAL) {
			person -> mortalityRisk /= FACTOR_MORTALITY_HOSPITAL;
		} 
		
		else if (isDoctorPresent(city, person -> position)) {
			person -> mortalityRisk /= FACTOR_MORTALITY_DOCTOR_PRESENT;
		}
	} 
}


/**
 * @brief Updates the health state of a person (healthy, sick, or dead) based on probabilities.
 * @param data The thread data containing the person to update.
 */
void updateHealthState(ThreadData* data) {

	Person* person = data -> person;
	
	if ((person -> healthState == HEALTHY) && isTrueWithProbabilityOf(person -> contaminationRate)) {
		person -> healthState = SICK;
	}

	else if ((person -> healthState == SICK) && isTrueWithProbabilityOf(person -> mortalityRisk)) {
		person -> healthState = DEAD;
	}
}


/**
 * @brief Updates the number of sick days for a person if they are sick.
 * @param data The thread data containing the person to update.
 */
void updateNumberSickDay(ThreadData* data) {

	Person* person = data -> person;

	if (person -> healthState == SICK) {
		person -> numberSickDay++;
	} 
}