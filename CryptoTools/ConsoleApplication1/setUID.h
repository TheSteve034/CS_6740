#ifndef _setUID_H
#define _setUID_H
struct eInfo {
    char fname[50];
    char lname[50];
    char pos[50];
    char eID[50];
    char phone[10];
};

void captureUInfo();
int validatePassword(const char *pword);
int changePassword(const char *newPword);
int printDirectory();
//TODO:
int addEmployee(struct eInfo *new);
//int modifyEmployee(struct eInfo *new);
void deleteEmployee(const char *eID);
#endif