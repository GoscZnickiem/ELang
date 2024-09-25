#ifndef _ELC_TOKENS_
#define _ELC_TOKENS_

#include <cstddef>
#include <string>

namespace elc {

enum class TokenType {
	INVALID,
	KEYWORD,
	SYMBOL,
	IDENTIFIER,
	NUMERAL,
	STRING
};

struct Token {
	TokenType type;
	std::size_t line;
	std::size_t column;
	std::size_t size;

	std::string data;
};

std::ostream& operator<<(std::ostream& os, const Token& token);

}

#endif // !_ELC_TOKENS_
