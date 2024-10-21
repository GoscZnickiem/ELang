#ifndef _ELC_TOKENS_
#define _ELC_TOKENS_

#include <cstddef>
#include <map>
#include <string>

namespace elc {

enum class TokenType {
	INVALID, START, END,

	IDENTIFIER, NUMERAL, BOOL, STRING,

	KEY_FUN, KEY_RETURN,

	SEMICOLON, PAREN_L, PAREN_R,

	OP_ASSIGN, 
	OP_PLUS, OP_MINUS, OP_MULT, OP_DIV, OP_MODULO,
	OP_BITAND, OP_BITOR, OP_BITXOR, OP_BITNEGATE, OP_BITSHIFTLEFT, OP_BITSHIFTRIGHT,
	OP_EQUALS, OP_NOTEQUALS, OP_LESS, OP_LESSEQUAL, OP_GREATER, OP_GREATEREQUAL,
	OP_AND, OP_OR, OP_XOR, OP_NEGATE
};

const inline std::map<std::string, TokenType> symbols = {
	{";", TokenType::SEMICOLON}, 
	{"(", TokenType::PAREN_L},
	{")", TokenType::PAREN_R},
	{"=", TokenType::OP_ASSIGN},
	{"+", TokenType::OP_PLUS},
	{"-", TokenType::OP_MINUS},
	{"*", TokenType::OP_MULT},
	{"/", TokenType::OP_DIV},
	{"%", TokenType::OP_MODULO},
	{"&", TokenType::OP_BITAND},
	{"|", TokenType::OP_BITOR},
	{"^", TokenType::OP_BITXOR},
	{"~", TokenType::OP_BITNEGATE},
	{"<<", TokenType::OP_BITSHIFTLEFT},
	{">>", TokenType::OP_BITSHIFTRIGHT},
	{"==", TokenType::OP_EQUALS},
	{"!=", TokenType::OP_NOTEQUALS},
	{"<", TokenType::OP_LESS},
	{"<=", TokenType::OP_LESSEQUAL},
	{">", TokenType::OP_GREATER},
	{">=", TokenType::OP_GREATEREQUAL},
	{"&&", TokenType::OP_AND},
	{"||", TokenType::OP_OR},
	{"^^", TokenType::OP_XOR},
	{"!", TokenType::OP_NEGATE}
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
