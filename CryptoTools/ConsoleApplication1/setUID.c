#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
//#include "sha1.h"
//holds the User and owning user information
struct userIds {
   uid_t rUser;
   uid_t owner;
   gid_t rootGroup;
} uID;

struct eInfo {
    int active;
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
    //downgrade uID
    return 0;
    
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
    //read the first line to determine how many employees are in the directory
    char eCount[500];
    char line[110];
    char *token = NULL;
    fgets(eCount,500,fp);
    int emps = atoi(eCount);
    printf("Employee Count: %d\n",emps);
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
                //printf("emp idx is: %d\n",empIdx);
                meta = 0;
            }
            //printf("%s\n",token);
            if(meta == 0) {
                for(int i = 0; i < strlen(token); i ++) {
                   emp[empIdx].lname[i] = token[i]; 
                }
            }
            if(meta == 1) {
                for(int i = 0; i < strlen(token); i ++) {
                   emp[empIdx].fname[i] = token[i]; 
                }
            }
            if(meta == 2) {
                for(int i = 0; i < strlen(token); i ++) {
                   emp[empIdx].pos[i] = token[i]; 
                }
            }
            if(meta == 3) {
                for(int i = 0; i < strlen(token); i ++) {
                   emp[empIdx].eID[i] = token[i]; 
                }
            }
            if(meta == 4) {
                for(int i = 0; i < strlen(token); i ++) {
                   emp[empIdx].phone[i] = token[i]; 
                }
            }
            if(meta < 5) {
                meta = meta+1;
                //printf("meta went up, meta now: %d\n",meta);
            }
            token = strtok(NULL,delmitier);
        }
    }
    
    printf("-------------------------------------------\n");
    for(int i =0; i < atoi(eCount); i++ ) {
        emp[i].active =1;
        printf("First Name: %s\nLast Name: %s\nPosition: %s\nEmployee ID: %s\nEmployee Phone Number: %s\n",emp[i].fname,emp[i].lname,emp[i].pos,emp[i].eID,emp[i].phone);
        printf("-------------------------------------------\n");
    }
    //step 3
    fclose(fp);
    return 0;                                                                          
}