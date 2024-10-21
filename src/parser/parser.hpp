#ifndef _ELC_PARSER_PARSER_
#define _ELC_PARSER_PARSER_

#include "ast.hpp"
#include "tokens.hpp"

#include <vector>

namespace elc {

ast::Block parse(std::vector<Token>& tokens);

}

#endif // !_ELC_PARSER_PARSER_
