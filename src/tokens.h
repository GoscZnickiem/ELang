#ifndef _ELC_TOKENS_
#define _ELC_TOKENS_

#define MAX_IDENT_LEN 64


typedef enum iTokenType {
	INVALID,
	SEMICOLON,
	RETURN,
	IDENTIFIER,
	OPERATOR
} TokenType;

typedef struct {
	TokenType type;

	char ident[MAX_IDENT_LEN];
} Token;

typedef struct iTokenList {
	struct iTokenList* previous;
	struct iTokenList* next;
	Token token;
} TokenList;


void createToken(Token* token, TokenType type, char* ident);


#endif // !_ELC_TOKENS_
