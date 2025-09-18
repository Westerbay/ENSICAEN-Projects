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
 * @file file_manager.h
 * 
 * @brief Set of functions for managing evolution.txt.
 * 
 * @author Chauvet Lucie <lucie.chauvet@ecole.ensicaen.fr>
 */


#ifndef __FILE_MANAGER_H__
#define __FILE_MANAGER_H__

#define EVOLUTION_PATH "resources/evolution.txt"
#define PRESS_PATH "resources/evolutionPress.txt"
#define GNUPLOT_PATH "resources/gnuplot.gp"
#define PRESS_GNUPLOT_PATH "resources/gnuplotPress.gp"

#define EPIDEMIC_SIMULATION_PATH "./bin/epidemic_sim"
#define TIMER_PATH "./bin/timer"
#define CITIZEN_MANAGER_PATH "./bin/citizen_manager"
#define PRESS_AGENCY_PATH "./bin/press_agency"


/**
 * @brief Data to put in the txt file.
 */
typedef struct {

    unsigned day;
    unsigned numberOfHealthyPerson;
    unsigned numberOfSickPerson;
    unsigned numberOfDeadPerson;
    unsigned numberOfBurnedDeadBodies;
    double globalContaminationRate;
    double firstContaminationReporter;
    double secondContaminationReporter;
    
} dataFile;


/**
 * @brief Clear the txt file evolution.txt.
 * @param filename The filename of the file.
 */
void clearFile(char* filename);


/**
 * @brief Write in evolution.txt the data.
 * @param filename The filename of the file.
 * @param dataToWrite In the file.
 */
void writeInFile(char* filename, dataFile* dataToWrite);


/**
 * @brief Updates the gnuplot graph related to evolution.txt.
 * @param filename The filename of gnuplot command.
 */
void generateGnuplot(char* filename);

#endif /* __FILE_MANAGER_H__ */

