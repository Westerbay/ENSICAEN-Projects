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
 * @file memory_shared.h 
 * 
 * @brief Declaration of functions for shared memory.
 *  
 * @author Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>
 */
 
#include <city.h>
#include <util.h>

 
#ifndef __MEMSH_H__
#define __MEMSH_H__

#define FILENAME_MEMORY_SHARED "/os_project_memory"


/**
 * @brief Data for shared memory space.
 */
typedef struct {
	
	unsigned endOfSimulation;
	unsigned mainPID;
	unsigned citizenPID;
	unsigned timerPID;
	unsigned pressPID;
	unsigned turn;
	City city;	

} MemoryShared;


/**
 * @brief Create the memoryshared space for all programs.
 * @param addr The memory struct address.
 * @return The memory shared descriptor.
 */
int createMemoryShared(MemoryShared** addr);


/**
 * @brief Load the memoryshared space.
 * @param addr The memory struct address.
 * @return The memory shared descriptor.
 */
int loadMemoryShared(MemoryShared** addr);


/**
 * @brief Free the memoryshared space.
 * @param shmd The memory shared descriptor.
 * @param memoryShared The memory shared space.
 */
void freeMemoryShared(int shmd, MemoryShared* memoryShared);


/**
 * @brief Stop the process to wait the end of the simulation.
 * @param memoryShared The memory shared space.
 */
void waitForEnd(MemoryShared* memoryShared);

#endif /* __MEMSH_H__ */

