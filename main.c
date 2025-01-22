#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"
#include "interp.h"

int main() {
	FILE *input_file = NULL;
	fopen_s(&input_file, "test.vr", "rb");
	fseek(input_file, 0, SEEK_END);
	long file_len = ftell(input_file);
	rewind(input_file);

	char *input_buf = (char *)malloc(file_len * sizeof(char));
	fread(input_buf, file_len, 1, input_file);
	fclose(input_file);

	/*for (int i = 0; i < file_len; i++) {
		printf("%c", input_buf[i]);
	}*/

	Lexer lexer;
	lexer.buf = input_buf;
	lexer.buf_len = file_len;
	lexer.tokens_len = 0;
	lexer.start = lexer.current = 0;
	lexer_scan(&lexer);

	// printf("printing %zu tokens:\n", lexer.tokens_len);
	// for (int i = 0; i < lexer.tokens_len; i++) {
	// 	Token token = lexer.tokens_arr[i];
	// 	char *str = token_to_string(token); // this might leak memory!
	// 	printf("%s\n", str);

	// 	switch (token.type) {
	// 	case TK_IDENT:
	// 	case TK_LIT_STR:
	// 	case TK_LIT_INT:
	// 		free(str);
	// 		break;
	// 	default:
	// 		break;
	// 	}
	// }

	Parser prs;
	prs.tokens = lexer.tokens_arr;
	prs.tokens_len = lexer.tokens_len;
	prs.top_stmts_len = 0;
	prs.current = 0;
	prs_parse(&prs);

	// for (int i = 0; i < prs.top_stmts_len; i++) {
	// 	TopStmt tstmt = prs.top_stmts_arr[i];

	// 	if (tstmt.type == TSTMT_BUILTIN_FUNC_DECL) {
	// 		FuncSign s = tstmt.value.builtin_func_decl.sign;
	// 		Param p = s.params[0];
	// 		printf("BuiltinFuncDecl(`%s`, %zu(%s `%s`), %s)\n", s.name, s.params_len, type_str(p.type), p.name, type_str(s.ret_type));
	// 	} else if (tstmt.type == TSTMT_FUNC_DEF) {
	// 		FuncDef t = tstmt.value.func_def;
	// 		FuncSign s = t.sign;
	// 		Param p = s.params[0];
	// 		printf("FuncDef(`%s`, %zu(%s `%s`), %s)\n", s.name, s.params_len, type_str(p.type), p.name, type_str(s.ret_type));
	// 		for (int j = 0; j < t.stmts_len; j++) {
	// 			Stmt stmt = t.stmts[j];

	// 			if (stmt.type == STMT_VAR_DEF) {
	// 				VarDef s = stmt.value.var_def;
	// 				printf("\tVarDef(%s `%s`)\n", type_str(s.type), s.name);
	// 			} else if (stmt.type == STMT_FUNC_CALL) {
	// 				FuncCall s = stmt.value.func_call;
	// 				printf("\tFuncCall(`%s`)\n", s.name);
	// 			}
	// 		}
	// 	}
	// }

	interp(prs.top_stmts_arr, prs.top_stmts_len);

	free(input_buf);

	return 0;
}

