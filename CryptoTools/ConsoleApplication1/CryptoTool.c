#include<stdio.h>
#include <stdbool.h>
void clearStdIn() {
	int x = 0;
	while ((x = getchar()) != '\n' && x != EOF) {}
}
//this processes the useres input
void mainMenuSelection() {
	printf("Press 1 for the simple crypto tool.\n");
	//create a char array and set it to NULL
	char buffer[2] = { 0 };
	//only allow 1 char from the user.
	fgets(buffer, 2, stdin);
	//validate the users 
	bool isValid = false;
	while (isValid == false) {
		if (buffer[0] != '1') {
			printf("Invalid selection. Press 1 for the simple crypto tool\n");
			clearStdIn();
			fgets(buffer, 2, stdin);
		}
		else if (buffer[0] == '1') {
			isValid = true;
		}
	}
}
//This routine sets up the main main menu
void setupMainMenu() {
	printf("------------------------------------\n");
	printf("Welcome to the Crypto Tools Suite\n");
	mainMenuSelection();
	printf("------------------------------------\n");
}

int main() {
	setupMainMenu();
	return 0;
}
