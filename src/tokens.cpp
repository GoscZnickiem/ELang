#include "tokens.hpp"

#include <map>
#include <iostream>
#include <string>

namespace elc {

const std::map<TokenType, std::string> convert = {
	{ TokenType::INVALID, "Invalid" },
	{ TokenType::END, "END" },
	{ TokenType::IDENTIFIER, "Identifier" },
	{ TokenType::NUMERAL, "Numeral" },
	{ TokenType::STRING, "String" },
	{ TokenType::SEMICOLON, ";" },
	{ TokenType::KEY_FUN, "fun" },
	{ TokenType::KEY_RETURN, "return" },
	{ TokenType::PAREN_L, "(" },
	{ TokenType::PAREN_R, ")" },
	{ TokenType::OP_PLUS, "+" },
	{ TokenType::OP_MINUS, "-" }
};

std::ostream& operator<<(std::ostream& os, const Token& token) {
	os << convert.at(token.type) << " " << token.data << ", length: " << token.size << " at: " << token.line << ":" << token.column;
	return os;
}

}
