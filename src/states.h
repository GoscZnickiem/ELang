#ifndef _ELC_STATES_
#define _ELC_STATES_

#include "tokens.h"

#include <stdbool.h>

void processState(bool* done, int* state, bool* skip, char character);

bool isFinalState(int state);

TokenType getEndTokenType(int state);

#endif // !_ELC_STATES_
