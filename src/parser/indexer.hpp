#ifndef _ELC_PARSER_INDEXER_
#define _ELC_PARSER_INDEXER_

#include "data/tokens.hpp"

#include <list>
#include <string>
#include <vector>

namespace elc {

[[nodiscard]] std::vector<Stub> index(std::list<Token>& tokens);

}

#endif // !_ELC_PARSER_INDEXER_
