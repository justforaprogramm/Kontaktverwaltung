/**
 * COPYRIGHT Benjamin Schmid <inf25189@lehre.dhbw-stuttgart.de>
 */

#ifndef MAIN_H
#define MAIN_H

/**
 * includes
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/**
 * files
 */
#include "sqlite.h"

/**
 * defines
 */
//#define DEBUG
#define HELP_LINE 28
#define INPUT_BUFFER 50

/**
 * private functions in "main.c"
 */
int help(void);
int giveAnswer(char in[INPUT_BUFFER]);

#endif // MAIN_H