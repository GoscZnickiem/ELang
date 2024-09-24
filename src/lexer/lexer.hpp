#ifndef _ELC_LEXER_LEXER_
#define _ELC_LEXER_LEXER_

#include "tokens.hpp"

#include <istream>
#include <vector>

namespace elc {

std::vector<Token> tokenize(std::istream& stream);

}

#endif // !_ELC_LEXER_LEXER_
