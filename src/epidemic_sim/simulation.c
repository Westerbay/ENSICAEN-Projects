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
 * @file simulation.c
 *
 * @brief Initialise the simulation.
 *
 * @author Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>
 * @author Chauvet Lucie <lucie.chauvet@ecole.ensicaen.fr>
 */

#include <memory_shared.h>
#include <city.h>
#include <people.h>
#include <util.h>
#include <stdio.h>
#include <stdlib.h>
#include <simulation.h>
#include <file_manager.h>
#include <timer.h>
#include <sys/wait.h>


static boolean end(MemoryShared* memoryShared) { 

	return memoryShared -> turn >= NUMBER_OF_TURN;
}


static void evolution(City * city, unsigned turn) {
	dataFile dataStruct = {turn, 0, 0, 0, 0, 0, 0, 0};	
	updateAllData(city, &dataStruct);
	writeInFile(EVOLUTION_PATH, &dataStruct);
	generateGnuplot(GNUPLOT_PATH);
}


/**
 * @brief Function which starts the simulation and handle the timer management.
 * @param memoryShared The common memory of all programs.
 */
void startSimulation(MemoryShared* memoryShared) {
    
	City* city;
	struct sigaction action;
	
	city = &memoryShared -> city;
    
	void handleSignal() {	

		if (end(memoryShared)) {
			memoryShared -> endOfSimulation = true;
			if (memoryShared -> citizenPID) {
				kill(memoryShared -> citizenPID, SIGEND);	
			}
			if (memoryShared -> timerPID) {
				kill(memoryShared -> timerPID, SIGEND);	
			}
			if (memoryShared -> pressPID) {
				kill(memoryShared -> pressPID, SIGEND);	
			}			
			return;
		}	

		memoryShared -> turn ++;		
		newTurnCity(city);	

		#ifdef DEBUG		
		if (system("clear") == -1) printf("\n\n");
		printf("Day %d :\n", memoryShared -> turn);
		displayCity(city);
		displayCityContamination(city);
		#endif		
		
		if (!memoryShared -> citizenPID) return;
		kill(memoryShared -> citizenPID, SIGTURN);	
		
		usleep(TIME_BEFORE_SIGNAL * 500);
		evolution(city, memoryShared -> turn);
	}

	action.sa_handler = &handleSignal;
	action.sa_flags = SA_RESTART;
	sigaction(SIGTURN, &action, NULL);    
    
	waitForEnd(memoryShared);
}




