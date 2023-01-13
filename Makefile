# EXE does not mean ".exe" file extension, but the executable file
# @<command> means that the command will not be printed to the terminal
# $@ means the target name


# VARIABLES

# Compiler settings
CC := gcc
CFLAGS := -Wall -g -c -I include

# Executable
EXE := bin/password-manager

# Main object & source file
MAIN_O := build/main.o
MAIN_C := src/main.c

# Password object & source file
PASSWORD_O := build/password.o
PASSWORD_C := lib/password.c

# User interface object & source file
UI_O := build/ui.o
UI_C := lib/ui.c

# Credentials object & source file
CREDENTIALS_O := build/credentials.o
CREDENTIALS_C := lib/credentials.c

# Objects string
OBJS := $(CREDENTIALS_O) $(UI_O) $(PASSWORD_O) $(MAIN_O)


# TARGETS

# Build executable
$(EXE): $(OBJS)
	@mkdir -p bin
	$(CC) $(OBJS) -o $@

# Build main object
$(MAIN_O): $(MAIN_C)
	@mkdir -p build
	$(CC) $(CFLAGS) -c $(MAIN_C) -o $@

# Build password object
$(PASSWORD_O): $(PASSWORD_C)
	@mkdir -p build
	$(CC) $(CFLAGS) -c $(PASSWORD_C) -o $@

# Build user interface object
$(UI_O): $(UI_C)
	@mkdir -p build
	$(CC) $(CFLAGS) -c $(UI_C) -o $@

# Build user interface object
$(CREDENTIALS_O): $(CREDENTIALS_C)
	@mkdir -p build
	$(CC) $(CFLAGS) -c $(CREDENTIALS_C) -o $@

# Clean build
clean:
	rm -rf build bin
