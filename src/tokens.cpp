#include "tokens.hpp"

#include <map>
#include <iostream>
#include <string>

namespace elc {

const std::map<TokenType, std::string> convert = {
	{ TokenType::INVALID, "Invalid" },
	{ TokenType::KEYWORD, "Keyword" },
	{ TokenType::IDENTIFIER, "Identifier" },
	{ TokenType::SYMBOL, "Symbol" },
	{ TokenType::NUMERAL, "Numeral" },
	{ TokenType::STRING, "String" }
};

std::ostream& operator<<(std::ostream& os, const Token& token) {
	os << convert.at(token.type) << " " << token.data << ", length: " << token.size << " at: " << token.line << ":" << token.column;
	return os;
}

}
