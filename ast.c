#include "stdio.h"
#include "ast.h"

char *type_str(Type type) {
	switch (type) {
	case TYPE_VOID: return "void";
	case TYPE_INT: return "int";
	case TYPE_STR: return "str";
	case TYPE_VAR: return "var";
	}
}

void print_value(Type type, TokenValue value) {
	switch (type) {
		case TYPE_INT:
			printf("%d", value.num);
			break;
		case TYPE_STR:
			printf("\"%s\"", value.str);
			break;
		default:
			printf("cant print value of type %s", type_str(type));
			break;
	}
}
