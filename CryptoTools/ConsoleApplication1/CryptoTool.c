#include<stdio.h>

int main() {
	char buffer[50] = { 0 };
	int a = 10, b = 20, c = 0;
	c = a + b;
	snprintf(buffer, 50, "%d", c);
	printf("Here is some stuff in the buffer:\n");
	for (int i = 0; i < 50; i++) {
		if (buffer[i] != '\0') {
			printf("%c", buffer[i]);
		}
		else {
			break;
		}
	}
	return 0;
}
