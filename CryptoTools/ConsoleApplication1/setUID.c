#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

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
    char phone[50];
};

/*
Collects the executing uID as well as the effective uID
*/
void captureUInfo() {
    uID.rUser = getuid();
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
    char phash[50];
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
    setuid(uID.rUser);
    //printf("actual: %s\n",phash);
    //printf("user: %s\n",pword);
    //check users input against the
    if(strcmp(phash,pword) == 0) {
        return 0;
    } else {
        return -1;
    }
}

/*
allows the user to change the password provided that they know the current password
*/
int changePassword(const char *newPword) {
    //step 1: open password file and truncate
    //step 2: add new hash and close file.
   
    //step 1
    FILE *fp;
    setuid(uID.owner);
    fp = fopen("passwordFile.txt", "w+");
    if(fp == NULL) {
        perror("failed: ");
        return 1;
    }
    //step 2
    if(fputs(newPword,fp)>0) {
        fclose(fp);
        setuid(uID.rUser);
        return 0;
    } else {
        perror("Failed: ");
        setuid(uID.rUser);
        return -1;
    }
    
}

int addEmployee(struct eInfo *new) {
    setuid(uID.owner);
    FILE *fp;
    fp = fopen("directory.txt", "a");
    if(fp == NULL) {
        perror("failed: ");
        return 1;
    }
    fprintf(fp,"%s,%s,%s,%s,%s\n", new->lname, new->fname, new->pos, new->eID, new->phone);
    fclose(fp);
    setuid(uID.rUser);

    return 0;
}

/*
Deletes an employee based on eID
*/
int deleteEmployee(const char *eID) {
    //step 1 open the directory file
    //step 2 read file into array of eInof structs
    //loop over array if element i has an eID = to eID then begin copying over
    //step 3 open file in append mode and repopulate with the modified array
    //step 1
    setuid(uID.owner);
    FILE *fp;
    //fp = fopen("testDir.txt","r");;
    fp = fopen("directory.txt", "r");
    if(fp == NULL) {
        perror("failed: ");
        return 1;
    }
    //step 2
    char line[110];
    struct eInfo emp[500];
    char delmitier[2] = ",";
    int empIdx = 0;
    int meta = 0;
    int metaidx;
    //while loop processes the file a line at time
    while(fgets(line,110,fp) != NULL) {
        int size = strlen(line);
        line[size-2]='\0';
        char *token;
        char *rest = line;
        while((token = strtok_r(rest,delmitier,&rest))) {
            if(meta == 5) {
                empIdx++;
                //printf("emp idx is: %d\n",empIdx);
                meta = 0;
            }
            //printf("%s\n",token);
            if(meta == 0) {
                //emp[empIdx].lname = token;
                //printf("%s\n",token);
                int idx = 0;
                for(int i = 0; i < strlen(token); i ++) {
                   emp[empIdx].lname[i] = token[i]; 
                   idx = idx +1;
                }
                for(int i = idx; i < 50-idx-1;i++) {
                    emp[empIdx].lname[i] = '\0';
                }
            }
            if(meta == 1) {
                //printf("%s\n",token);
                //emp[empIdx].fname = token;
                int idx =0;
                for(int i = 0; i < strlen(token); i ++) {
                   emp[empIdx].fname[i] = token[i];
                   idx = idx +1; 
                }
                for(int i = idx; i < 50-idx;i++) {
                    emp[empIdx].fname[i] = '\0';
                }
            }
            if(meta == 2) {
                //printf("%s\n",token);
                //emp[empIdx].pos = token;
                int idx =0;
                for(int i = 0; i < strlen(token); i ++) {
                   emp[empIdx].pos[i] = token[i];
                   idx = idx +1; 
                }
                for(int i = idx; i < 50-idx;i++) {
                    emp[empIdx].pos[i] = '\0';
                }
            }
            if(meta == 3) {
                //printf("%s\n",token);
                //mp[empIdx].eID = token;
                int idx =0;
                for(int i = 0; i < strlen(token); i ++) {
                   emp[empIdx].eID[i] = token[i];
                   idx = idx+1; 
                }
                for(int i = idx; i < 50-idx;i++) {
                    emp[empIdx].eID[i] = '\0';
                }
                //printf("%s\n",emp[empIdx].eID);
            }
            if(meta == 4) {
                //printf("%s\n",token);
                //emp[empIdx].phone = token;
                int idx =0;
                for(int i = 0; i < strlen(token); i ++) {
                   emp[empIdx].phone[i] = token[i];
                   idx = idx+1; 
                }
                for(int i = idx; i < 50-idx;i++) {
                    emp[empIdx].phone[i] = '\0';
                }
            }
            if(meta < 5) {
                meta = meta+1;
                //printf("meta went up, meta now: %d\n",meta);
            }
            //token = strtok(0,delmitier);
        }
    }
    fclose(fp);

    //step 3
    int index = 0;
    int success =-1;
    for(int i = 0;i < empIdx+1; i ++ ) {
        int temp = strcmp(emp[i].eID,eID);
        //printf("%d\n",temp);
        //printf("eid = %s and %s\n", emp[i].eID, eID);
        if(strcmp(emp[i].eID,eID) == 0) {
            //printf("eid = %s and %s\n", emp[i].eID, eID);
            //emp[i] = emp[i+1];i
            success =0;
            index = i;
        }
    }
    if(success == -1) {
        return -1;
    }
    for(int i = index; i < empIdx + 1; i++) {
        emp[i] = emp[i+1];
    }
    empIdx = empIdx -1;
    //step 4
    //fp = fopen("testDir.txt","w+");
    fp = fopen("directory.txt", "w+");
    if(fp == NULL) {
        perror("failed: ");
        return 1;
    }
    for(int i = 0; i < empIdx+1; i ++) {
        fprintf(fp,"%s,%s,%s,%s,%s\n", emp[i].lname, emp[i].fname, emp[i].pos, emp[i].eID, emp[i].phone);
        //printf("round %d\n",i);
    }
    //printf("after print\n");
    setuid(uID.rUser);
    fclose(fp);
    return 0;
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
    //char eCount[500];
    char line[110];
    struct eInfo emp[500];
    char delmitier[2] = ",";
    int empIdx = 0;
    int meta = 0;
    int metaidx;
    //while loop processes the file a line at time
    while(fgets(line,110,fp) != NULL) {
        int size = strlen(line);
        line[size-2]='\0';
        char *token;
        char *rest = line;
        
        while((token = strtok_r(rest,delmitier,&rest))) {
            if(meta == 5) {
                empIdx++;
                //printf("emp idx is: %d\n",empIdx);
                meta = 0;
            }
            //printf("%s\n",token);
            if(meta == 0) {
                //emp[empIdx].lname = token;
                //printf("%s\n",token);
                int idx = 0;
                for(int i = 0; i < strlen(token); i ++) {
                   emp[empIdx].lname[i] = token[i]; 
                   idx = idx +1;
                }
                for(int i = idx; i < 50-idx;i++) {
                    emp[empIdx].lname[i] = '\0';
                }
            }
            if(meta == 1) {
                //printf("%s\n",token);
                //emp[empIdx].fname = token;
                int idx =0;
                for(int i = 0; i < strlen(token); i ++) {
                   emp[empIdx].fname[i] = token[i];
                   idx = idx +1; 
                }
                for(int i = idx; i < 50-idx;i++) {
                    emp[empIdx].fname[i] = '\0';
                }
            }
            if(meta == 2) {
                //printf("%s\n",token);
                //emp[empIdx].pos = token;
                int idx =0;
                for(int i = 0; i < strlen(token); i ++) {
                   emp[empIdx].pos[i] = token[i];
                   idx = idx +1; 
                }
                for(int i = idx; i < 50-idx;i++) {
                    emp[empIdx].pos[i] = '\0';
                }
            }
            if(meta == 3) {
                //printf("%s\n",token);
                //mp[empIdx].eID = token;
                int idx =0;
                for(int i = 0; i < strlen(token); i ++) {
                   emp[empIdx].eID[i] = token[i];
                   idx = idx+1; 
                }
                for(int i = idx; i < 50-idx;i++) {
                    emp[empIdx].eID[i] = '\0';
                }
                //printf("%s\n",emp[empIdx].eID);
            }
            if(meta == 4) {
                //printf("%s\n",token);
                //emp[empIdx].phone = token;
                int idx =0;
                for(int i = 0; i < strlen(token); i ++) {
                   emp[empIdx].phone[i] = token[i];
                   idx = idx+1; 
                }
                for(int i = idx; i < 50-idx;i++) {
                    emp[empIdx].phone[i] = '\0';
                }
            }
            if(meta < 5) {
                meta = meta+1;
                //printf("meta went up, meta now: %d\n",meta);
            }
            //token = strtok(0,delmitier);
        }
    }
    
    printf("-------------------------------------------\n");
    for(int i =0; i < empIdx+1; i++ ) {
        printf("First Name: %s\nLast Name: %s\nPosition: %s\nEmployee ID: %s\nEmployee Phone Number: %s\n",emp[i].fname,emp[i].lname,emp[i].pos,emp[i].eID,emp[i].phone);
        printf("-------------------------------------------\n");
    }
    //step 3
    fclose(fp);
    return 0;                                                                          
}