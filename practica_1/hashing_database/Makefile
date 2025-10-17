CC = gcc
LIBS = -lssl -lcrypto -lxxhash

all: hash_database find_record

hash_database: hash_database.c hashing_functions.c
	$(CC) hash_database.c hashing_functions.c -o hash_database $(LIBS)

find_record: find_record.c hashing_functions.c
	$(CC) find_record.c hashing_functions.c -o find_record $(LIBS)

clean:
	rm -f hash_database find_record
