#include "parser.c"
#include "main.c"
#include "utils.c"

void lex() {
    // ignore any spaces or tabs that might be before the next token.
    getNonBlank();

    // checks what kind of character encountered
    // determines how process the characters to form token
    switch (charClass) {
        // If letter, could be identifier or a keyword.
        case LETTER:
            // clear out any old data from lexeme to start fresh.
            memset(lexeme, 0, sizeof(lexeme));

            // keep adding letters or digits to form a complete identifier.
            do {
                addChar();
                getChar();
            } while (charClass == LETTER || charClass == DIGIT);

            // mark this token as an identifier.
            nextToken = IDENT;
            break;

        // If digit, then numeric literal.
        case DIGIT:
            // clearing lexeme to start capturing the number.
            memset(lexeme, 0, sizeof(lexeme));

            // keep adding digits to get the whole number.
            do {
                addChar();
                getChar();
            } while (charClass == DIGIT);

            // token as an integer literal after completion.
            nextToken = INT_LIT;
            break;

        // For characters that aren't letters or digits, mark unknown.
        case UNKNOWN:
            // lookup function helps id the character and assign the right token
            lookup(nextChar);
            getChar(); //Moving on to the next character
            break;

        // If hit end of the file, then done scanning.
        case EOF:
            // mark the token as END_OF_FILE to signal that scanning is complete.
            nextToken = END_OF_FILE;

            // lexeme to "EOF" to visually represent the end of file in output.
            strcpy(lexeme, "EOF");
            break;
    }

    // After iding the token and capturing the lexeme, print
    // helps see what token is processed and value associated with it.
    printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
}
void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar)) {
            charClass = LETTER;
        } else if (isdigit(nextChar)) {
            charClass = DIGIT;
        } else {
            charClass = UNKNOWN;
        }
    } else {
        charClass = EOF_TYPE;
    }
}
void addChar() {
    int len = strlen(lexeme);
    if (len + 1 < MAX_LEXEME_LENGTH) {
        lexeme[len] = nextChar;
        lexeme[len + 1] = 0;  // Null-terminate string
    } else {
        printf("Error - lexeme is too long \n");
    }
}
