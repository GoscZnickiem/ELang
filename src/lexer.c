#include "lexer.h"
#include "states.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static bool bufferedChar = false;
static int c;
static bool done;
static bool eof = false;
static int state;
static char tokenBuffer[MAX_IDENT_LEN];
static int tokenLength;

static void step(FILE* file) {
	if(!bufferedChar)
		c = fgetc(file);
	if(c == EOF) {
		done = true;
		eof = true;
		return;
	}

	bool skip;
	processState(&done, &state, &skip, (char) c);

	if(tokenLength == MAX_IDENT_LEN) {
		// TODO: something
		fprintf(stderr, "tokenLength == max ident length");
		return;
	}
	if(!done) {
		tokenBuffer[tokenLength] = (char) c;
		tokenLength++;
	} else {
		bufferedChar = !skip;
	}
}

static void readToken(FILE* file) {
	done = 0;
	state = 0;
	tokenLength = 0;

	while(!done)
		step(file);

	if(state < 0) {
		fprintf(stderr, "invalid end state\n");
	}


}

void tokenize(TokenList* tokens, FILE* file) {
	Token* bufferToken = malloc(sizeof(Token));
	if(bufferToken == NULL) {
		fprintf(stderr, "Memory allocation error\n");
		return;
	}

	size_t strIndex = 0;

	int c;
	while ( (c = fgetc(file)) != EOF ) {

	}

	free(bufferToken);
}
