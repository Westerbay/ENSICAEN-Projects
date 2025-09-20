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
 * @file simulation.h 
 * 
 * @brief Prototype of function for simulation.
 * 
 * @author Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>
 * @author Chauvet Lucie <lucie.chauvet@ecole.ensicaen.fr>
 */
 
#include <util.h>


#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#define NUMBER_OF_TURN 100


/**
 * @brief Function which starts the simulation and handle the timer management.
 * @param memoryShared The common memory of all programs.
 */
void startSimulation(MemoryShared* memoryShared);

#endif /* __SIMULATION_H__ */

