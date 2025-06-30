#ifndef _ELC_PARSER_INDEXER_
#define _ELC_PARSER_INDEXER_

#include "data/tokens.hpp"

#include <list>
#include <string>
#include <vector>

namespace elc {

struct Stub {
	std::string name;
	std::list<Token> tokens;
};

[[nodiscard]] std::vector<Stub> index(std::list<Token>& tokens);

}

#endif // !_ELC_PARSER_INDEXER_
