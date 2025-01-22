#ifndef AST_H
#define AST_H

#include "token.h"

typedef enum Type {
    TYPE_VOID,
    TYPE_STR,
    TYPE_INT,
    TYPE_VAR,
} Type;

typedef struct Value {
    Type type;
    TokenValue value;
} Value;

typedef struct Param {
    char *name;
    Type type;
} Param;

typedef struct VarDef {
    Type type;
    char *name;
    Value value;
} VarDef;

typedef struct FuncCall {
    char *name;
    Value *args;
    size_t args_len;
} FuncCall;

typedef enum StmtType {
    STMT_VAR_DEF,
    STMT_FUNC_CALL,
} StmtType;

typedef union StmtValue {
    VarDef var_def;
    FuncCall func_call;
} StmtValue;

typedef struct Stmt {
    StmtType type;
    StmtValue value;
} Stmt;

typedef struct FuncSign {
    char *name;
    Param *params;
    size_t params_len;
    Type ret_type;
} FuncSign;

typedef struct BuiltinFuncDecl {
    FuncSign sign;
} BuiltinFuncDecl;

typedef struct FuncDef {
    FuncSign sign;
    Stmt *stmts;
    size_t stmts_len;
} FuncDef;

typedef enum TopStmtType {
    TSTMT_BUILTIN_FUNC_DECL,
    TSTMT_FUNC_DEF,
} TopStmtType;

typedef union TopStmtValue {
    BuiltinFuncDecl builtin_func_decl;
    FuncDef func_def;
} TopStmtValue;

typedef struct TopStmt {
    TopStmtType type;
    TopStmtValue value;
} TopStmt;

char *type_str(Type type);
void print_value(Type type, TokenValue value);

#endif
