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
 * @file memory_shared.c 
 * 
 * @brief Functions for shared memory.
 * 
 * @author Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>
 */

#include <util.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <memory_shared.h>


/**
 * @brief Create the memoryshared space for all programs.
 * @param addr The memory struct address.
 * @return The memory shared descriptor.
 */
int createMemoryShared(MemoryShared** addr) {

	int shmd = shm_open(FILENAME_MEMORY_SHARED, O_CREAT|O_RDWR, S_IRUSR | S_IWUSR);
	if (shmd == -1) {
		error("Cannot create shared memory !");
	}

	if (ftruncate(shmd, sizeof(MemoryShared)) == -1) {
		error("Cannot truncate shared memory !");
	}

	*addr = mmap(NULL, sizeof(MemoryShared), PROT_READ | PROT_WRITE, MAP_SHARED, shmd, 0);	

	return shmd;
}


/**
 * @brief Load the memoryshared space.
 * @param addr The memory struct address.
 * @return The memory shared descriptor.
 */
int loadMemoryShared(MemoryShared** addr) {

	int shmd = shm_open(FILENAME_MEMORY_SHARED, O_RDWR, S_IRUSR | S_IWUSR);
	if (shmd == -1) {
		error("Cannot open shared memory !");
	}

	*addr = mmap(NULL, sizeof(MemoryShared), PROT_READ | PROT_WRITE, MAP_SHARED, shmd, 0);	

	return shmd;
}


/**
 * @brief Free the memoryshared space.
 * @param shmd The memory shared descriptor.
 * @param memoryShared The memory shared space.
 */
void freeMemoryShared(int shmd, MemoryShared* memoryShared) {

	munmap(memoryShared, sizeof(MemoryShared));
	close(shmd);
	shm_unlink(FILENAME_MEMORY_SHARED);
}


/**
 * @brief Stop the process to wait the end of the simulation.
 * @param memoryShared The memory shared space.
 */
void waitForEnd(MemoryShared* memoryShared) {

	while (!memoryShared -> endOfSimulation) {
		pause();
	}
}
