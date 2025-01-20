#ifndef TOKEN_H
#define TOKEN_H

typedef struct Span {
	size_t lo;
	size_t hi;
} Span;

typedef union TokenValue {
	char *str;
	int num;
} TokenValue;

typedef enum TokenType {
	TK_KW_BUILTIN,
	TK_KW_PROC,
	TK_KW_STR,
	TK_KW_INT,
	TK_KW_VOID,

	TK_LPAREN,
	TK_RPAREN,
	TK_LBRACE,
	TK_RBRACE,
	TK_ARROW,
	TK_SEMICOLON,
	TK_EQUALS,

	TK_IDENT,

	TK_LIT_STR,
	TK_LIT_INT,

	TK_EOF,
} TokenType;

typedef struct Token {
	TokenType type;
	TokenValue value;
	Span span;
} Token;

char *token_to_string(Token token);

#endif
