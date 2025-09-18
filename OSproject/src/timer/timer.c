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
 * @file timer.c 
 * 
 * @brief File for timer.
 * 
 * @author Chauvet Lucie <lucie.chauvet@ecole.ensicaen.fr>
 * @author Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>
 */
 
#include <timer.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <memory_shared.h>


/**
 * @brief Timer for the simulation, sends a signal to the simulation every predefined x seconds.
 * @param pidSimulator The processus id of the simulator.
 */
void startTimer(pid_t pidSimulator) {
	
	struct sigaction action;
	MemoryShared* memoryShared;	
	loadMemoryShared(&memoryShared);
	
	void sendSignal() {
		#if DEBUG
		printf("Timer : Sending signal ! \n");
		#endif
		if (memoryShared -> endOfSimulation) {
			return;
		}
		if (memoryShared -> mainPID) {
			kill(pidSimulator, SIGTURN);
		}
		alarm(TIME_BEFORE_SIGNAL);
	}
  
	action.sa_handler = &sendSignal;
	action.sa_flags = SA_RESTART; 
	sigaction(SIGALRM, &action, NULL);
	alarm(1);
	
	while(!memoryShared -> endOfSimulation){
		pause();
	}
}



