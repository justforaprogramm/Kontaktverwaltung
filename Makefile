# COPYRIGHT Benjamin Schmid <inf25189@lehre.dhbw-stuttgart.de>
# basic layout made by chatgpt <https://chatgpt.com/share/69398acd-3388-8008-8900-ddb0ca42372f>

# Compiler
CC = gcc
# Compiler flags
CFLAGS = -Wall -Wextra -O2
# Target executable
TARGET = Kontaktverwaltung
# Source file
SRC = main.c sqlite.c
# libs
libs = -lsqlite3

# main
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(libs)

# Clean rule
clean:
	rm -f $(TARGET)
