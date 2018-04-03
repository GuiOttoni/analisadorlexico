#ifndef SCANNER_H
#define SCANNER_H

int isExAcceptableChar(char);

int isDelimiter(char);
int isOtherOperators(char);
int isStartRelationalOperator(char);

void printToken(Token *); // func prototype
void splitWords();
void printWords();
void printKeywords();
void printNumbers();
void printIdentifiers();
void printDelimiters();
void printOtherOperators();
void printRelationalOperators();
void printSummary();

#endif


