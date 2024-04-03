#ifndef PARSER_H
#define PARSER_H

void expression();
void term();
void factor();
void match(int expectedToken);
void error();

#endif
