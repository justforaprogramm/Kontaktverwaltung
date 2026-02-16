/**
 * COPYRIGHT Benjamin Schmid <inf25189@lehre.dhbw-stuttgart.de>
 */
#ifndef SQLITE_H
#define SQLITE_H

/**
 * includes
 */
#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/**
 * private functions in "main.c"
 */
int print_callback(void *data, int argc, char **argv, char **azColName);
int execute_internal_sql(const char *sql);
void removeContact(int id);
int mySqlite(int exec);

#endif // SQLITE_H