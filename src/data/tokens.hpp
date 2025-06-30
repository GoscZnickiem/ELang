#ifndef _ELC_DATA_TOKENS_
#define _ELC_DATA_TOKENS_

#include <cstddef>
#include <string>

namespace elc {

enum class TokenType {
	INVALID, START, END,

	SYMBOL, NUMERAL, BOOL, STRING,

	SEMICOLON, COMMA, PAREN_L, PAREN_R, BRACE_L, BRACE_R,

	SYMBOL_CLUSTER
};

struct Token {
	TokenType type;
	std::size_t line;
	std::size_t column;

	std::string data;
};

std::ostream& operator<<(std::ostream& os, const Token& token);

std::string tokenTypeToString(TokenType type);

}

#endif // !_ELC_DATA_TOKENS_
