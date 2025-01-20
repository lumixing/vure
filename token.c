#include <stdio.h>
#include <stdlib.h>
#include "token.h"

char *token_to_string(Token token) {
	switch (token.type) {
	case TK_KW_BUILTIN: return "builtin";
	case TK_KW_PROC: return "proc";
	case TK_KW_STR: return "str";
	case TK_KW_INT: return "int";
	case TK_KW_VOID: return "void";
	case TK_LPAREN: return "(";
	case TK_RPAREN: return ")";
	case TK_LBRACE: return "{";
	case TK_RBRACE: return "}";
	case TK_ARROW: return "->";
	case TK_SEMICOLON: return ";";
	case TK_EQUALS: return "=";
	case TK_IDENT:
		int len = snprintf(NULL, 0, "ident(\"%s\")", token.value.str) + 1;
		char *str = malloc(len * sizeof(char));
		sprintf(str, "ident(\"%s\")", token.value.str);
		return str;
	case TK_LIT_STR:
		len = snprintf(NULL, 0, "str(\"%s\")", token.value.str) + 1;
		str = malloc(len * sizeof(char));
		sprintf(str, "str(\"%s\")", token.value.str);
		return str;
	case TK_LIT_INT:
		len = snprintf(NULL, 0, "int(%d)", token.value.num) + 1;
		str = malloc(len * sizeof(char));
		sprintf(str, "int(%d)", token.value.num);
		return str;
	case TK_EOF: return "eof";
	default:
		printf("unknown token type for string: %d\n", token.type);
		return "<UNKNOWN TOKEN TYPE>";
	}
}
