#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 

// Definitions for tokens corresponding to elements in a simplified C language syntax
// The lexical analyzer is designed to recognize basic arithmetic operations, variable names, and numeric values
// parentheses for grouping, and braces for block structuring, following a simplified BNF grammar.

#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

// Token definitions for recognized elements
#define INT_LIT 10
#define IDENT 11
#define PLUS_OP 21
#define MINUS_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define END_OF_FILE -1
#define LEFT_BRACE 27
#define RIGHT_BRACE 28

// Preprocessor tokens
#define PREPROCESSOR 30 
#define MAX_LEXEME_LENGTH 100

// Global vars
char lexeme[MAX_LEXEME_LENGTH];
int nextToken;
FILE *in_fp = NULL;
char nextChar;
int charClass;

// Function prototypes
void initLexer(char *filename);
int getNextToken(void);
void closeLexer(void);
void getChar(void);
void addChar(void);
void getNonBlank(void);
void lex(void);
void lookup(char ch);

void expression(void);
void term(void);
void factor(void);
void match(int expectedToken);
void error(void);

// Inits the lexer with the input file
void initLexer(char *filename) {
    in_fp = fopen(filename, "r");
    if (in_fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    getChar();
    getNextToken();
}

// Retrieves the next token from the input stream
int getNextToken(void) {
    lex();
    return nextToken;
}

// Closes the lexer by closing the input file
void closeLexer(void) {
    if (in_fp != NULL) {
        fclose(in_fp);
        in_fp = NULL;
    }
}

// Reads the next character from the input file and determines its class
void getChar() {
    nextChar = getc(in_fp);
    if (nextChar == EOF) {
        charClass = EOF;
    } else if (isalpha(nextChar)) {
        charClass = LETTER;
    } else if (isdigit(nextChar)) {
        charClass = DIGIT;
    } else {
        charClass = UNKNOWN; 
    }
}

// Adds the current character to the lexeme buffer
void addChar() {
    int len = strlen(lexeme);
    if (len + 1 < MAX_LEXEME_LENGTH) {
        lexeme[len] = nextChar;
        lexeme[len + 1] = '\0';
    } else {
        printf("Error - lexeme is too long\n");
    }
}

// Token is based on the character and adds it to the lexeme
void lookup(char ch) {
    switch (ch) {
        case '+': addChar(); nextToken = PLUS_OP; break;
        case '-': addChar(); nextToken = MINUS_OP; break;
        case '*': addChar(); nextToken = MULT_OP; break;
        case '/': addChar(); nextToken = DIV_OP; break;
        case '(': addChar(); nextToken = LEFT_PAREN; break;
        case ')': addChar(); nextToken = RIGHT_PAREN; break;
        case '{': addChar(); nextToken = LEFT_BRACE; break;
        case '}': addChar(); nextToken = RIGHT_BRACE; break;
        default: addChar(); nextToken = UNKNOWN; break;
    }
}

// Skips whitespace characters in the input
void getNonBlank() {
    while (isspace(nextChar)) getChar();
}

// Main lex function. Analyzes characters and updates lexeme and token.
void lex() {
    lexeme[0] = '\0';
    getNonBlank();
    switch (charClass) {
        case LETTER:
            do {
                addChar();
                getChar();
            } while (charClass == LETTER || charClass == DIGIT);
            nextToken = IDENT;
            break;
        case DIGIT:
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

// Matches the current token with the expected token and goes to the next token
void match(int expectedToken) {
    if (nextToken != expectedToken) {
        error();
    } else {
        switch (expectedToken) {
            case PLUS_OP: printf("[operator [+]]\n"); break;
            case MINUS_OP: printf("[operator [-]]\n"); break;
            case MULT_OP: printf("[operator [*]]\n"); break;
            case DIV_OP: printf("[operator [/]]\n"); break;
            //other operators if necessary
        }
    }
    getNextToken();
}

// Error message and exits the program if an unexpected token 
void error(void) {
    fprintf(stderr, "Error: Unexpected token.\n");
    exit(EXIT_FAILURE);
}

// Analyzes and prints the structure of a factor in an expression
void factor(void) {
    if (nextToken == IDENT || nextToken == INT_LIT) {
        printf("[factor\n");
        match(nextToken);
        printf("[id [%s]]\n", lexeme); 
        printf("]\n");
    } else if (nextToken == LEFT_PAREN) {
        printf("[factor\n");
        match(LEFT_PAREN);
        printf("(\n");
        expression();
        match(RIGHT_PAREN);
        printf(")]\n");
    } else {
        error();
    }
}

// Analyzes and prints the structure of a term in an expression
void term(void) {
    printf("[term\n");
    factor();
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        printf("[");
        match(nextToken); 
        printf("]\n");
        factor();
    }
    printf("]\n");
}

// Analyzes and prints the structure of an expression
void expression() {
    printf("[expr\n");
    term();
    while (nextToken == PLUS_OP || nextToken == MINUS_OP) {
        printf("[");
        match(nextToken); 
        printf("]\n");
        term();
    }
    printf("]\n");
}

// Main function to initiate lexical analysis on a given file
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <source_file>\n", argv[0]);
        return -1;
    }

    initLexer(argv[1]);
    while (nextToken != END_OF_FILE) {
        lex();
    }

    fclose(in_fp);
    return 0;
}