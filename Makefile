# COPYRIGHT Benjamin Schmid <inf25189@lehre.dhbw-stuttgart.de>

# Compiler
CC = x86_64-w64-mingw32-gcc

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
SRC = src/main.c src/sqlite.c

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