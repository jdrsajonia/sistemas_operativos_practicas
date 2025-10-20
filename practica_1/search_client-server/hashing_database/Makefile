CC = gcc
CFLAGS = -Wall -I../include
LIBS = -lssl -lcrypto -lxxhash

all: hashdb

hashdb: hash_database.c hashing_functions.c
	$(CC) $(CFLAGS) hash_database.c hashing_functions.c -o hashdb $(LIBS)

clean:
	rm -f hashdb
