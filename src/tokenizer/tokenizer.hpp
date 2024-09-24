#ifndef _ELC_TOKENIZER_TOKENIZER_
#define _ELC_TOKENIZER_TOKENIZER_

#include "tokens.hpp"

#include <istream>
#include <vector>

namespace elc {

std::vector<Token> tokenize(std::istream& stream);

}

#endif // !_ELC_TOKENIZER_TOKENIZER_
