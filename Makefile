# EXE does not mean ".exe" file extension, but the executable file
# @<command> means that the command will not be printed to the terminal
# $@ means the target name

# VARIABLES

# Compiler settings
CC := gcc
CFLAGS := -Wall -g -c -I include
LIBS := $(shell pkg-config --cflags --libs openssl) -lhpdf $(shell pkg-config --libs gtk4)

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

# Local account object & source file
LOCAL_ACCOUNT_O := build/local_account.o
LOCAL_ACCOUNT_C := lib/local_account.c

# Encryption & decryption object & source file
ENCDEC_O := build/encdec.o
ENCDEC_C := lib/encdec.c

# Export object & source file
EXPORT_O := build/export.o
EXPORT_C := lib/export.c

# GUI object & source file
GUI_O := build/gui.o
GUI_C := lib/gui.c

# Objects string
OBJS := $(LOCAL_ACCOUNT_O) $(CREDENTIALS_O) $(UI_O) $(PASSWORD_O) $(EXPORT_O) $(ENCDEC_O) $(GUI_O) $(MAIN_O)

# TARGETS

# Build executable
$(EXE): $(OBJS)
	@mkdir -p bin
	$(CC) $(OBJS) $(LIBS) -o $@

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

# Build credentials object
$(CREDENTIALS_O): $(CREDENTIALS_C)
	@mkdir -p build
	$(CC) $(CFLAGS) -c $(CREDENTIALS_C) -o $@

# Build local account object
$(LOCAL_ACCOUNT_O): $(LOCAL_ACCOUNT_C)
	@mkdir -p build
	$(CC) $(CFLAGS) -c $(LOCAL_ACCOUNT_C) -o $@

# Build encryption & decryption object
$(ENCDEC_O): $(ENCDEC_C)
	@mkdir -p build
	$(CC) $(CFLAGS) -c $(ENCDEC_C) -o $@

# Build export object
$(EXPORT_O): $(EXPORT_C)
	@mkdir -p build
	$(CC) $(CFLAGS) -c $(EXPORT_C) -o $@

# Build GUI object
$(GUI_O): $(GUI_C)
	@mkdir -p build
	$(CC) $(CFLAGS) $(shell pkg-config --cflags gtk4) -c $(GUI_C) -o $@

# Clean build
clean:
	rm -rf build bin
