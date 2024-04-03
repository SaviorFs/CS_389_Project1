#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

// Define token codes
#define INT_LIT 10
#define IDENT 11
#define PLUS_OP 21
#define MINUS_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define END_OF_FILE -1

// Define the maximum length of a lexeme
#define MAX_LEXEME_LENGTH 100

extern FILE *in_fp;  
extern char lexeme[MAX_LEXEME_LENGTH];  
extern int nextToken;  
extern char nextChar;  
extern int charClass;  
extern int currentToken;


void getChar();
void addChar();
void getNonBlank();
void lex();
void lookup(char ch);


int getNextToken(void) {
    lex(); 
    return nextToken;
}


void lex() {
    getNonBlank();

    switch (charClass) {
        case LETTER:
            memset(lexeme, 0, sizeof(lexeme));
            do {
                addChar();
                getChar();
            } while (charClass == LETTER || charClass == DIGIT);
            nextToken = IDENT;
            break;
        case DIGIT:
            memset(lexeme, 0, sizeof(lexeme));
            do {
                addChar();
                getChar();
            } while (charClass == DIGIT);
            nextToken = INT_LIT;
            break;
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;
        case EOF:
            nextToken = END_OF_FILE;
            strcpy(lexeme, "EOF");
            break;
    }

    printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
}

void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar)) charClass = LETTER;
        else if (isdigit(nextChar)) charClass = DIGIT;
        else charClass = UNKNOWN;
    } else charClass = EOF;
}

void addChar() {
    int len = strlen(lexeme);
    if (len + 1 < MAX_LEXEME_LENGTH) {
        lexeme[len] = nextChar;
        lexeme[len + 1] = '\0';
    } else {
        printf("Error - lexeme is too long\n");
    }
}

void lookup(char ch) {
    switch (ch) {
        case '+': addChar(); nextToken = PLUS_OP; break;
        case '-': addChar(); nextToken = MINUS_OP; break;
        case '*': addChar(); nextToken = MULT_OP; break;
        case '/': addChar(); nextToken = DIV_OP; break;
        case '(': addChar(); nextToken = LEFT_PAREN; break;
        case ')': addChar(); nextToken = RIGHT_PAREN; break;
        default: addChar(); nextToken = EOF; 
    }
}

void getNonBlank() {
    while (isspace(nextChar)) getChar();
}

void initLexer(char *filename) {
    in_fp = fopen(filename, "r");
    if (in_fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    getChar();
}
void closeLexer() {
    if (in_fp != NULL) {
        fclose(in_fp);
        in_fp = NULL;
    }
}
