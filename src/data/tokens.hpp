#ifndef _ELC_DATA_TOKENS_
#define _ELC_DATA_TOKENS_

#include <cstddef>
#include <string>

namespace elc {

enum class TokenType {
	INVALID, START, END,

	SYMBOL, NUMERAL, STRING,

	SEMICOLON, COMMA, PAREN_L, PAREN_R, BRACE_L, BRACE_R,

	SYMBOL_CLUSTER
};

struct TokenMetaData {
	std::size_t lineStart;
	std::size_t columnStart;
	std::size_t lineEnd;
	std::size_t columnEnd;
};

struct Token {
	TokenType type;
	std::string data;
	TokenMetaData metadata;
};

std::ostream& operator<<(std::ostream& os, const Token& token);

std::string tokenTypeToString(TokenType type);

}

#endif // !_ELC_DATA_TOKENS_
