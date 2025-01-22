#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "ast.h"
#include "token.h"

int prs_end(Parser prs) {
    return prs.current >= prs.tokens_len;
}

Token prs_peek(Parser prs) {
    if (prs_end(prs)) {
		printf("tried to peek after end cur=%zu len=%zu\n", prs.current, prs.tokens_len);
		exit(1);
	}

	return prs.tokens[prs.current];
}

Token prs_consume(Parser *prs) {
    if (prs_end(*prs)) {
		printf("tried to peek after end cur=%zu len=%zu\n", prs->current, prs->tokens_len);
		exit(1);
	}

    Token tk = prs_peek(*prs);
    prs->current++;
    return tk;
}

TokenValue prs_expect(Parser *prs, TokenType tk_type) {
    Token tk = prs_consume(prs);
    if (tk.type == tk_type) {
        return tk.value;
    }
    printf("expected %s but got %s\n", token_to_string((Token){tk_type}), token_to_string(tk));
    exit(1);
}

Type prs_type(Parser *prs) {
    Token tk = prs_consume(prs);
    switch (tk.type) {
    case TK_KW_INT: return TYPE_INT;
    case TK_KW_STR: return TYPE_STR;
    case TK_KW_VOID: return TYPE_VOID;
    default:
        printf("expected type but got %s\n", token_to_string(tk));
        exit(1);
    }
}

int prs_is_type(Parser prs) {
    Token tk = prs_peek(prs);
    switch (tk.type) {
    case TK_KW_INT:
    case TK_KW_STR:
    case TK_KW_VOID:
        return 1;
    default:
        return 0;
    }
}

FuncSign prs_func_sign(Parser *prs) {
    char* name = prs_expect(prs, TK_IDENT).str;
    prs_expect(prs, TK_LPAREN);
    Type param_type = prs_type(prs);
    char* param_name = prs_expect(prs, TK_IDENT).str;
    prs_expect(prs, TK_RPAREN);
    Type ret_type = prs_type(prs);

    Param *params = malloc(sizeof(Param)); // @free
    params[0].name = param_name;
    params[0].type = param_type;

    return (FuncSign){name, params, 1, ret_type};
}

Value prs_value(Parser *prs) {
    Token tk = prs_consume(prs);

    switch (tk.type) {
        case TK_LIT_INT: return (Value){TYPE_INT, tk.value};
        case TK_LIT_STR: return (Value){TYPE_STR, tk.value};
        case TK_IDENT: return (Value){TYPE_VAR, tk.value};
        default:
            printf("expected value but got %s\n", token_to_string(tk));
            exit(1);
    }
}

Stmt prs_stmt(Parser *prs) {
    Token tk = prs_peek(*prs);

    switch (tk.type) {
        case TK_IDENT: {
            prs->current++;
            char *name = tk.value.str;
            prs_expect(prs, TK_LPAREN);
            Value value = prs_value(prs);
            prs_expect(prs, TK_RPAREN);
            prs_expect(prs, TK_SEMICOLON);

            Value *args = malloc(sizeof(Value));
            args[0] = value;
            FuncCall func_call = {name, args, 1};

            return (Stmt){STMT_FUNC_CALL, (StmtValue)func_call};
        } break;
        default: {
            if (prs_is_type(*prs)) {
                Type type = prs_type(prs);
                char *name = prs_expect(prs, TK_IDENT).str;
                prs_expect(prs, TK_EQUALS);
                Value value = prs_value(prs);
                prs_expect(prs, TK_SEMICOLON);

                return (Stmt){STMT_VAR_DEF, (VarDef){type, name, value}};
            } else {
                printf("expected stmt but got %s\n", token_to_string(tk));
                exit(1);
            }
        } break;
    }
}

void prs_parse(Parser *prs) {
    while (!prs_end(*prs)) {
        Token tk = prs_consume(prs);

        switch (tk.type) {
            case TK_KW_BUILTIN: {
                prs_expect(prs, TK_KW_PROC);
                FuncSign func_sign = prs_func_sign(prs);
                prs_expect(prs, TK_SEMICOLON);

                TopStmtValue tstmt;
                tstmt.builtin_func_decl = (BuiltinFuncDecl){func_sign};
                prs->top_stmts_arr[prs->top_stmts_len++] = (TopStmt){TSTMT_BUILTIN_FUNC_DECL, tstmt};
                break;
            }
            case TK_KW_PROC: {
                FuncSign func_sign = prs_func_sign(prs);
                prs_expect(prs, TK_LBRACE);

                Stmt stmts[100];
                size_t stmts_len = 0;

                while (prs_peek(*prs).type != TK_RBRACE) {
                    Stmt stmt = prs_stmt(prs);
                    stmts[stmts_len++] = stmt;
                }

                prs_expect(prs, TK_RBRACE);
                TopStmtValue tstmt;
                tstmt.func_def = (FuncDef){func_sign, stmts, stmts_len};
                prs->top_stmts_arr[prs->top_stmts_len++] = (TopStmt){TSTMT_FUNC_DEF, tstmt};
                break;
            }
            default: {
                printf("unexpected token: %s\n", token_to_string(tk));
                break;
                // exit(1);
            }
        }
    }
}
