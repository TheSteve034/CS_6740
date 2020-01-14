#include <stdio.h>
#include <stdbool.h>
#include "CryptoTool.h"
#include "SimpleCryptoTool.h"

//clears stdin as a part of input validation
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
			if (buffer[0] != '1') {
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
	}
}

void mainMenuSelection() {
	printf("Press 1 for the simple crypto tool.\n");
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
}


void setupMainMenu() {
	printf("------------------------------------\n");
	printf("Welcome to the Crypto Tools Suite\n");
	mainMenuSelection();
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
