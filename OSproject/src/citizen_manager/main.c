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
 * @file main.c
 * 
 * @brief Main program for managing people.
 * 
 * @author Leconte Audrey <audrey.leconte@ecole.ensicaen.fr>
 * @author Lucie Chauvet <lucie.chauvet@ecole.ensicaen.fr>
 * @author Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>
 */

#include <citizen_manager.h>
#include <memory_shared.h>
#include <press_agency.h>
 

int main(void) {  

	struct sigaction action;
	MemoryShared* memoryShared;
	City* city;
	mqd_t mq = mq_open(MQ_PATH, O_CREAT, 0644, NULL);
	
	srand(time(NULL));
	loadMemoryShared(&memoryShared);	
	city = &memoryShared -> city;
	initialisePeople(city); 
	memoryShared -> citizenPID = getpid();
	
	#if DEBUG
	printf("Citizen Manager : Waiting for epidemic sim ! \n");
	#endif
	
	void update() {
		#if DEBUG
		printf("Citizen Manager : NPC are in actions ! \n");
		#endif
		updateAll(memoryShared);
	}

	action.sa_handler = &update;
	action.sa_flags = SA_RESTART;
	sigaction(SIGTURN, &action, NULL);    
    
	waitForEnd(memoryShared);
	
	mq_close(mq);
	mq_unlink(MQ_PATH);
	return EXIT_SUCCESS;
}
 
