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
 * @file timer.h 
 * 
 * @brief Declaration of functions for timer.
 * 
 * @author Chauvet Lucie <lucie.chauvet@ecole.ensicaen.fr>
 */

#include <unistd.h>
 

#ifndef __TIMER_H__
#define __TIMER_H__

#define TIME_BEFORE_SIGNAL 2


/**
 * @brief Timer for the simulation, sends a signal to the simulation every predefined x seconds.
 * @param pidSimulator The processus id of the simulator.
 */
void startTimer(pid_t pidSimulator);

#endif /* __TIMER_H__ */
