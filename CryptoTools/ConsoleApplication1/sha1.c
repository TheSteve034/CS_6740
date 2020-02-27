#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>
#include "sha1.h"

unsigned char getSha1(const char *text) {
    int i;
    unsigned char res[SHA_DIGEST_LENGTH];

    SHA1(text, strlen(text), res);

    return res;
}
