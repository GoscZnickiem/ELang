#include "tokens.h"

#include <string.h>

void createToken(Token *token, TokenType type, char *ident) {
	token->type = type;
	strncpy(token->ident, ident, MAX_IDENT_LEN - 1);
}
