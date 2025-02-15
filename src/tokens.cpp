#include "tokens.hpp"

#include <algorithm>
#include <map>
#include <iostream>
#include <string>

namespace elc {

namespace {

bool isInMap(const std::map<std::string, TokenType>& map, TokenType type) {
	return std::ranges::any_of(map, [type](const auto& entry){ return entry.second == type; });
}

std::string convert(TokenType type) {
	switch (type) {
		case TokenType::INVALID:		return "Invalid";
		case TokenType::START:			return "START";
		case TokenType::END:			return "END";
		case TokenType::IDENTIFIER:		return "Identifier";
		case TokenType::NUMERAL:		return "Numeral";
		case TokenType::BOOL:			return "Bool";
		case TokenType::STRING:			return "String";
		case TokenType::PAREN_L:		return "(";
		case TokenType::PAREN_R:		return ")";
		case TokenType::BRACE_L:		return "{";
		case TokenType::BRACE_R:		return "}";
		case TokenType::CARROT_L:		return "<";
		case TokenType::CARROT_R:		return ">";
		case TokenType::ARROW_RIGHT:	return "->";
		default:
			if(isInMap(keywords, type)) return "Keyword";
			if(isInMap(symbols, type))	return "Symbol";
			return "Unknown token type";
	}
}
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
	os << convert(token.type) << " " << token.data << ", length: " << token.data.size() << " at: " << token.line << ":" << token.column;
	return os;
}

std::string tokenTypeToString(TokenType type) {
	return convert(type);
}

}
