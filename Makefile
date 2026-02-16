# COPYRIGHT Benjamin Schmid <inf25189@lehre.dhbw-stuttgart.de>
# basic layout made by chatgpt <https://chatgpt.com/share/69398acd-3388-8008-8900-ddb0ca42372f>

# Compiler
CC = gcc

# Base compiler flags
CFLAGS = -Wall -Wextra

# Release flags
RELEASE_FLAGS = -O2

# Debug flags
DEBUG_FLAGS = -g -O0 -DDEBUG

# Target executables
TARGET = Kontaktverwaltung
DEBUG_TARGET = Kontaktverwaltung_debug

# Source files
SRC = main.c sqlite.c

# Libraries
LIBS = -lsqlite3

# Default target (release)
all: $(TARGET)

# Release build
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -o $(TARGET) $(SRC) $(LIBS)

# Debug build
debug: $(DEBUG_TARGET)

$(DEBUG_TARGET): $(SRC)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o $(DEBUG_TARGET) $(SRC) $(LIBS)

# Clean rule
clean:
	rm -f $(TARGET) $(DEBUG_TARGET)