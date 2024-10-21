#include "tokens.hpp"

#include <algorithm>
#include <map>
#include <iostream>
#include <string>

namespace elc {

bool isInMap(const std::map<std::string, TokenType>& map, TokenType type) {
	return std::any_of(map.begin(), map.end(), [type](const auto& entry){ return entry.second == type; });
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
		default:
			if(isInMap(keywords, type)) return "Keyword";
			if(isInMap(symbols, type))	return "Symbol";
			return "Unknown token type";
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
