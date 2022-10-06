CC := gcc
CFLAGS := -Wall

all: vault.o

vault.o: main.c src/password/password.c src/password/password.h
	$(CC) $(CFLAGS) main.c src/password/password.c src/password/password.h -o vault.o

clean:
	rm -rf vault.o
