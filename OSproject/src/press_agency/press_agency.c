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
 * @file press_agency.c 
 * 
 * @brief Functions used by the press agency.
 * 
 * @author Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>
 * @author Chauvet Lucie <lucie.chauvet@ecole.ensicaen.fr>
 */

#include <press_agency.h>
#include <file_manager.h>
#include <memory_shared.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static unsigned getDay(MemoryShared* memoryShared) {

	return memoryShared -> turn;
}


static void fillDataStruct(dataFile* dataStruct, double data, int priority) {

	switch (priority) {
		case PRIORITY_BURNED_BODIES:
			dataStruct -> numberOfBurnedDeadBodies = data;
			break;

		case PRIORITY_DEAD_BODIES:
			dataStruct -> numberOfDeadPerson = data;
			break;

		case PRIORITY_SICK_BODIES:
			dataStruct -> numberOfSickPerson = data;
			break;
			
		case PRIORITY_CITY_CONTAMINATION_RATE:
			dataStruct -> globalContaminationRate = data;
			break;

		case PRIORITY_HEALTHY_BODIES:
			dataStruct -> numberOfHealthyPerson = data;			
			break;

		case PRIORITY_JOURNALIST:
			if (data >= 1) {
				dataStruct -> firstContaminationReporter = data - 1;
			}
			else {
				dataStruct -> secondContaminationReporter = data;
			}
			break; 

		default:
			break;
	}
}


/**
 * @brief gets datas from reporters through a message queue and puts them in evolution.txt
 * @param memoryShared the common memory of all programms
 * @param dataStruct the arguments needed for file format
 */
void receiveAndWriteInformations(MemoryShared* memoryShared, dataFile* dataToWrite) {
    
	ssize_t n;
	unsigned int priority;
	struct mq_attr attr;
	mqd_t mq;
	char buffer[MESSAGE_SIZE];
	double data;		
     
	mq = mq_open(MQ_PATH, O_RDONLY);
	if (mq == -1) {
		error("Can't open the message queue");
	}
	
	mq_getattr(mq, &attr);
	
	dataToWrite -> day = getDay(memoryShared);
	while (true) {
		if (mq_getattr(mq, &attr) == -1) {
			perror("mq_getattr");
			break;
		}

		if (attr.mq_curmsgs == 0) {
			break;
		}

		n = mq_receive(mq, buffer, attr.mq_msgsize, &priority);
		if (n == -1) {
			perror("mq_receive");
			mq_close(mq);
			break;
		}

		buffer[n] = '\0';
		data = strtof(buffer, NULL);
		fillDataStruct(dataToWrite, data, priority);
	}
	
	mq_close(mq);
	
}

