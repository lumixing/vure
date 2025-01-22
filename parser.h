#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast.h"

typedef struct Parser {
    Token *tokens;
    size_t tokens_len;
    TopStmt top_stmts_arr[100];
    size_t top_stmts_len;
    size_t current;
} Parser;

void prs_parse(Parser *prs);

#endif
