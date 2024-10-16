#ifndef _ELC_TOKENS_
#define _ELC_TOKENS_

#include <cstddef>
#include <string>

namespace elc {

enum class TokenType {
	INVALID,
	END,
	IDENTIFIER,
	NUMERAL,
	STRING,
	KEY_FUN,
	KEY_RETURN,
	SEMICOLON,
	PAREN_L,
	PAREN_R,
	OP_PLUS,
	OP_MINUS,
};

struct Token {
	TokenType type;
	std::size_t line;
	std::size_t column;
	std::size_t size;

	std::string data;
};

std::ostream& operator<<(std::ostream& os, const Token& token);

std::string tokenTypeToString(TokenType type);

}

#endif // !_ELC_TOKENS_
