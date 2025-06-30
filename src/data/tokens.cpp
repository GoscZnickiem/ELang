#include "tokens.hpp"

#include <iostream>
#include <string>

namespace elc {

std::ostream& operator<<(std::ostream& os, const Token& token) {
	os << tokenTypeToString(token.type) << " " << token.data << ", length: " << token.data.size() << " at: " << token.line << ":" << token.column;
	return os;
}

std::string tokenTypeToString(TokenType type) {
	switch (type) {
		case TokenType::INVALID:		return "Invalid";
		case TokenType::START:			return "START";
		case TokenType::END:			return "END";
		case TokenType::SYMBOL:			return "Symbol";
		case TokenType::NUMERAL:		return "Numeral";
		case TokenType::BOOL:			return "Bool";
		case TokenType::STRING:			return "String";
		case TokenType::PAREN_L:		return "(";
		case TokenType::PAREN_R:		return ")";
		case TokenType::BRACE_L:		return "{";
		case TokenType::BRACE_R:		return "}";
		case TokenType::SEMICOLON:		return ";";
		case TokenType::COMMA:			return ",";
		case TokenType::SYMBOL_CLUSTER:	return "Symbol cluster";
	}
}

}
