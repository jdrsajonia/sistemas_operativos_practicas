#ifndef HASHING_FUNCTIONS_H
#define HASING_FUNCTIONS_H

#include <openssl/sha.h>
#include <xxhash.h>
#include <string.h>

long hash_xxh64(const char *string_to_hash, long module, int seed);
long hash_sha256(const char *string_to_hash, long module);


#endif