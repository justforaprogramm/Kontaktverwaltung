/**
 * COPYRIGHT Benjamin Schmid <inf25189@lehre.dhbw-stuttgart.de>
 */
#include "sqlite.h"
#include <string.h>

int execute_internal_sql(const char *sql) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("Contacts.db", &db);
    if (rc != SQLITE_OK) return 1;
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }
    sqlite3_close(db);
    return 0;
}

static int print_callback(void *data, int argc, char **argv, char **azColName) {
    (void)data;
    for (int i = 0; i < argc; i++) {
        printf("%s: %s  ", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int mySqlite(int exec) {
    sqlite3 *db;
    char sql[1024];

    switch(exec) {
        case 1: // Init/MakeTable
            execute_internal_sql(
                "CREATE TABLE IF NOT EXISTS kontakte ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "vorname TEXT, mittelname TEXT, nachname TEXT, geburtstag TEXT);"
                "CREATE TABLE IF NOT EXISTS telefone ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, kontakt_id INTEGER, "
                "typ TEXT, nummer TEXT, FOREIGN KEY(kontakt_id) REFERENCES kontakte(id));"
                "CREATE TABLE IF NOT EXISTS mails ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, kontakt_id INTEGER, "
                "typ TEXT, adresse TEXT, FOREIGN KEY(kontakt_id) REFERENCES kontakte(id));"
            );
            break;

        case 2: { // addContact
            char v[50], m[50], n[50], g[50], typ[20], val[100], choice[5];
            printf("Vorname: "); if(scanf("%49s", v)) {}
            printf("Mittelname: "); if(scanf("%49s", m)) {}
            printf("Nachname: "); if(scanf("%49s", n)) {}
            printf("Geburtstag: "); if(scanf("%49s", g)) {}

            sprintf(sql, "INSERT INTO kontakte (vorname, mittelname, nachname, geburtstag) VALUES ('%s','%s','%s','%s');", v, m, n, g);
            execute_internal_sql(sql);
            
            int last_id = 0;
            if (sqlite3_open("Contacts.db", &db) == SQLITE_OK) {
                sqlite3_stmt *stmt;
                sqlite3_prepare_v2(db, "SELECT last_insert_rowid();", -1, &stmt, 0);
                if (sqlite3_step(stmt) == SQLITE_ROW) last_id = sqlite3_column_int(stmt, 0);
                sqlite3_finalize(stmt);
                sqlite3_close(db);
            }

            do {
                printf("Telefon hinzufügen? (y/n): "); if(scanf("%4s", choice)) {}
                if (strcmp(choice, "y") == 0) {
                    printf("Typ: "); if(scanf("%19s", typ)) {}
                    printf("Nummer: "); if(scanf("%99s", val)) {}
                    sprintf(sql, "INSERT INTO telefone (kontakt_id, typ, nummer) VALUES (%d, '%s', '%s');", last_id, typ, val);
                    execute_internal_sql(sql);
                }
            } while (strcmp(choice, "y") == 0);

            do {
                printf("E-Mail hinzufügen? (y/n): "); if(scanf("%4s", choice)) {}
                if (strcmp(choice, "y") == 0) {
                    printf("Typ: "); if(scanf("%19s", typ)) {}
                    printf("Adresse: "); if(scanf("%99s", val)) {}
                    sprintf(sql, "INSERT INTO mails (kontakt_id, typ, adresse) VALUES (%d, '%s', '%s');", last_id, typ, val);
                    execute_internal_sql(sql);
                }
            } while (strcmp(choice, "y") == 0);
            break;
        }

        case 5: { // getContact (Details)
            int id;
            printf("ID des Kontakts: ");
            if (scanf("%d", &id) != 1) break;
            if (sqlite3_open("Contacts.db", &db) == SQLITE_OK) {
                printf("\n--- BASISDATEN ---\n");
                sprintf(sql, "SELECT * FROM kontakte WHERE id = %d;", id);
                sqlite3_exec(db, sql, print_callback, 0, 0);
                printf("--- TELEFON ---\n");
                sprintf(sql, "SELECT typ, nummer FROM telefone WHERE kontakt_id = %d;", id);
                sqlite3_exec(db, sql, print_callback, 0, 0);
                printf("--- MAIL ---\n");
                sprintf(sql, "SELECT typ, adresse FROM mails WHERE kontakt_id = %d;", id);
                sqlite3_exec(db, sql, print_callback, 0, 0);
                sqlite3_close(db);
            }
            break;
        }

        case 6: // getContacts (Liste)
            if (sqlite3_open("Contacts.db", &db) == SQLITE_OK) {
                printf("\n--- ALLE KONTAKTE (ID & NAME) ---\n");
                sqlite3_exec(db, "SELECT id, vorname, nachname FROM kontakte;", print_callback, 0, 0);
                sqlite3_close(db);
            }
            break;
    }
    return 0;
}