#ifndef _ELC_PARSER_PARSER_
#define _ELC_PARSER_PARSER_

#include "data/ast.hpp"
#include "data/tokens.hpp"

#include <list>
#include <vector>

namespace elc {

std::vector<ast::Declaration> parse(std::list<Token>& tokens);

}

#endif // !_ELC_PARSER_PARSER_
