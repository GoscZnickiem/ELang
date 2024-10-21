#ifndef _ELC_TOKENS_
#define _ELC_TOKENS_

#include <cstddef>
#include <map>
#include <string>

namespace elc {

enum class TokenType {
	INVALID,
	START,
	END,
	IDENTIFIER,
	NUMERAL,
	BOOL,
	STRING,
	KEY_FUN,
	KEY_RETURN,
	SEMICOLON,
	PAREN_L,
	PAREN_R,
	OP_PLUS,
	OP_MINUS,
	OP_MULT,
	OP_DIV,
	OP_ASSIGN,
};

const inline std::map<std::string, TokenType> symbols = {
	{";", TokenType::SEMICOLON}, 
	{"(", TokenType::PAREN_L},
	{")", TokenType::PAREN_R},
	{"+", TokenType::OP_PLUS},
	{"-", TokenType::OP_MINUS},
	{"*", TokenType::OP_MULT},
	{"/", TokenType::OP_DIV},
	{"=", TokenType::OP_ASSIGN}
};

const inline std::map<std::string, TokenType> keywords = {
	{"fun", TokenType::KEY_FUN}, 
	{"return", TokenType::KEY_RETURN} 
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

#endif // !_ELC_TOKENS_
