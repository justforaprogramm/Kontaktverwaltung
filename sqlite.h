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

/**
 * defines
 */
#define DATABASE "Contacts.db"

/**
 * private functions in "sqlite.c"
 */
int mySqlite(int exec);
int execute_internal_sql(const char *sql);


#endif // SQLITE_H