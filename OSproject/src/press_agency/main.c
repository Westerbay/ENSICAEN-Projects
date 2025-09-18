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
 * @brief Main program for press agency.
 * 
 * @author Chauvet Lucie <lucie.chauvet@ecole.ensicaen.fr>
 */

#include <memory_shared.h>
#include <press_agency.h>
#include <file_manager.h>
#include <unistd.h>
#include <sys/wait.h>
#include <mqueue.h>

 
int main(void) {
	
	MemoryShared* memoryShared;	
	int descriptor;
	struct sigaction action;
	mqd_t mq = mq_open(MQ_PATH, O_CREAT, 0644, NULL);
	dataFile dataStruct;
	dataFile dataNULL = {0, 0, 0, 0, 0, 0, 0, 0};	
	
	#if DEBUG
	printf("Press Agency : Waiting for reporters ! \n");
	#endif
	
	clearFile(PRESS_PATH);
	generateGnuplot(PRESS_GNUPLOT_PATH);
	descriptor = loadMemoryShared(&memoryShared);
	memoryShared -> pressPID = getpid();
	
	void handleSignal() {	
		#if DEBUG
		printf("Press Agency : Publishing data ! \n");
		#endif
		receiveAndWriteInformations(memoryShared, &dataStruct);
		writeInFile(PRESS_PATH, &dataStruct);
		generateGnuplot(PRESS_GNUPLOT_PATH);
		dataStruct = dataNULL;
	}
	
	action.sa_handler = &handleSignal;
	action.sa_flags = SA_RESTART;
	sigaction(SIGTURN, &action, NULL);
	
	waitForEnd(memoryShared);
	
	close(descriptor);
	mq_close(mq);
	mq_unlink(MQ_PATH);
	return EXIT_SUCCESS;
}
 
