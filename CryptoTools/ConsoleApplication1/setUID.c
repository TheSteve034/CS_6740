#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
struct userIds {
   uid_t rUser;
   uid_t owner;
} uID;

/*
Collects the executing uID as well as the effective uID
*/
void captureUInfo() {
    uID.owner = getuid();
    uID.owner = geteuid();
}