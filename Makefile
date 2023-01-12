# EXE does not mean ".exe" file extension, but the executable file
# @<command> means that the command will not be printed to the terminal
# $@ means the target name


# VARIABLES

# Compiler settings
CC := gcc
CFLAGS := -Wall -g -c

# Executable
EXE := bin/password-manager

# Main object & source file
MAIN_O := build/main.o
MAIN_C := src/main.c

# Main object & source file
PASSWORD_O := build/password.o
PASSWORD_C := lib/password.c

# Objects string
OBJS := $(PASSWORD_O) $(MAIN_O)


# TARGETS

# Build executable
$(EXE): $(OBJS)
	@mkdir -p bin
	$(CC) $(OBJS) -o $@

# Build main object
$(MAIN_O): $(MAIN_C)
	@mkdir -p build
	$(CC) $(CFLAGS) -c $(MAIN_C) -o $@

# Build main object
$(PASSWORD_O): $(PASSWORD_C)
	@mkdir -p build
	$(CC) $(CFLAGS) -c $(PASSWORD_C) -o $@

# Clean build
clean:
	rm -rf build bin
