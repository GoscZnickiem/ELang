#include "lexer.h"

#include <stdlib.h>
#include <string.h>

TokenList addToken(TokenList* tokens, Token* token) {

}

int isWhiteSpace(int c) {
	return c == ' ' || c == '\n';
}

int makeToken(Token* token, char* ident) {
	if(strcmp(ident, ";") != 0) {
		createToken(token, SEMICOLON, ";");
		return 1;
	} else if(strcmp(ident, "return") != 0) {
		createToken(token, RETURN, "return");
		return 1;
	} else if(strcmp(ident, "test") != 0) {
		createToken(token, IDENTIFIER, ident);
		return 1;
	}

	fprintf(stderr, "Lexer error\n");
	return 0;
}

void tokenize(TokenList* tokens, FILE* file) {
	Token* bufferToken = malloc(sizeof(Token));
	if(bufferToken == NULL) {
		fprintf(stderr, "Memory allocation error\n");
		return;
	}
	char strBuffer[MAX_IDENT_LEN] = { 0 };

	size_t strIndex = 0;
	int c;
	while ( (c = fgetc(file)) != EOF ) {
		if(isWhiteSpace(c)) {
			if(strIndex != 0) {
				makeToken(bufferToken, strBuffer);
				addToken(tokens, bufferToken);
				strBuffer[0] = 0;
				strIndex = 0;
			}
		} else {
			if(strIndex == MAX_IDENT_LEN) {
				fprintf(stderr, "aaaaaaaaaaaaaa\n");
				free(bufferToken);
				return;
			}
			strBuffer[strIndex] = (char)c;
			strIndex++;
		}
	}

	free(bufferToken);
}
