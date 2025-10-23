#include "hashing_functions.h"
/*
En este archivo se definen las funciones hash que podemos usar en el proyecto. Dejo dos las cuales son hash_xxh64 y hash_sha256 como para probar no mas.
*/
long hash_xxh64(const char *string_to_hash, long module, int seed){
    unsigned long hash= XXH64(string_to_hash, strlen(string_to_hash), seed);
    return hash%module;
}

long hash_sha256(const char *string_to_hash, long module){
    unsigned char char_hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)string_to_hash, strlen(string_to_hash), char_hash);
    long numeric_hash=0;

    for (int i=0; i<8; i++){
        numeric_hash=(numeric_hash<<8) | char_hash[i];
    }
    return numeric_hash%module;
}