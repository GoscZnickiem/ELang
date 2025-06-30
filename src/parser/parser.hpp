#ifndef _ELC_PARSER_PARSER_
#define _ELC_PARSER_PARSER_

#include "data/tokens.hpp"

#include <list>
#include <string>
#include <vector>

namespace elc {

struct ModuleSymbolCode {
	std::string name;
	std::list<Token> tokens;
};

[[nodiscard]] std::vector<ModuleSymbolCode> getModuleSymbols(std::list<Token>& tokens);

}

#endif // !_ELC_PARSER_PARSER_
