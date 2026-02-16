/**
 * COPYRIGHT Benjamin Schmid <inf25189@lehre.dhbw-stuttgart.de>
 */
#include "sqlite.h"

static void clear_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


int mySqlite(int exec)
{
    sqlite3 *db;
    char sql[1024];
    DataPayload data; // Instance for use in various cases

    static int seeded = 0;
    if (!seeded)
    {
        srand(time(NULL));
        seeded = 1;
    }

    switch(exec)
    {
        case 1: // Initialize English Tables
            execute_internal_sql(
                "CREATE TABLE IF NOT EXISTS contacts ("
                "id INTEGER PRIMARY KEY, " 
                "first_name TEXT, middle_name TEXT, last_name TEXT, birthday TEXT);"
                "CREATE TABLE IF NOT EXISTS phones ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, contact_id INTEGER, "
                "typ TEXT, phone_number TEXT, FOREIGN KEY(contact_id) REFERENCES contacts(id) ON DELETE CASCADE);"
                "CREATE TABLE IF NOT EXISTS emails ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, contact_id INTEGER, "
                "typ TEXT, email_address TEXT, FOREIGN KEY(contact_id) REFERENCES contacts(id) ON DELETE CASCADE);"
            );
            break;

        case 2: // Add Contact
        {
            int random_id; 

            clear_buffer(); 
            (void)printf("First Name: "); if(scanf("%49s", data.firstname)) {}
            
            clear_buffer();
            (void)printf("Middle Name (Press ENTER to skip): ");
            if (fgets(data.middlename, sizeof(data.middlename), stdin))
            {
                data.middlename[strcspn(data.middlename, "\n")] = 0;
            }

            (void)printf("Last Name: "); if(scanf("%49s", data.lastname)) {}
            (void)printf("Birthday: "); if(scanf("%49s", data.birthday)) {}

            random_id = 100000000 + rand() % 900000000;

            if (sqlite3_open("Contacts.db", &db) != SQLITE_OK)
            {
                fprintf(stderr, "Could not open database\n");
                break;
            }
            
            sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0);

            sprintf(sql, "INSERT INTO contacts (id, first_name, middle_name, last_name, birthday) VALUES (%d, '%s','%s','%s','%s');", 
                    random_id, data.firstname, data.middlename, data.lastname, data.birthday);

            if (sqlite3_exec(db, sql, 0, 0, 0) != SQLITE_OK)
            {
                fprintf(stderr, "Failed to insert contact: %s\n", sqlite3_errmsg(db));
                sqlite3_close(db);
                break;
            }

            // Phone Loop
            do {
                (void)printf("Add phone number? (y/n): "); if(scanf("%4s", data.choice)) {}
                if (strcmp(data.choice, "y") == 0) {
                    (void)printf("Type (e.g. Mobile): "); if(scanf("%19s", data.typ)) {}
                    (void)printf("Number: "); if(scanf("%99s", data.val)) {}
                    sprintf(sql, "INSERT INTO phones (contact_id, typ, phone_number) VALUES (%d, '%s', '%s');", random_id, data.typ, data.val);
                    execute_internal_sql(sql);
                }
            } while (strcmp(data.choice, "y") == 0);

            // Email Loop
            do {
                (void)printf("Add E-Mail? (y/n): "); if(scanf("%4s", data.choice)) {}
                if (strcmp(data.choice, "y") == 0) {
                    (void)printf("Type (e.g. Work): "); if(scanf("%19s", data.typ)) {}
                    (void)printf("Address: "); if(scanf("%99s", data.val)) {}
                    sprintf(sql, "INSERT INTO emails (contact_id, typ, email_address) VALUES (%d, '%s', '%s');", random_id, data.typ, data.val);
                    execute_internal_sql(sql);
                }
            } while (strcmp(data.choice, "y") == 0);

            sqlite3_close(db);
            (void)printf("Contact added successfully (ID: %d)\n", random_id);
            break;
        }

        case 3: // Edit Contact
        {
            int id;
            int sub_choice;
            const char *menu = "\n1. Update Name/Bday\n2. Add Phone\n3. Add Email\n4. Del Phone\n5. Del Email\n6. Cancel\nChoice: ";

            (void)printf("Enter ID to edit: ");
            if (scanf("%d", &id) != 1) { clear_buffer(); break; }
            clear_buffer();

            if (sqlite3_open("Contacts.db", &db) == SQLITE_OK) {
                (void)printf("\n--- PHONES ---\n");
                sprintf(sql, "SELECT id, typ, phone_number FROM phones WHERE contact_id = %d;", id);
                sqlite3_exec(db, sql, print_callback, 0, 0);
                (void)printf("--- EMAILS ---\n");
                sprintf(sql, "SELECT id, typ, email_address FROM emails WHERE contact_id = %d;", id);
                sqlite3_exec(db, sql, print_callback, 0, 0);
                sqlite3_close(db);
            }

            (void)printf("%s", menu);
            if (scanf("%d", &sub_choice) != 1) { clear_buffer(); break; }
            clear_buffer();

            switch(sub_choice) {
                case 1:
                    (void)printf("New First Name: "); if (scanf("%49s", data.firstname)) {} clear_buffer();
                    (void)printf("New Middle Name: "); if (fgets(data.middlename, 50, stdin)) { data.middlename[strcspn(data.middlename, "\n")] = 0; }
                    (void)printf("New Last Name: "); if (scanf("%49s", data.lastname)) {} clear_buffer();
                    (void)printf("New Birthday: "); if (scanf("%49s", data.birthday)) {} clear_buffer();
                    sprintf(sql, "UPDATE contacts SET first_name='%s', middle_name='%s', last_name='%s', birthday='%s' WHERE id=%d;", 
                            data.firstname, data.middlename, data.lastname, data.birthday, id);
                    execute_internal_sql(sql);
                    break;
                case 4: // Delete Phone
                    (void)printf("Phone ID to delete: "); if (scanf("%d", &sub_choice) == 1) {
                        sprintf(sql, "DELETE FROM phones WHERE id = %d AND contact_id = %d;", sub_choice, id);
                        execute_internal_sql(sql);
                    }
                    break;
                case 5: // Delete Email
                    (void)printf("Email ID to delete: "); if (scanf("%d", &sub_choice) == 1) {
                        sprintf(sql, "DELETE FROM emails WHERE id = %d AND contact_id = %d;", sub_choice, id);
                        execute_internal_sql(sql);
                    }
                    break;
            }
            break;
        }

        case 4: // Remove Contact
        {
            int id;
            (void)printf("Enter ID to delete: ");
            if (scanf("%d", &id) != 1) break;
            sprintf(sql, "DELETE FROM contacts WHERE id = %d;", id);
            execute_internal_sql(sql);
            (void)printf("Contact deleted.\n");
            break;
        }

        case 5: // Get Contact Details
        {
            int id;
            (void)printf("Contact ID: ");
            if (scanf("%d", &id) != 1) break;
            if (sqlite3_open("Contacts.db", &db) == SQLITE_OK) {
                sprintf(sql, "SELECT * FROM contacts WHERE id = %d;", id);
                sqlite3_exec(db, sql, print_callback, 0, 0);
                sprintf(sql, "SELECT typ, phone_number FROM phones WHERE contact_id = %d;", id);
                sqlite3_exec(db, sql, print_callback, 0, 0);
                sprintf(sql, "SELECT typ, email_address FROM emails WHERE contact_id = %d;", id);
                sqlite3_exec(db, sql, print_callback, 0, 0);
                sqlite3_close(db);
            }
            break;
        }

        case 6: // List all contacts
            if (sqlite3_open("Contacts.db", &db) == SQLITE_OK) {
                sqlite3_exec(db, "SELECT id, first_name, last_name FROM contacts;", print_callback, 0, 0);
                sqlite3_close(db);
            }
            break;
    }
    return 0;
}

int execute_internal_sql(const char *sql)
{
    sqlite3 *db;
    char *err_msg = 0;
    if (sqlite3_open("Contacts.db", &db) != SQLITE_OK) return 1;
    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", 0, 0, 0);
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL Error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    sqlite3_close(db);
    return rc;
}

int print_callback(void *data, int argc, char **argv, char **azColName)
{
    (void)data;
    for (int i = 0; i < argc; i++) {
        (void)printf("%s: %s  ", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    (void)printf("\n");
    return 0;
}