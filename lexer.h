#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct Lexer {
	char *buf;
	size_t buf_len;
	Token tokens_arr[1000];
	Token *tokens;
	size_t tokens_len;
	size_t start;
	size_t current;
} Lexer;

int lexer_end(Lexer lexer);
char lexer_peek(Lexer lexer);
char lexer_consume(Lexer *lexer);
Span lexer_span(Lexer lexer);
void lexer_add_token(Lexer *lexer, TokenType type, TokenValue value);
int char_is_alpha(char ch);
int char_is_digit(char ch);
void lexer_scan(Lexer *lexer);

#endif
