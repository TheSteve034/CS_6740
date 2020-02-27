#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>
#include "sha1.h"

int checkSha1(const char *text) {
    //get hash from password file
    FILE *fp;
    fp = fopen("passwordFile.txt", "r");
    if(fp == NULL) {
        perror("failed: ");
        return 1;
    }
    char ch;
    unsigned char phash[1000];
    int i = 0;
    while((ch = fgetc(fp)) != EOF) {
        if(i < 1000) {
            if(ch == '\n') {
                continue;
            }
            phash[i] = ch;
            i++;
        }
    }
    fclose(fp);
    printf("real hash-%s-\n",phash);

    //generate has for user input
    unsigned char result1[SHA_DIGEST_LENGTH];
    SHA1(text, strlen(text), result1);
    int j;
    printf("user hash-");
    for(j = 0; j < 1; j++){
        printf("%x%c", result1[j], i < (SHA_DIGEST_LENGTH-1));
    }
    printf("-\n");
    //check is hashes are equal
    int valid = 0;
    for(int i = 0; i < 1; i++) {
        if(phash[i] != result1[i]) {
            printf("%c and %d not the same\n",phash[i],result1[i]);
            valid = 1;
        }
    }

    return valid;
}
