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

Var *vars_find(Var *vars, size_t vars_len, char *name) {
	for (int i = 0; i < vars_len; i++) {
		if (strcmp(vars[i].name, name) != 0) continue;
		return &vars[i];
	}
	return NULL;
}

typedef struct Data {
	Var vars[100];
	size_t vars_len;
	Func funcs[100];
	size_t funcs_len;
} Data;

void interp_stmt(Data *data, Stmt stmt) {
	switch (stmt.type) {
		case STMT_VAR_DEF: {
			VarDef s = stmt.value.var_def;

			if (s.value.type != s.type) {
				printf("`%s` expected type %s but got %s", s.name, type_str(s.type), type_str(s.value.type));
				exit(1);
			}

			data->vars[data->vars_len++] = (Var){s.name, s.type, s.value.value};
			break;
		}
		case STMT_FUNC_CALL: {
			FuncCall s = stmt.value.func_call;
			Func *func = funcs_find(data->funcs, data->funcs_len, s.name);

			if (func == NULL) {
				printf("could not find func by name `%s`\n", s.name);
				exit(1);
			}

			if (func->builtin) {
				if (strcmp(func->sign.name, "print") == 0) {
					switch (s.args[0].type) {
						case TYPE_STR:
							printf("%s\n", s.args[0].value.str);
							break;
						case TYPE_VAR:
							Var *var = vars_find(data->vars, data->vars_len, s.args[0].value.str);

							if (var == NULL) {
								printf("could not find var by name `%s`\n", s.args[0].value.str);
								exit(1);
							}

							printf("%s\n", var->value.str);
							break;
						default:
							printf("expected str for 1st arg of print\n");
							exit(1);
					}
				}
			} else {
				printf("calling %s with %zu stmts\n", func->sign.name, func->stmts_len);
				for (int i = 0; i < func->stmts_len; i++) {
					printf("%d\n", func->stmts[i].type);
					interp_stmt(data, func->stmts[i]);
				}
			}
			break;
		}
	}
}

void interp(TopStmt *tstmts, size_t tstmts_len) {
	Data data;
	data.vars_len = data.funcs_len = 0;

	for (int i = 0; i < tstmts_len; i++) {
		TopStmt tstmt = tstmts[i];
		switch (tstmt.type) {
			case TSTMT_BUILTIN_FUNC_DECL: {
				BuiltinFuncDecl t = tstmt.value.builtin_func_decl;
				data.funcs[data.funcs_len++] = (Func){t.sign, 0, 0, 1};
				break;
			}
			case TSTMT_FUNC_DEF: {
				FuncDef t = tstmt.value.func_def;
				data.funcs[data.funcs_len++] = (Func){t.sign, t.stmts, t.stmts_len, 0};

				break;
			}
		}
	}

	Func *main_func = funcs_find(data.funcs, data.funcs_len, "main");

	if (main_func == NULL) {
		printf("could not find main function!\n");
		exit(1);
	}

	for (int i = 0; i < main_func->stmts_len; i++) {
		Stmt stmt = main_func->stmts[i];
		interp_stmt(&data, stmt);
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
