#ifndef INTERP_H
#define INTERP_H
#include "ast.h"

typedef struct Var {
	char *name;
	Type type;
	TokenValue value;
} Var;

typedef struct Func {
	FuncSign sign;
	Stmt *stmts;
	size_t stmts_len;
	int builtin;
} Func;

void interp(TopStmt *tstmts, size_t tstmts_len);

#endif
