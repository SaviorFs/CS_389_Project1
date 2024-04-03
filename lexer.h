#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

#define INT_LIT 10
#define IDENT 11
#define PLUS_OP 21
#define MINUS_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define END_OF_FILE -1

#define MAX_LEXEME_LENGTH 100

extern FILE *in_fp;
extern char lexeme[MAX_LEXEME_LENGTH];
extern int nextToken;
extern char nextChar;
extern int charClass;
extern int currentToken;

int getNextToken(void);
void getChar(void);
void addChar(void);
void getNonBlank(void);
void lex(void); 
void lookup(char ch);
void initLexer(char *filename);
void closeLexer(void);

#endif
