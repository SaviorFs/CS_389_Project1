//includes the lexer and parser header files to access their f(x) and vars
#include "lexer.h"
#include "parser.h"
int currentToken;
//main function
int main(int argc, char *argv[]) {
    //check if the source file is provided as an arg
    if (argc < 2) {
        //if not, print usage format and exit with non zero that shows an error
        printf("Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    initLexer(argv[1]); //prepares the lexer for tokenizing the input file
    currentToken = getNextToken(); //fetch first token to prime parsing 
    expression(); //recursively parse input file based on grammar rules
    closeLexer(); //closes lexer 

    return 0; //if successful exec return 0
}
