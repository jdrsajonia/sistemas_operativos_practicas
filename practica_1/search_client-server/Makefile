# --- CONFIGURACIÓN GENERAL ---
CC = gcc
CFLAGS = -Wall -Iinclude
LIBS = -lssl -lcrypto -lxxhash

# --- RUTAS ---
SRC_DIR = src
HASH_DIR = hashing_database
INCLUDE_DIR = include

# --- BINARIOS ---
SERVER = server
CLIENT = cli

# --- FUENTES ---
SERVER_SRC = $(SRC_DIR)/server.c $(HASH_DIR)/find_record.c $(HASH_DIR)/hashing_functions.c
CLIENT_SRC = $(SRC_DIR)/client.c
HASH_SRC = $(HASH_DIR)/hash_database.c $(HASH_DIR)/hashing_functions.c

# --- REGLAS ---
all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_SRC)
	$(CC) $(CFLAGS) $(SERVER_SRC) -o $(SERVER) $(LIBS)

$(CLIENT): $(CLIENT_SRC)
	$(CC) $(CFLAGS) $(CLIENT_SRC) -o $(CLIENT)

hashdb: $(HASH_SRC)
	$(CC) $(CFLAGS) $(HASH_SRC) -o hashdb $(LIBS)

clean:
	rm -f $(SERVER) $(CLIENT) hashdb
