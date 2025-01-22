#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

int lexer_end(Lexer lexer) {
	return lexer.current >= lexer.buf_len;
}

char lexer_peek(Lexer lexer) {
	if (lexer_end(lexer)) {
		printf("tried to peek after end cur=%zu len=%zu\n", lexer.current, lexer.buf_len);
		exit(1);
	}

	return lexer.buf[lexer.current];
}

char lexer_consume(Lexer *lexer) {
	if (lexer_end(*lexer)) {
		printf("tried to consume after end cur=%zu len=%zu\n", lexer->current, lexer->buf_len);
		exit(1);
	}

	char ch = lexer_peek(*lexer);
	lexer->current++;
	return ch;
}

Span lexer_span(Lexer lexer) {
	return (Span){lexer.start, lexer.current};
}

void lexer_add_token(Lexer *lexer, TokenType type, TokenValue value) {
	lexer->tokens_arr[lexer->tokens_len++] = (Token){type, value, lexer_span(*lexer)};
}

int char_is_alpha(char ch) {
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

int char_is_digit(char ch) {
	return ch >= '0' && ch <= '9';
}

void lexer_scan(Lexer *lexer) {
	while (!lexer_end(*lexer)) {
		lexer->start = lexer->current;
		char ch = lexer_consume(lexer);
		switch (ch) {
		case ' ':
		case '\n':
		case '\r':
		case '\t':
			continue;
		case '{':
			lexer_add_token(lexer, TK_LBRACE, (TokenValue){});
			break;
		case '}':
			lexer_add_token(lexer, TK_RBRACE, (TokenValue){});
			break;
		case '(':
			lexer_add_token(lexer, TK_LPAREN, (TokenValue){});
			break;
		case ')':
			lexer_add_token(lexer, TK_RPAREN, (TokenValue){});
			break;
		case ';':
			lexer_add_token(lexer, TK_SEMICOLON, (TokenValue){});
			break;
		case '=':
			lexer_add_token(lexer, TK_EQUALS, (TokenValue){});
			break;
		case '-':
			if (lexer_consume(lexer) == '>') {
				lexer_add_token(lexer, TK_ARROW, (TokenValue){});
				break;
			}
			printf("expected > after - for -> at %zu\n", lexer->current);
			exit(1);
		case '"':
			int terminated = 1;

			while (lexer_peek(*lexer) != '"') {
				if (lexer_end(*lexer) || lexer_peek(*lexer) == '\n') {
					terminated = 0;
					break;
				}
				lexer->current++;
			}

			if (!terminated) {
				printf("untermiated string!\n");
				exit(1);
			}

			lexer->current++; // consume second quote
			
			size_t size = lexer->current - 1 - lexer->start - 1;
			char *lexeme = malloc(size + 1);
			strncpy(lexeme, lexer->buf + lexer->start + 1, size);
			lexeme[size] = 0;

			lexer_add_token(lexer, TK_LIT_STR, (TokenValue)lexeme);
			
			break;
		case '#':
			while (lexer_peek(*lexer) != '\r') {
				lexer->current++;
			}
			break;
		default:
			if (char_is_alpha(ch)) {
				while (char_is_alpha(lexer_peek(*lexer))) {
					lexer->current++;
				}
				size_t size = lexer->current - lexer->start;
				char *lexeme = malloc(size + 1);
				strncpy(lexeme, lexer->buf + lexer->start, size);
				lexeme[size] = 0;

				if (strcmp(lexeme, "builtin") == 0) {
					lexer_add_token(lexer, TK_KW_BUILTIN, (TokenValue){});
				} else if (strcmp(lexeme, "proc") == 0) {
					lexer_add_token(lexer, TK_KW_PROC, (TokenValue){});
				} else if (strcmp(lexeme, "str") == 0) {
					lexer_add_token(lexer, TK_KW_STR, (TokenValue){});
				} else if (strcmp(lexeme, "int") == 0) {
					lexer_add_token(lexer, TK_KW_INT, (TokenValue){});
				} else if (strcmp(lexeme, "void") == 0) {
					lexer_add_token(lexer, TK_KW_VOID, (TokenValue){});
				} else {
					lexer_add_token(lexer, TK_IDENT, (TokenValue)lexeme);
				}
			} else if (char_is_digit(ch)) {
				while (char_is_digit(lexer_peek(*lexer))) {
					lexer->current++;
				}
				size_t size = lexer->current - lexer->start;
				char *lexeme = malloc(size + 1);
				strncpy(lexeme, lexer->buf + lexer->start, size);
				lexeme[size] = 0;
				int parsed_int;
				sscanf(lexeme, "%d", &parsed_int);
				lexer_add_token(lexer, TK_LIT_INT, (TokenValue)parsed_int);
			} else {
				printf("unexpected char: %c (%d)\n", ch, ch);
				// exit(1);
			}
		}
	}
}
