#include "states.h"

#define STATE_COUNT 1

bool isFinalStateTable[STATE_COUNT] = {
	true
};

int stateTables[STATE_COUNT][128] = {
	{
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
	}
};

TokenType tokenTypeTable[STATE_COUNT] = {
	INVALID
};

void processState(bool* done, int* state, bool* skip, char character) {
	char c = (char) character;
	int result = stateTables[*state][c];

	if(result == -1)
		*done = true;
	else
		*state = result;
}

bool isFinalState(int state) {
	return isFinalStateTable[state];
}

TokenType getEndTokenType(int state) {
	return tokenTypeTable[state];
}
