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
 * @file press_agency.h 
 * 
 * @brief Functions for the press agency which communicates with reporters.
 * 
 * @author Chauvet Lucie <lucie.chauvet@ecole.ensicaen.fr>
 */
 
#include <memory_shared.h>
#include <file_manager.h>


#ifndef __PRESS_AGENCY_H__
#define __PRESS_AGENCY_H__

#define MESSAGE_SIZE 1024
#define MQ_PATH "/press_agency"

#define PRIORITY_BURNED_BODIES 9
#define PRIORITY_DEAD_BODIES 8
#define PRIORITY_SICK_BODIES 7
#define PRIORITY_HEALTHY_BODIES 6
#define PRIORITY_CITY_CONTAMINATION_RATE 5
#define PRIORITY_JOURNALIST 1


/**
 * @brief gets datas from reporters through a message queue and puts them in evolution.txt
 * @param memoryShared the common memory of all programms
 * @param dataStruct the arguments needed for file format
 */
void receiveAndWriteInformations(MemoryShared* memoryShared, dataFile* dataStruct);

#endif /* __PRESS_AGENCY_H__ */