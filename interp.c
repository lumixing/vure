#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "ast.h"

Func *funcs_find(Func *funcs, size_t funcs_len, char *name) {
	for (int i = 0; i < funcs_len; i++) {
		if (strcmp(funcs[i].sign.name, name) != 0) continue;
		return &funcs[i];
	}
	return NULL;
}

void interp(TopStmt *tstmts, size_t tstmts_len) {
	Var vars[100];
	size_t vars_len = 0;

	Func funcs[100];
	size_t funcs_len = 0;

	for (int i = 0; i < tstmts_len; i++) {
		TopStmt tstmt = tstmts[i];
		switch (tstmt.type) {
			case TSTMT_BUILTIN_FUNC_DECL: {
				BuiltinFuncDecl t = tstmt.value.builtin_func_decl;
				funcs[funcs_len++] = (Func){t.sign, 0, 0, 1};
				break;
			}
			case TSTMT_FUNC_DEF: {
				FuncDef t = tstmt.value.func_def;
				funcs[funcs_len++] = (Func){t.sign, t.stmts, tstmts_len, 0};

				for (int j = 0; j < t.stmts_len; j++) {
					Stmt stmt = t.stmts[j];
					switch (stmt.type) {
						case STMT_VAR_DEF: {
							VarDef s = stmt.value.var_def;

							if (s.value.type != s.type) {
								printf("`%s` expected type %s but got %s", s.name, type_str(s.type), type_str(s.value.type));
								exit(1);
							}

							vars[vars_len++] = (Var){s.name, s.type, s.value.value};
							break;
						}
						case STMT_FUNC_CALL: {
							FuncCall s = stmt.value.func_call;
							Func *func = funcs_find(funcs, funcs_len, s.name);

							if (func == NULL) {
								printf("could not find func by name `%s`\n", s.name);
								exit(1);
							}

							if (func->builtin) {
								if (strcmp(func->sign.name, "print") == 0) {
									printf("%s\n", s.args[0].value.str);
								}
							}
						}
					}
				}

				break;
			}
		}
	}

	// printf("\nprinting %zu vars:\n", vars_len);
	// for (int i = 0; i < vars_len; i++) {
	// 	Var var = vars[i];
	// 	printf("[%d] `%s`: ", i, var.name);
	// 	printf("%s = ", type_str(var.type));
	// 	print_value(var.type, var.value);
	// 	printf("\n");
	// }

	// printf("\nprinting %zu funcs:\n", funcs_len);
	// for (int i = 0; i < funcs_len; i++) {
	// 	Func func = funcs[i];
	// 	char *builtin_str = func.builtin ? "builtin " : "";
	// 	printf("[%d] %s`%s` -> %s\n", i, builtin_str, func.sign.name, type_str(func.sign.ret_type));
	// }
}
