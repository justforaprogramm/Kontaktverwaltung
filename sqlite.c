/**
 * COPYRIGHT Benjamin Schmid <inf25189@lehre.dhbw-stuttgart.de>
 */
#include "sqlite.h"

static void clear_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int mySqlite(int exec) {
    sqlite3 *db;
    char sql[1024];

    static int seeded = 0;
    if (!seeded) {
        srand(time(NULL));
        seeded = 1;
    }

    switch(exec) {
        case 1:
            execute_internal_sql(
                "CREATE TABLE IF NOT EXISTS kontakte ("
                "id INTEGER PRIMARY KEY, " 
                "vorname TEXT, mittelname TEXT, nachname TEXT, geburtstag TEXT);"
                "CREATE TABLE IF NOT EXISTS telefone ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, kontakt_id INTEGER, "
                "typ TEXT, nummer TEXT, FOREIGN KEY(kontakt_id) REFERENCES kontakte(id) ON DELETE CASCADE);"
                "CREATE TABLE IF NOT EXISTS mails ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, kontakt_id INTEGER, "
                "typ TEXT, adresse TEXT, FOREIGN KEY(kontakt_id) REFERENCES kontakte(id) ON DELETE CASCADE);"
            );
            break;

        case 2: {
            char v[50], m[50], n[50], g[50], typ[20], val[100], choice[5];
            int random_id; // Variable for our 9-digit ID
            sqlite3 *db;

            clear_buffer(); 
            printf("First Name: "); if(scanf("%49s", v)) {}
            
            clear_buffer();
            printf("Middle Name (Press ENTER to skip): ");
            if (fgets(m, sizeof(m), stdin)) {
                m[strcspn(m, "\n")] = 0;
            }

            printf("Last Name: "); if(scanf("%49s", n)) {}
            printf("Birthday: "); if(scanf("%49s", g)) {}

            random_id = 100000000 + rand() % 900000000;

            if (sqlite3_open("Contacts.db", &db) != SQLITE_OK) {
                fprintf(stderr, "Could not open database\n");
                break;
            }
            
            sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0);

            sprintf(sql, "INSERT INTO kontakte (id, vorname, mittelname, nachname, geburtstag) VALUES (%d, '%s','%s','%s','%s');", 
                    random_id, v, m, n, g);

            if (sqlite3_exec(db, sql, 0, 0, 0) != SQLITE_OK) {
                fprintf(stderr, "Failed to insert contact: %s\n", sqlite3_errmsg(db));
                sqlite3_close(db);
                break;
            }

            int last_id = random_id;

            do {
                printf("Add phone number? (y/n): "); if(scanf("%4s", choice)) {}
                if (strcmp(choice, "y") == 0) {
                    printf("Type (e.g. Mobile): "); if(scanf("%19s", typ)) {}
                    printf("Number: "); if(scanf("%99s", val)) {}
                    
                    sprintf(sql, "INSERT INTO telefone (kontakt_id, typ, nummer) VALUES (%d, '%s', '%s');", last_id, typ, val);
                    char *err_msg = 0;
                    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
                        fprintf(stderr, "SQL Error: %s\n", err_msg);
                        sqlite3_free(err_msg);
                    }
                }
            } while (strcmp(choice, "y") == 0);

            do {
                printf("Add E-Mail? (y/n): "); if(scanf("%4s", choice)) {}
                if (strcmp(choice, "y") == 0) {
                    printf("Type (e.g. Work): "); if(scanf("%19s", typ)) {}
                    printf("Address: "); if(scanf("%99s", val)) {}
                    
                    sprintf(sql, "INSERT INTO mails (kontakt_id, typ, adresse) VALUES (%d, '%s', '%s');", last_id, typ, val);
                    char *err_msg = 0;
                    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
                        fprintf(stderr, "SQL Error: %s\n", err_msg);
                        sqlite3_free(err_msg);
                    }
                }
            } while (strcmp(choice, "y") == 0);

            sqlite3_close(db);
            printf("Contact added successfully (ID: %d)\n", last_id);
            break;
        }

        case 3: { // editContact
            int id;
            char v[50], m[50], n[50], g[50];
            int sub_choice;

            printf("Enter the ID of the contact you want to edit: ");
            if (scanf("%d", &id) != 1) {
                clear_buffer();
                printf("Invalid ID format.\n");
                break;
            }
            clear_buffer();

            printf("\n--- CURRENT DATA FOR ID %d ---\n", id);
            if (sqlite3_open("Contacts.db", &db) == SQLITE_OK) {
                sprintf(sql, "SELECT * FROM kontakte WHERE id = %d;", id);
                sqlite3_exec(db, sql, print_callback, 0, 0);
                sqlite3_close(db);
            }

            printf("\nWhat would you like to edit?\n");
            printf("1. Name & Birthday\n");
            printf("2. Add a new Phone Number\n");
            printf("3. Add a new E-Mail\n");
            printf("4. Cancel\n");
            printf("Choice: ");
            
            if (scanf("%d", &sub_choice) != 1) {
                clear_buffer();
                break;
            }
            clear_buffer();

            switch(sub_choice) {
                case 1: // Update Basic Info
                    printf("New First Name: "); 
                    if (scanf("%49s", v)) {} // Adding the if() silences the warning
                    clear_buffer();

                    printf("New Middle Name (Press ENTER to skip): ");
                    if (fgets(m, sizeof(m), stdin)) { m[strcspn(m, "\n")] = 0; }

                    printf("New Last Name: "); 
                    if (scanf("%49s", n)) {} 
                    clear_buffer();

                    printf("New Birthday: "); 
                    if (scanf("%49s", g)) {} 
                    clear_buffer();

                    sprintf(sql, "UPDATE kontakte SET vorname='%s', mittelname='%s', nachname='%s', geburtstag='%s' WHERE id=%d;", 
                            v, m, n, g, id);
                    execute_internal_sql(sql);
                    printf("Contact updated successfully.\n");
                    break;

                case 2: { // Add phone
                    char typ[20], num[100];
                    printf("Type (e.g. Work): "); 
                    if (scanf("%19s", typ)) {} 
                    clear_buffer();

                    printf("Number: "); 
                    if (scanf("%99s", num)) {} 
                    clear_buffer();
                    
                    sprintf(sql, "INSERT INTO telefone (kontakt_id, typ, nummer) VALUES (%d, '%s', '%s');", id, typ, num);
                    execute_internal_sql(sql);
                    printf("Phone number added.\n");
                    break;
                }

                case 3: { // Add email
                    char typ[20], adr[100];
                    printf("Type (e.g. Private): "); 
                    if (scanf("%19s", typ)) {} 
                    clear_buffer();

                    printf("Address: "); 
                    if (scanf("%99s", adr)) {} 
                    clear_buffer();
                    
                    sprintf(sql, "INSERT INTO mails (kontakt_id, typ, adresse) VALUES (%d, '%s', '%s');", id, typ, adr);
                    execute_internal_sql(sql);
                    printf("Email added.\n");
                    break;
                }

                default:
                    printf("Edit cancelled.\n");
                    break;
            }
            break;
        }

        case 4: { // Remove (Pop) Contact
            int id;
            printf("Enter ID to delete: ");
            if (scanf("%d", &id) != 1) break;

            printf("\n--- PREVIEW BEFORE DELETION ---\n");
            if (sqlite3_open("Contacts.db", &db) == SQLITE_OK) {
                sprintf(sql, "SELECT * FROM kontakte WHERE id = %d;", id);
                sqlite3_exec(db, sql, print_callback, 0, 0);
                sqlite3_close(db);
            }

            printf("\nAre you sure you want to delete this contact? (y/n): ");
            char confirm;
            if (scanf(" %c", &confirm)) {} 

            if (confirm == 'y' || confirm == 'Y') {
                // Since we have ON DELETE CASCADE, we only need to delete the main record
                sprintf(sql, "DELETE FROM kontakte WHERE id = %d;", id);
                execute_internal_sql(sql);
                printf("Contact deleted successfully.\n");
            } else {
                printf("Deletion cancelled.\n");
            }
            break;
        }

        case 5: { // getContact details
            int id;
            printf("Contact ID: ");
            if (scanf("%d", &id) != 1) break;
            if (sqlite3_open("Contacts.db", &db) == SQLITE_OK) {
                printf("\n--- BASIC DATA ---\n");
                sprintf(sql, "SELECT * FROM kontakte WHERE id = %d;", id);
                sqlite3_exec(db, sql, print_callback, 0, 0);
                printf("--- PHONE ---\n");
                sprintf(sql, "SELECT typ, nummer FROM telefone WHERE kontakt_id = %d;", id);
                sqlite3_exec(db, sql, print_callback, 0, 0);
                printf("--- EMAIL ---\n");
                sprintf(sql, "SELECT typ, adresse FROM mails WHERE kontakt_id = %d;", id);
                sqlite3_exec(db, sql, print_callback, 0, 0);
                sqlite3_close(db);
            }
            break;
        }

        case 6: // getContacts (without details)
            if (sqlite3_open("Contacts.db", &db) == SQLITE_OK) {
                printf("\n--- ALLE KONTAKTE (ID & NAME) ---\n");
                sqlite3_exec(db, "SELECT id, vorname, nachname FROM kontakte;", print_callback, 0, 0);
                sqlite3_close(db);
            }
            break;
    }
    return 0;
}

int execute_internal_sql(const char *sql) {
    sqlite3 *db;
    char *err_msg = 0;
    if (sqlite3_open("Contacts.db", &db) != SQLITE_OK) return 1;
    // Enable foreign keys for CASCADE support
    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0);
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL Error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    sqlite3_close(db);
    return rc;
}

int print_callback(void *data, int argc, char **argv, char **azColName) {
    (void)data; // needet for sql logic
    for (int i = 0; i < argc; i++) {
        printf("%s: %s  ", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}
void removeContact(int id) {
    // 1. Show the user what they are about to delete
    printf("\n--- Previewing Contact ID: %d ---\n", id);
    mySqlite(5);

    printf("\nAre you sure you want to delete this contact? (y/n): ");
    char confirm;
    if (scanf(" %c", &confirm) != 1) {
        printf("Error reading input.\n");
        return;
    }

    if (confirm == 'y' || confirm == 'Y') {
        execute_internal_sql(
            "BEGIN TRANSACTION;"
            "DELETE FROM telefone WHERE kontakt_id = ?;"
            "DELETE FROM mails WHERE kontakt_id = ?;"
            "DELETE FROM kontakte WHERE id = ?;"
            "COMMIT;"
        );
        printf("Contact and associated data deleted successfully.\n");
    } else {
        printf("Deletion cancelled.\n");
    }
}
