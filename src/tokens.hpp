#ifndef _ELC_TOKENS_
#define _ELC_TOKENS_

#include <cstddef>
#include <map>
#include <string>

namespace elc {

enum class TokenType {
	INVALID, START, END,

	IDENTIFIER, NUMERAL, BOOL, STRING,

	KEY_RETURN, KEY_FUN,

	SEMICOLON, COMMA, PAREN_L, PAREN_R, BRACE_L, BRACE_R, CARROT_L, CARROT_R,

	OP_ASSIGN, 
	OP_PLUS, OP_MINUS, OP_STAR, OP_DIV, OP_MODULO,
	OP_BITAND, OP_BITOR, OP_BITXOR, OP_BITNEGATE, OP_BITSHIFTLEFT, OP_BITSHIFTRIGHT,
	OP_EQUALS, OP_NOTEQUALS, OP_LESS, OP_LESSEQUAL, OP_GREATER, OP_GREATEREQUAL,
	OP_AND, OP_OR, OP_XOR, OP_NEGATE,

	ARROW_RIGHT
};

const inline std::map<std::string, TokenType> symbols = {
	{";", TokenType::SEMICOLON}, 
	{",", TokenType::COMMA}, 
	{"(", TokenType::PAREN_L},
	{")", TokenType::PAREN_R},
	{"{", TokenType::BRACE_L},
	{"}", TokenType::BRACE_R},
	{"<", TokenType::CARROT_L},
	{">", TokenType::CARROT_R},
	{"=", TokenType::OP_ASSIGN},
	{"+", TokenType::OP_PLUS},
	{"-", TokenType::OP_MINUS},
	{"*", TokenType::OP_STAR},
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
	{"!", TokenType::OP_NEGATE},
	{"->", TokenType::ARROW_RIGHT}
};

const inline std::map<std::string, TokenType> keywords = {
	{"return", TokenType::KEY_RETURN},
	{"fun", TokenType::KEY_FUN}
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
