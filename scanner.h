#ifndef SCANNER_H
#define SCANNER_H

int isExAcceptableChar(char);

int isDelimiter(char);
int isOtherOperators(char);
int isStartRelationalOperator(char);
int isRepeated(char);

void printToken(Token *); // func prototype
void splitWords();
void printLexemeString();
void printLexemeChar();
void printSummary();


#endif


