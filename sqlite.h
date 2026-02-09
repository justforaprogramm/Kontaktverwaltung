/**
 * COPYRIGHT Benjamin Schmid <inf25189@lehre.dhbw-stuttgart.de>
 */
#ifndef SQLITE_H
#define SQLITE_H

#include <stdio.h>
#include <sqlite3.h>

int mySqlite(int exec);
int execute_internal_sql(const char *sql);

#endif // SQLITE_H