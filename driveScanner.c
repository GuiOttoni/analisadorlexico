#include <stdio.h>
#include <ctype.h>

#include "token.h"
#include "scanner.h"

int main(int argc, char *argv[])
{
	FILE *filePtr;
    filePtr = fopen("file.txt","r");
	fseek(filePtr, 0, SEEK_END);
	if (ftell(filePtr) == 0) {
		printf("File is empty.\n");
		exit(1);
	} else {
		rewind(filePtr);
	}

	char c;
	int numLine = 1;

	int charCount = 0;
	char tempStr[MAX]; // ! Remember: C doesn't do array out-of-bound checking!
	int i;

	int isValid = 1; // true
	while ((c = fgetc(filePtr)) != EOF) {

		if (c == '\n')
			numLine++;

		if (c == '/') {
			if (fgetc(filePtr) == '/') {
				while ((c = fgetc(filePtr)) != '\n') {}
				numLine++;
			}
		}

		if (isalnum(c)) {
			tempStr[charCount] = c; // array out-of-bound checking not happen here
			charCount++;
			if (charCount > MAX) {
				printf("Word '%s' on line number %d is too long. \n", tempStr, numLine);
				exit(1);
			}
		} else if (isspace(c) || isExAcceptableChar(c)) {
			charCount = 0;
		} else {
			printf("Invalid character '%c' at line %d. \n", c, numLine);
			isValid = 0; // false
		}

	}

	if (isValid == 0) {
		printf("Something wrong with the input file. \n");
		exit(1);
	}

	rewind(filePtr);

	/*---------Begin Job-------------*/
	TokenType tokenType = UNDEF;
	while ((tokenType = getTokenType(filePtr)) != EOT) {}
	/*---------/End Job-------------*/

	fclose(filePtr);
	return 0;
}

