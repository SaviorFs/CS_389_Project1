#include "lexer.h"  
#include "parser.h"  

int currentToken;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    initLexer(argv[1]);  
    currentToken = getNextToken();  
    expression();  
    closeLexer(); 

    return 0;
}