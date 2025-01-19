typedef struct Span {
	size_t lo;
	size_t hi;
} Span;

typedef union TokenValue {
	char *str;
	int num;
} TokenValue;

typedef enum TokenType {
	TK_KW_BUILTIN,
	TK_KW_PROC,
	TK_KW_STR,
	TK_KW_INT,
	TK_KW_VOID,

	TK_LPAREN,
	TK_RPAREN,
	TK_LBRACE,
	TK_RBRACE,
	TK_ARROW,
	TK_SEMICOLON,
	TK_EQUALS,

	TK_IDENT,

	TK_LIT_STR,
	TK_LIT_INT,

	TK_EOF,
} TokenType;

typedef struct Token {
	TokenType type;
	TokenValue value;
	Span span;
} Token;

typedef struct Lexer {
	char *buf;
	size_t buf_len;
	Token tokens_arr[1000];
	Token *tokens;
	size_t tokens_len;
	size_t start;
	size_t current;
} Lexer;



