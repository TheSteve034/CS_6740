#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "sha1.h"
//holds the User and owning user information
struct userIds {
   uid_t rUser;
   uid_t owner;
   gid_t rootGroup;
} uID;

struct eInfo {
    char fname[50];
    char lname[50];
    char pos[50];
    char eID[50];
    char phone[10];
};

/*
Collects the executing uID as well as the effective uID
*/
void captureUInfo() {
    uID.owner = getuid();
    uID.owner = geteuid();
    uID.rootGroup = 0;
}

/*
accepts input from the user and checks to see if it matches the password in the
password file.
*/
int validatePassword(const char *pword) {
    //step 1:read the password file and get the current password hash
    //step 2:use the plain text to create a hash 
    //step 3:check the hashes if they match return 0 if not return -1;

    //step 1
    //the password file is only readable as root to read from it we need
    //to raise the the UID to root
    setuid(uID.owner);
    //read the file.
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
            phash[i] = ch;
            i++;
        }
    }
    //downgrade uID and close the file since it keeps root permission until closed
    fclose(fp);
    setuid(uID.rUser);
    //step 2
    //get a hash of the plain text provided.
    unsigned char uhash = getSha1(pword);
    //step 3
    if(uhash == phash) {
        return 0;
    } else {
        return -1;
    }
}

/*
allows the user to change the password provided that they know the current password
*/
int changePassword(const char *curPword, const char *newPword) {
    //step 1: validate the current password
    //step 2: is user knows the curent password create a new has for the new password
    //step 3: open password file and truncate
    //step 4: add new hash and close file.

    //step 1
    if(validatePassword(curPword)) {
        //step 2
        unsigned char newHash = SHA1(newPword);
        //step 3
        FILE *fp;
        setuid(uID.owner);
        fp = fopen("passwordFile.txt", "w+");
        if(fp == NULL) {
            perror("failed: ");
            return 1;
        }
        setuid(uID.rUser);
        //step 4
        if(fputs(newHash,fp)>0) {
            fclose(fp);
        } else {
            perror("Failed: ");
        }
    } else {
        return -1;
    }
}

/*
Prints the employee directory
*/
int printDirectory() {
    //step 1: Open file
    //step 2: Print employees
    //step 3: Close file
    
    //step 1
    setuid(uID.owner);
    FILE *fp;
    fp = fopen("directory.txt", "r");
    if(fp == NULL) {
        perror("failed: ");
        return 1;
    }
    printf("directory is open\n");
    //step 2
    //int count =0;
    //int meta =0;
    char ch;
    int i = 0;
    //read the first line to determine how many employees are in the directory
    char eCount[500];
    char line[110];
    char *token = NULL;
    fgets(eCount,500,fp);
    printf("first 5000 chars of file: %s",eCount);
    //create and array of eInfo structs the size of eCount
    struct eInfo emp[500];
    char delmitier[2] = ",";
    int empIdx = 0;
    int meta = 0;
    //while loop processes the file a line at time
    while(fgets(line,110,fp) != NULL) {
        int size = strlen(line);
        line[size-2]='\0';
        token = strtok(line,delmitier);
        while(token != NULL) {
            if(meta == 5) {
                empIdx++;
                printf("emp idx is: %d\n",empIdx);
                meta = 0;
            }
            printf("%s\n",token);
            if(meta == 0) {
                for(int i = 0; i < strlen(token); i ++) {
                   emp[empIdx].fname[i] = token[i]; 
                }
            }
            if(meta < 5) {
                meta = meta+1;
                printf("meta went up, meta now: %d\n",meta);
            }
            token = strtok(NULL,delmitier);
        }
    }

    for(int i =0; i < atoi(eCount); i++ ) {
        printf("Name: %s\n",emp[i].fname);
    }
    //step 3
    fclose(fp);                                                                          
}