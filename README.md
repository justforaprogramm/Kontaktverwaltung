# Kontaktverwaltung
A C-based contact management system using SQLite3

## Quickstart

[![downloads](https://img.shields.io/github/downloads/lencx/ChatGPT/total.svg?style=flat-square)](https://github.com/justforaprogramm/Kontaktverwaltung/releases)

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

- only uses std libs (besides sqlite3.h)