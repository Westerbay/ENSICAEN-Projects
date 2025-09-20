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
 * @brief Main program for epidimic simulation.
 *
 * @author Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>
 * @author Chauvet Lucie <lucie.chauvet@ecole.ensicaen.fr>
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <memory_shared.h>
#include <file_manager.h>
#include <city.h>
#include <time.h>
#include <simulation.h>
#include <util.h>
#include <file_manager.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


int main(void) {
	
	int descriptor;
	MemoryShared * memoryShared;
	City city;	
	
	srand(time(NULL));	
	initialiseMap(&city);	
	
	#if DEBUG
	printf("Epidemic Sim : Waiting for timer ! \n");
	#endif
	
	descriptor = createMemoryShared(&memoryShared); 
	memoryShared -> endOfSimulation = false;
	memoryShared -> turn = 0;
	memoryShared -> pressPID = 0;
	memoryShared -> timerPID = 0;
	memoryShared -> citizenPID = 0;
	memoryShared -> mainPID = getpid();
	memoryShared -> city = city;    
	
	clearFile(EVOLUTION_PATH);
	generateGnuplot(GNUPLOT_PATH);
	startSimulation(memoryShared);
	sleep(2);
	freeMemoryShared(descriptor, memoryShared);
	
	return EXIT_SUCCESS;
}
 
