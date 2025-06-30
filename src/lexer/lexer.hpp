#ifndef _ELC_LEXER_LEXER_
#define _ELC_LEXER_LEXER_

#include "data/tokens.hpp"

#include <istream>
#include <list>

namespace elc {

[[nodiscard]] std::list<Token> lex(std::istream& stream);

}

#endif // !_ELC_LEXER_LEXER_
