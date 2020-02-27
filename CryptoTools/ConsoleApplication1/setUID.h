#ifndef _setUID_H
#define _setUID_H
struct eInfo {
    int active;
    char fname[50];
    char lname[50];
    char pos[50];
    char eID[50];
    char phone[10];
};

void captureUInfo();
int validatePassword(const char *pword);
int changePassword(const char *curPword, const char *newPword);
int printDirectory();
#endif