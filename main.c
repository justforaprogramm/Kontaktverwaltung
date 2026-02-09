/**
 * COPYRIGHT Benjamin Schmid <inf25189@lehre.dhbw-stuttgart.de>
 */

#include "main.h"

/**
 * manages input
 */
int main(int argc, char *argv[])
{
    // Initialisiert die Tabellen beim Start
    mySqlite(1); 

    if(argc > 1)
    {
        #ifdef DEBUG
            (void)printf("DEBUG: argument given: %s\n", argv[1]);
        #endif //DEBUG

        if(!giveAnswer(argv[1]))
        {
            #ifdef DEBUG
            (void)printf("DEBUG: answer was completed correctly!\n");
            #endif //DEBUG
        }
        return 0;
    }
    
    char input[INPUT_BUFFER];
    (void)printf("Hello from Contactmanager!\n");
    #ifdef DEBUG
        (void)printf("DEBUG: Debug print enabled!\n");
    #endif //DEBUG

    while(true)
    {
        (void)printf("choose Action: ");
        if(scanf("%49s", input) != 1)
        {
            #ifdef DEBUG
                (void)printf("DEBUG: scan failed!\n");
            #endif //DEBUG
            return 0;
        }

        #ifdef DEBUG
            (void)printf("DEBUG: argument given: %s\n", input);
        #endif //DEBUG

        if(!giveAnswer(input))
        {
            #ifdef DEBUG
            (void)printf("DEBUG: answer was completed correctly!\n");
            #endif //DEBUG
        }
        else return 0;
    }
}

/**
 * calls functions for given input
 */
int giveAnswer(char in[INPUT_BUFFER])
{
    bool exit = false;
    if(!strcmp("exit", in)) exit = true;
    else if(!strcmp("help", in)) (void)help();
    else if(!strcmp("getContacts", in)) (void)getContacts();
    else if(!strcmp("getContact", in)) (void)getContact();
    else if(!strcmp("add", in)) (void)addContact();
    else if(!strcmp("edit", in)) (void)editContact();
    else if(!strcmp("remove", in)) (void)removeContact();
    else (void)printf("type help for help!\n");
    return exit ? 1 : 0;
}

/**
 * displays help in terminal
 */
int help(void)
{
    (void)printf("help text:\n");
    for(int i=0;i<HELP_LINE;i++) (void)printf("-"); 
    (void)printf("\nexit:\texits the programm\nhelp:\tshows this help text\n"
                 "add:\tadds a new contact\ngetContacts:\tlist all (index & name)\n"
                 "getContact:\tshow all details for one index\n");

    #ifdef DEBUG
        (void)printf("DEBUG: added help\n");
    #endif //DEBUG
    for(int i=0;i<HELP_LINE;i++) (void)printf("-");
    (void)printf("\n");
    return 0;
}

/**
 * adds a kontakt with info
 */
int addContact(void)
{
    return mySqlite(2); // Case 2 in sqlite.c übernimmt das interaktive Hinzufügen
}

/**
 * gets all infos about a contact
 */
int getContact(void)
{
    return mySqlite(5); // Case 5: Detailansicht
}

/**
 * gets name and index of all contacts
 */
int getContacts(void)
{
    return mySqlite(6); // Case 6: Kurze Liste
}

// Dummy Implementierungen für den Rest
int editContact(void) { (void)printf("Feature noch nicht implementiert.\n"); return 0; }
int removeContact(void) { (void)printf("Feature noch nicht implementiert.\n"); return 0; }