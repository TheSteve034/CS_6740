#include<stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "SimpleCryptoTool.h"

char key[1];
char alphabet[26] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
char ealphabet[26] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
char plainText[100];
char encrypted[50];
char decrypted[50];
FILE* inFile;
FILE* outfile;

//this method encrypts the text provided by the user.
void encrypt() {
	char c;
	outfile = fopen("encryptedFile.txt", "w");
	while (1) {
		c = fgetc(inFile);
		c = tolower(c);
		if (c == EOF) {
			break;
		}
		if (isalpha(c) == 0) {
			fputc(c, outfile);
		}
		else {
			int asciiVal = c - 97;
			fputc(ealphabet[asciiVal],outfile);
			
		}
	}
	fclose(outfile);
	fclose(inFile);
	printf("You provided \'%s\', it was encrypted to encryptedFile.txt.\n", plainText);
}

//this method decrypts a file provided by the user based on a ket provided by the user.
void decrypt() {
	outfile = fopen("decryptedFile.txt", "w");
	char c;
	while (1) {
		//will bo converting all characters in the file to a lower case.
		c = fgetc(inFile);
		c = tolower(c);
		if (c == EOF) {
			break;
		}
		if (isalpha(c) == 0) {
			fputc(c, outfile);
		}
		else {
			for (int i = 0; i < 26; i++) {
				if (ealphabet[i] == c) {
					fputc(i + 97, outfile);
					break;
				}
			}
		}
	}
	fclose(outfile);
	fclose(inFile);
	printf("You provided \'%s\', it was decrypted to decryptedFile.txt.\n", encrypted);
}

void getKey() {
	printf("Enter a one character key:\n");
	gets(key);
}

//this method gether the string the user wished to encrypt
void getInput() {
	printf("Enter a file name to be encrypted. Please provide the fulle path for the file. The file must only contain characters that are letters of the alphabet:\n");
	gets(plainText);
	//open the file
	if ((inFile = fopen(plainText, "r+")) == '\0') {
		printf("cannot open the file");
		exit(1);
	}
	subAlphabet();
}

//Asks the user to provided a file to be decrypted.
void getDecryptInput() {
	printf("Enter a File to be decrypted. Please provide the full path for the file. The file must only contain characters that are letters of the alphabet:\n");
	gets(encrypted);
	if ((inFile = fopen(encrypted, "r+")) == '\0') {
		printf("cannot open the file");
		exit(1);
	}
	subAlphabet();
}

//this method will created the encrypted alphabet based on the key provided by the user
void subAlphabet() {
	int asciiVal = key[0];
	int index = 0;
	for (int i = 0; i < 26; i++) {
		if (asciiVal == 122 && index == 0) {
			//this the case where the key is z
			ealphabet[i] = key[0];
			asciiVal = 97;
			for (int j = 1; j < 26; j++) {
				ealphabet[j] = asciiVal + (j-1);
			}
			break;
		}
		if (asciiVal < 122) {
			//this is the case that the letter a - y
			ealphabet[i] = key[0] + i;
			asciiVal = key[0] + i;
			index++;
		}
	}
	if (index > 0) {
		asciiVal = 97;
		int modifier = 0;
		for (int i = index; i < 26; i++) {
			ealphabet[i] = asciiVal + modifier;
			modifier++;
		}
	}
}