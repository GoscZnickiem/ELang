#include "lexer.hpp"
#include "tokens.hpp"
#include "tokenizer.hpp"
#include <istream>
#include <vector>

namespace elc {

std::vector<Token> tokenize(std::istream& stream) {
	Tokenizer t(&stream);
	return t.tokenize();
}

}
