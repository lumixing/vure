#include <stdio.h>
#include <stdlib.h>
#include "main.h"

int lexer_end(Lexer lexer) {
	return lexer.current >= lexer.buf_len;
}

char lexer_peek(Lexer lexer) {
	if (lexer_end(lexer)) {
		printf("tried to peek after end cur=%d len=%d\n", lexer.current, lexer.buf_len);
		exit(1);
	}

	return lexer.buf[lexer.current];
}

char lexer_consume(Lexer *lexer) {
	if (lexer_end(*lexer)) {
		printf("tried to consume after end cur=%d len=%d\n", lexer->current, lexer->buf_len);
		exit(1);
	}

	char ch = lexer_peek(*lexer);
	lexer->current++;
	return ch;
}

Span lexer_span(Lexer lexer) {
	return (Span){lexer.start, lexer.current};
}

void lexer_scan(Lexer *lexer) {
	while (!lexer_end(*lexer)) {
		lexer->start = lexer->current;
		char ch = lexer_consume(lexer);
		printf("%c\n", ch);
		switch (ch) {
		case '{':
			lexer->tokens_arr[lexer->tokens_len++] = (Token){TK_LBRACE, {}, lexer_span(*lexer)};
			printf("%d\n", lexer->tokens_len++);
			break;
		}
	}
}

int main() {
	FILE *input_file = fopen("test.vr", "rb");
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
	lexer.start = lexer.current = 0;
	lexer_scan(&lexer);
	printf("ds %d\n", lexer.tokens_len);

	for (int i = 0; i < lexer.tokens_len; i++) {
		printf("%d\n", lexer.tokens_arr[i].type);
	}

	free(input_buf);

	return 0;
}

