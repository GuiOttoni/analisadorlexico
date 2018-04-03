#include <stdio.h>

#include "token.h"
#include "scanner.h"
#include "symdef.h"

/*--------------------Begin getTokenType()--------------------*/

TokenType getTokenType(FILE *filePtr) {
	int lineNum = 1;
	char ch;

	while ((ch = fgetc(filePtr)) != EOF) {
		if (ch == '\n') {
			lineNum++;
		}

		// Ignore comment starting with // to the end of line
		if (ch == '/') {
			if (fgetc(filePtr) == '/') {
				while ((ch = fgetc(filePtr)) != '\n') {}
				lineNum++;
			} else
				fseek(filePtr, -1, SEEK_CUR);
		}

		if (isalpha(ch)) {
			words[wordi][wordj++] = ch;
			while (isalpha(ch = fgetc(filePtr))) {
				words[wordi][wordj++] = ch;
			}
			words[wordi][wordj] = '\0';
			wordLineNums[wordi] = lineNum;
			wordi++; wordj = 0;
			fseek(filePtr, -1, SEEK_CUR);
		}

		else if (isdigit(ch)) {
			nums[numi][numj++] = ch;
			while (isdigit(ch = fgetc(filePtr))) {
				nums[numi][numj++] = ch;
			}
			nums[numi][numj] = '\0';
			numLineNums[numi] = lineNum;
			numi++; numj = 0;
			fseek(filePtr, -1, SEEK_CUR);
		}

		else if (ispunct(ch)) {
			if (isDelimiter(ch)) {
				delims[delimi] = ch;
				delimLineNums[delimi] = lineNum;
				delimi++;
			}
			else if (isOtherOperators(ch)) {
				otherOps[otherOpi] = ch;
				otherOpLineNums[otherOpi] = lineNum;
				otherOpi++;
			}
			else if (isStartRelationalOperator(ch)) {
				if (ch == '<' || ch == '>') {
					relOps[relOpi][relOpj++] = ch;
					relOps[relOpi][relOpj] = '\0';
					relOpLineNums[relOpi] = lineNum;
					relOpi++; relOpj = 0;
				}
				else if (ch == '=') {
					if ((ch = fgetc(filePtr)) == '=' || ch == '>' || ch == '<') {
						relOps[relOpi][relOpj++] = '=';
						relOps[relOpi][relOpj++] = ch;
						relOps[relOpi][relOpj] = '\0';
						relOpLineNums[relOpi] = lineNum;
						relOpi++; relOpj = 0;
					} else if (ch == '!') {
						if ((ch = fgetc(filePtr)) == '=') {
							relOps[relOpi][relOpj++] = '=';
							relOps[relOpi][relOpj++] = '!';
							relOps[relOpi][relOpj++] = ch;
							relOps[relOpi][relOpj] = '\0';
							relOpLineNums[relOpi] = lineNum;
							relOpi++; relOpj = 0;
						} else {
							fseek(filePtr, -1, SEEK_CUR);
						}
					} else {
						fseek(filePtr, -1, SEEK_CUR);
					}

				}
			}
		}
	} // end while

	splitWords();

	printSummary();

	FILE *fptr;
	fptr = fopen("symbol.txt","w");
	fprintf(fptr,"Token\t| Pattern\t\t| Lexeme\t|\n");
    fprintf(fptr,"KEY\t| Palavras-chave do sistema\t\t| int, if, printf, while e etc\t|\n");
    fprintf(fptr,"ID\t| Letra seguida por letras ou/e digitos\t\t| variaveis em geral\t|\n");
    fprintf(fptr,"NUM\t| Numeros\t\t| 0,25,36...\t|\n");
    fprintf(fptr,"DELIM\t| Unico caracter especial\t\t| (,),{,},[,] e etc\t|\n");
    fprintf(fptr,"OTH_OP\t| Caracter unico\t\t| +, -, * e etc\t|\n");
    fprintf(fptr,"REL_OP\t| Carecter seguido por outro carecter\t\t| ==, <=, >=, != e etc\t|\n");

	return EOT; // end of token
}

/*--------------------/End getTokenType()--------------------*/

void printSummary() {
    FILE *fptr;
    fptr = fopen("exp.txt","w");

	//fprintf(fptr,"Total %d KEYWORDS found are: \n", keyi);
	printLexemeString(fptr, keys, keyLineNums, keyi, "KEY"); //TODO: CHANGE LAST ARGUMENT

	//fprintf(fptr,"\nTotal %d IDENTIFIERS found are: \n", ideni);
    printLexemeString(fptr, idens, idenLineNums, ideni, "ID");

	//fprintf(fptr,"\nTotal %d NUMBERS found are: \n", numi);
	printLexemeString(fptr,nums,numLineNums, numi, "NUM");

	//fprintf(fptr,"\nTotal %d DELIMITERS found are: \n", delimi);
	printLexemeChar(fptr, delims, delimLineNums, delimi, "DELIM"); //CHAR

	//fprintf(fptr,"\nTotal %d OTHER OPERATORS found are: \n", relOpi);
	printLexemeChar(fptr, otherOps, otherOpLineNums, otherOpi, "OTH_OP"); //CHAR

	//fprintf(fptr,"\nTotal %d RELATIONAL OPERATORS found are: \n", relOpi);
	printLexemeString(fptr, relOps, relOpLineNums, relOpi, "REL_OP");
}

int isStartRelationalOperator(char c) {
	int i;
	int result = 0; // false
	if (c == '=' || c == '<' || c == '>') {
		result = 1;
	}
	return result;
}

int isOtherOperators(char c) {
	 int i;
	 int result = 0; // false
	 for (i = 0; i < 6; i++) {
		if (otherOperators[i] == c)
			result = 1;
	 }
	 return result;
}

int isDelimiter(char c) {
	 int i;
	 int result = 0; // false
	 for (i = 0; i < 9; i++) {
		if (delimiters[i] == c)
			result = 1;
	 }
	 return result;
}

int isKeyword(char *str) {
	int i;
	int result = 0; // false
	for (i = 0; i < 15; i++) {
		if (!strcasecmp(keywords[i], str))
			result = 1;
	}
	return result;
}

void printLexemeString(FILE *filePtr,char ar[LIMIT][MAX],int *_arr, int lexi, char *TypeOfToken){
	int i;
	for (i = 0; i < lexi; i++) {
		fprintf(filePtr,"<%s,%s> Line: %d \n", ar[i],TypeOfToken, _arr[i]);
	}
}

void printLexemeChar(FILE *filePtr,char *ar,int *_arr, int lexi, char *TypeOfToken){
	int i;
	for (i = 0; i < lexi; i++) {
		fprintf(filePtr,"<%c,%s> Line: %d \n", ar[i], TypeOfToken, _arr[i]);
	}
}



/* void printDelimiters(FILE *filePtr) {
	int i;
	for (i = 0; i < delimi; i++) {
		fprintf(filePtr,"%c \t (line #%d) \n", delims[i], delimLineNums[i]);
	}
}

void printRelationalOperators(FILE *filePtr) {
	int i;
	for (i = 0; i < relOpi; i++) {
		fprintf(filePtr,"%s \t (line #%d) \n", relOps[i], relOpLineNums[i]);
	}
}

void printWords(FILE *filePtr) {
	// printf("Num of words = %d \n", wordi);
	int i;
	for (i = 0; i < wordi; i++) {
		fprintf(filePtr,"%s \t (line #%d) \n", words[i], wordLineNums[i]);
	}
}

void printKeywords(FILE *filePtr) {
	int i;
	for (i = 0; i < keyi; i++) {
		fprintf(filePtr,"<%s,KEY> Line: %d \n", keys[i], keyLineNums[i]);
	}
}

void printNumbers(FILE *filePtr) {
	int i;
	for (i = 0; i < numi; i++) {
		fprintf(filePtr,"<%s \t (line #%d) \n", nums[i], numLineNums[i]);
	}
}

void printIdentifiers(FILE *filePtr) {
	int i;
	for (i = 0; i < ideni; i++) {
		fprintf(filePtr,"<%s \t (line #%d) \n", idens[i], idenLineNums[i]);
	}
} */

void splitWords() {
	int i;
	for (i = 0; i < wordi; i++) {
		if (isKeyword(words[i])) {
			strcpy(keys[keyi], words[i]);
			keyLineNums[keyi] =  wordLineNums[i];
			keyi++;
		} else {
			strcpy(idens[ideni], words[i]);
			idenLineNums[ideni] = wordLineNums[i];
			ideni++;

		}
	}
}

void printToken(Token *token) {
	char *tokenTypeStrings[] = {"Identifier", "Keyword", "Number", "Relation Operator",
		"Other Operator", "Delimiter"};

	int index = token->tokenType;
	if (index < 0 || index > 5)
		return;
	//char *tokenTypeStr = (char *)
		//malloc(1 + sizeof(char) * strlen(tokenTypeStrings[index]));

	//printf("index = %d \n", index);
	//printf("size: %d \n", strlen(tokenTypeStrings[index]));

	printf("Token type: %s |  Instance: %s | Line: %d \n",
		tokenTypeStrings[index], token->instance, token->lineNum);

}

// Besides English letters, and digits, these are extra acceptable characters
int isExAcceptableChar(char c) {
	if (c == '.' || c == '(' || c == ')' || c == ',' || c =='{' || c == '}' ||
		c == ';' || c == '[' || c == ']' ||
		c == ':' || c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||
		c == '=' || c == '<' || c == '>' || c == '!'
		/* || c == '#' */ ) {

		return 1;
	} else
		return 0;
}
