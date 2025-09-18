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
 * @brief Main program for timer.
 * 
 * @author Chauvet Lucie <lucie.chauvet@ecole.ensicaen.fr>
 */

#include <timer.h>
#include <stdlib.h>
#include <util.h>
#include <memory_shared.h>


int main(void) {	
	
	MemoryShared* memoryShared;
	
	loadMemoryShared(&memoryShared);
	memoryShared -> timerPID = getpid();
	startTimer(memoryShared -> mainPID);	

	return EXIT_SUCCESS;
}
 
