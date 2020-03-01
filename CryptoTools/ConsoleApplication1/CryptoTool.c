#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "CryptoTool.h"
#include "SimpleCryptoTool.h"
#include "setUID.h"

//clears stdin by consuming any uncaught chars.
void clearStdIn() {
	int x = 0;
	while ((x = getchar()) != '\n' && x != EOF) {}
}

//validates the user selection.
void validateSelection(char buffer[2], int menuType) {
	bool isValid = false;
	int invaildAttempts = 0;
	while (isValid == false) {
		if (invaildAttempts > 5) {
			printf("To many invalid slections, program will exit.\n");
			return;
		}
		if (menuType == 1) {
			//menuType 1 refers to the main menu
			if (buffer[0] != '1'&& buffer[0] != '2'&& buffer[0] != '3') {
				printf("Invalid selection. Press 1 for the simple crypto tool\n");
				invaildAttempts++;
				clearStdIn();
				fgets(buffer, 2, stdin);
			}
			else {
				isValid = true;
			}
		}
		if (menuType == 2) {
			//menuType 2 refers to the simple crypto tool
			if (buffer[0] != '1' && buffer[0] != '2' && buffer[0] != '3') {
				printf("Invalid selection.\n");
				invaildAttempts++;
				clearStdIn();
				fgets(buffer, 2, stdin);
			}
			else {
				isValid = true;
			}
		}
		if (menuType == 3) {
			//menuType 2 refers to theemployee directory
			if (buffer[0] != '1' && buffer[0] != '2' && buffer[0] != '3' && buffer[0] != '4') {
				printf("Invalid selection.\n");
				invaildAttempts++;
				clearStdIn();
				fgets(buffer, 2, stdin);
			}
			else {
				isValid = true;
			}
		}
		
	}
}

void mainMenuSelection() {
	printf("Press 1 for the simple crypto tool.\n");
	printf("Press 2 for employee directory\n");
	printf("Press 3 to exit the appication.\n");
	//create a char array and set it to NULL
	char buffer[2] = { 0 };
	//only allow 1 char from the user.
	fgets(buffer, 2, stdin);
	//validate the user input
	validateSelection(buffer,1);
	//launch the tool selected
	if (buffer[0] == '1') {
		//option 1 launches the simple crypto tool
		simpleCryptoMenu();
	}
	if(buffer[0] == '2') {
		setUIDMenu();
	}
	if (buffer[0] == '3') {
		return;
	}
}


void setupMainMenu() {
	printf("------------------------------------\n");
	printf("Welcome to the Crypto Tools Suite\n");
	mainMenuSelection();
}

void setUIDMenu() {
	printf("------------------------------------\n");
	printf("Welcome to the setUID tool.\n");
	printf("Select the desiered opertaion from the menu bellow.\n");
	printf("1. View Directory\n2. Add Employee\n3. Change Password\n4. Main Menu\n");

	//create a char array and set it to NULL
	char buffer[2] = { 0 };
	clearStdIn();
	//only allow 1 char from the user.
	fgets(buffer, 2, stdin);
	validateSelection(buffer, 3);
	if(buffer[0] == '1') {
		printDirectory();
		setUIDMenu();
	}
	if(buffer[0] == '2') {
		struct eInfo newEmployee;
		clearStdIn();
		printf("Enter first Name\n");
		gets(newEmployee.fname,50,stdin);
		printf("Enter Last Name\n");
		gets(newEmployee.lname);
		printf("Enter Position\n");
		gets(newEmployee.pos);
		printf("Enter ID\n");
		gets(newEmployee.eID);
		printf("Enter phone\n");
		gets(newEmployee.phone);
		printf("%s %s %s %s %s\n",newEmployee.fname,newEmployee.lname,newEmployee.pos,newEmployee.eID,newEmployee.phone);

		//call addEmp
		if(addEmployee(&newEmployee) == 0) {
			printf("Employee added.\n");
			setupMainMenu();
		} else {
			printf("failed to add employee.\n");
			setupMainMenu();
		}
		
	}
	if (buffer[0] == '3') {
		//return to the main menu
		clearStdIn();
		printf("To reset password please provide the current password:\n");
		unsigned char currPword[50];
		gets(currPword);
		//check password
		if(validatePassword(currPword) == 0) {
			//password is valid
			printf("passord is valid.\n");
			printf("Please enter new password. Password length not exceed 50 characters.\n");
			const char *newPword[50];
			fgets(newPword,55,stdin);
			char *p;
			if(p=strchr(newPword,'\n')) {
				*p = 0;
			} else {
				scanf("%*[^\n]%*c");
			}
			if(changePassword(newPword) == 0) {
				printf("Password was changed\n");
				setupMainMenu();
			} else {
				printf("Password was not changed.\n");
				setupMainMenu();
			}
		} else {
			printf("Invalid password.\n");
			setupMainMenu();
		}
	}
	if (buffer[0] == '4') {
		//return to the main menu
		clearStdIn();
		setupMainMenu();
	}
}

void simpleCryptoMenu() {
	printf("------------------------------------\n");
	printf("Welcome to the Simple Crypto Tool.\n");
	printf("Select the desiered operation from the menu bellow.\n");
	printf("1. Encrypt\n2. Decrypt\n3. Main Menu\n");

	//create a char array and set it to NULL
	char buffer[2] = { 0 };
	clearStdIn();
	//only allow 1 char from the user.
	fgets(buffer, 2, stdin);
	validateSelection(buffer, 2);
	//call the routine for the selected menu option.
	if (buffer[0] == '1') {
		//here we will handle encrypting plain text
		clearStdIn();
		getKey();
		getInput();
		encrypt();
		setupMainMenu();
	}
	if (buffer[0] == '2') {
		//her we will handle decrypting a string based on a key
		clearStdIn();
		getKey();
		getDecryptInput();
		decrypt();
		setupMainMenu();
	}
	if (buffer[0] == '3') {
		//return to the main menu
		clearStdIn();
		setupMainMenu();
	}
}

int main() {
	setupMainMenu();
	return 0;
}
