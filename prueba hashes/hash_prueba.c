#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <openssl/sha.h> //instalar libssl-dev con sudo apt install libssl-dev | compilar con gcc open.c -o open -lssl -lcrypto
#include <xxhash.h> //instalar libxxhash-dev con sudo apt install libxxhash-dev | compilar con gcc open.c -o open -lxxhash


long hash_sha_index(const char *array, long num);
long hash_xxh64(const char *array, long num, int seed);

int main() {
    char text[100];
    printf("Escribe una cadena para ser hasheada: ");

    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0';  // borra el '\n'

    
    long index=hash_sha_index(text, 1000000);

    printf("\"%s\" (SHA256) --> %lu\n",text, index);

    index=hash_xxh64(text, 1000000, 0);
    printf("\"%s\" (XXH64) --> %lu\n",text, index);
    return 0;
}



long hash_xxh64(const char *string_to_hash, long module, int seed){
    unsigned long hash= XXH64(string_to_hash, strlen(string_to_hash), seed);
    return hash%module;
}

long hash_sha_index(const char *string_to_hash, long module){
    unsigned char char_hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)string_to_hash, strlen(string_to_hash), char_hash);
    long numeric_hash=0;

    for (int i=0; i<8; i++){
        numeric_hash=(numeric_hash<<8) | char_hash[i];
    }
    return numeric_hash%module;
}