#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

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

	printf("printing %zu tokens:\n", lexer.tokens_len);
	for (int i = 0; i < lexer.tokens_len; i++) {
		Token token = lexer.tokens_arr[i];
		char *str = token_to_string(token); // this might leak memory!
		printf("%s\n", str);

		switch (token.type) {
		case TK_IDENT:
		case TK_LIT_STR:
		case TK_LIT_INT:
			free(str);
			break;
		default:
			break;
		}
	}

	free(input_buf);

	return 0;
}

