#include "lexer.h"
#include "parser.h"

extern int currentToken; 

void expression() {
    term();
    while (currentToken == PLUS_OP || currentToken == MINUS_OP) {
        match(currentToken); 
        term(); 
    }
}

void term() {
    factor();
    while (currentToken == MULT_OP || currentToken == DIV_OP) {
        match(currentToken);
        factor();
    }
}

void factor() {
    if (currentToken == IDENT || currentToken == INT_LIT) {
        match(currentToken); 
    } else if (currentToken == LEFT_PAREN) {
        match(LEFT_PAREN);
        expression(); 
        match(RIGHT_PAREN);
    } else {
        error(); 
    }
}

void match(int expectedToken) {
    if (currentToken == expectedToken) {
        currentToken = getNextToken(); 
    } else {
        error(); 
    }
}

void error() {
    fprintf(stderr, "Error in parsing.\n");
    
}
