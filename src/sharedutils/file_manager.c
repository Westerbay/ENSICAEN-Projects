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
 * @file file_manager.c 
 * 
 * @brief Funtions for managing files.
 * 
 * @author Chauvet Lucie <lucie.chauvet@ecole.ensicaen.fr>
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <file_manager.h>
#include <util.h>
#include <people.h>
#include <sys/wait.h>
#include <fcntl.h>


/**
 * @brief Clear the txt file evolution.txt.
 * @param filename The filename of the file.
 */
void clearFile(char* filename) {  

	FILE* file = fopen(filename, "w");

	if (file == NULL) {
		error("Error when opening evolution.txt");
	}

	fclose(file);
	
	dataFile dataStruct = {0, NUMBER_PEOPLE, 0, 0, 0, 0, 0, 0};

	writeInFile(filename, &dataStruct);
}


/**
 * @brief Write in evolution.txt the data.
 * @param filename The filename of the file.
 * @param dataToWrite In the file.
 */
void writeInFile(char* filename, dataFile* data) {

	FILE* file = fopen(filename, "a");

	if (file == NULL) {
		error("Error when opening evolution.txt");
	}

	fprintf(file, "%u ", data->day);
	fprintf(file, "%u ", data->numberOfHealthyPerson);
	fprintf(file, "%u ", data->numberOfSickPerson);
	fprintf(file, "%u ", data->numberOfDeadPerson);
	fprintf(file, "%u ", data->numberOfBurnedDeadBodies);
	fprintf(file, "%.1f ", toPercent(data->globalContaminationRate));
	fprintf(file, "%.1f ", toPercent(data->firstContaminationReporter));
	fprintf(file, "%.1f\n", toPercent(data->secondContaminationReporter));

	fflush(file);
	fclose(file);
}


/**
 * @brief Updates the gnuplot graph related to evolution.txt.
 * @param filename The filename of gnuplot command.
 */
void generateGnuplot(char* filename) {

	int err;

	if (fork() == 0) {
		dup2(open(TRASH, O_WRONLY), STDERR_FILENO);
		dup2(open(TRASH, O_WRONLY), STDOUT_FILENO);
		err = execlp("gnuplot", "gnuplot", filename, NULL);
		if (err == -1) {
			error("Can't load Gnuplot");
		}
	}	

	wait(NULL);
}