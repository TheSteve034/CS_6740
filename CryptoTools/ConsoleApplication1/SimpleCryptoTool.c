#include<stdio.h>
#include "SimpleCryptoTool.h"
#include <ctype.h>

char key[1];
char alphabet[26] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
char ealphabet[26] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };
char plainText[50];
char encrypted[50];
char decrypted[50];

void getKey() {
	printf("Enter a one character key:\n");
	gets(key);
}

void getInput() {
	printf("Enter a string to be encrypted:\n");
	gets(plainText);
	convertInputToLower();
	subAlphabet();

}

void subAlphabet() {
	//ealphabet[0] = key[0];
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

void convertInputToLower() {
	for (int i = 0; i < 50; i++) {
		plainText[i] = tolower(plainText[i]);
	}
}