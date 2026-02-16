# Kontaktverwaltung
A C-based contact management system using SQLite3

## Quickstart

[![downloads](https://img.shields.io/github/downloads/justforaprogramm/Kontaktverwaltung/total.svg?cacheSeconds=3600)](https://github.com/justforaprogramm/Kontaktverwaltung/releases)

### Compile EZ
To compile the project, Run:

```bash
git clone https://github.com/justforaprogramm/Kontaktverwaltung.git
sudo apt install libsqlite3-dev
make all


# Debug or delete tool
make debug

make clean

sudo rm -rf ../Kontaktverwaltung/

# if wanted for windows, install mingw and sqlite3 for mingw (to test in linux, use sudo apt install wine) 
x86_64-w64-mingw32-gcc -Wall -Wextra -O2 -Iheader -o Kontaktverwaltung.exe src/main.c src/sqlite.c src/sqlite3.c
```

### helptext

```
exit:           exits the program
help:           shows this help text
clear:          clears the console screen
add:            adds a new contact
edit:           edits an existing contact
remove:         removes a contact
getContacts:    list all (index & name)
getContact:     show all details for one index
```


## Notes

- security like encryption has to be added

- only in alpha, has some minor bugs

- only uses std libs (besides sqlite3.h)