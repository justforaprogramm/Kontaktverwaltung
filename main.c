/**
 * COPYRIGHT Benjamin Schmid <inf25189@lehre.dhbw-stuttgart.de>
 */

#include "main.h"

/**
 * manages input
 */
int main(int argc, char *argv[])
{
    // Initializes db-file and creates tables if they don't exist
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
    else if(!strcmp("getContacts", in)) (void)mySqlite(6);
    else if(!strcmp("getContact", in)) (void)mySqlite(5);
    else if(!strcmp("add", in)) (void)mySqlite(2);
    else if(!strcmp("edit", in)) (void)mySqlite(3);
    else if(!strcmp("remove", in)) (void)mySqlite(4);
    else if (!strcmp("clear", in)) {
        int unused; 
        #ifdef _WIN32
            unused = system("cls");
        #else
            unused = system("clear");
        #endif
        (void)unused;
}
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
    const char functionText[] = 
    "\nexit:\t\texits the program"
    "\nhelp:\t\tshows this help text"
    "\nclear:\t\tclears the console screen"
    "\nadd:\t\tadds a new contact"
    "\nedit:\t\tedits an existing contact"
    "\nremove:\t\tremoves a contact"
    "\ngetContacts:\tlist all (index & name)"
    "\ngetContact:\tshow all details for one index\n";
    
    (void)printf("%s", functionText);

    #ifdef DEBUG
        (void)printf("DEBUG: added help\n");
    #endif //DEBUG
    for(int i=0;i<HELP_LINE;i++) (void)printf("-");
    (void)printf("\n");
    return 0;
}
