#ifndef _setUID_H
#define _setUID_H
void captureUInfo();
int validatePassword(const char *pword);
int changePassword(const char *curPword, const char *newPword);
int printDirectory();
#endif