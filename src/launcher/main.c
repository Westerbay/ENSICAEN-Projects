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
 * @brief Main program for epidimic simulation.
 *
 * @author Dubuisson Mathis <mathis.dubuisson@ecole.ensicaen.fr>
 * @author Chauvet Lucie <lucie.chauvet@ecole.ensicaen.fr>
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <memory_shared.h>
#include <city.h>
#include <time.h>
#include <simulation.h>
#include <util.h>
#include <file_manager.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>


int main(void) {

	int err;	
	pid_t pidOfTimer;

	/* Epidemic Sim */
	if (fork() == 0) {
		dup2(open(TRASH, O_WRONLY), STDOUT_FILENO);
		err = execl(EPIDEMIC_SIMULATION_PATH, "epidemic_sim", NULL, NULL);
		if (err == -1) {
			error("Error when executing epidemic_sim");
		}
	} 	
	
	sleep(2);
	
	/* Timer */
	if ((pidOfTimer = fork()) == 0) {
		dup2(open(TRASH, O_WRONLY), STDOUT_FILENO);
		err = execl(TIMER_PATH, "timer", NULL, NULL);
		if (err == -1) {
			error("Error when executing timer");
		}
	} 

	/* Citizen Manager */
	else if (fork() == 0) {	
		dup2(open(TRASH, O_WRONLY), STDOUT_FILENO);	
		err = execl(CITIZEN_MANAGER_PATH, "citizen_manager", NULL, NULL);
		if (err == -1) {
			error("Error when executing citizen manager");
		}
	} 

	/* Press agency */
	else if (fork() == 0) {
		dup2(open(TRASH, O_WRONLY), STDOUT_FILENO);
		err = execl(PRESS_AGENCY_PATH, "press_agency", NULL, NULL);
		if (err == -1) {
			error("Error when executing press_agency");
		}
	} 

	/* Interface */
	else if (fork() == 0) {
		err = execlp("bash", "bash", "-c", "source venv/bin/activate && python3 bin/interface.pyz 2> resources/log.txt", NULL);
		if (err == -1) {
			sleep(1);
			kill(pidOfTimer, SIGCONT);
			error("Error when executing interface");
		}
	} 
	
	else {
		kill(pidOfTimer, SIGTSTP);
		wait(NULL);
	}   
	
	return EXIT_SUCCESS;
}
 
