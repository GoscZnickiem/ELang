#include "lexer.hpp"
#include "data/tokens.hpp"
#include <istream>
#include <list>
#include <map>
#include <stdexcept>
#include <string>

namespace elc {

enum class State {
	INIT, COMMENT_LINE, COMMENT_BLOCK,

	STRING, NUMERAL, IDENT, SYMBOL, 

	DOT, SLASH
};

namespace {

void emit(std::list<Token>& tokens, TokenType type, TokenMetaData& meta, std::string& data) {
	tokens.emplace_back(type, meta.lineStart, meta.columnStart, data);
	data.clear();
}

void emit(std::list<Token>& tokens, TokenType type, TokenMetaData& meta) {
	tokens.emplace_back(type, meta.lineStart, meta.columnStart, "");
}

void stateInit(std::list<Token>& tokens, State& state, std::string& buffer, TokenMetaData& meta, char c) {
	if(std::isspace(c) != 0) {
		return;
	}

	meta.lineStart = meta.lineEnd;
	meta.columnStart = meta.columnEnd;

	if(c == '"') { 
		state = State::STRING;
	} else if(std::isdigit(c) != 0) {
		buffer += c;
		state = State::NUMERAL;
	} else if(std::isalpha(c) != 0 || c == '_') {
		buffer += c;
		state = State::IDENT;
	} else if(c == '.') {
		buffer += c;
		state = State::DOT;
	} else if(c == '/') {
		buffer += c;
		state = State::SLASH;
	} else if(c == ';') {
		emit(tokens, TokenType::SEMICOLON, meta);
	} else if(c == ',') {
		emit(tokens, TokenType::COMMA, meta);
	} else if(c == '{') {
		emit(tokens, TokenType::BRACE_L, meta);
	} else if(c == '}') {
		emit(tokens, TokenType::BRACE_R, meta);
	} else if(c == '(') {
		emit(tokens, TokenType::PAREN_L, meta);
	} else if(c == ')') {
		emit(tokens, TokenType::PAREN_R, meta);
	} else if(std::ispunct(c) != 0) {
		buffer += c;
		state = State::SYMBOL;
	}
}

void stateCommentLine(State& state, char c) {
	if(c == '\n') {
		state = State::INIT;
	}
}

void stateCommentBlock(State& state, std::string& buffer, char c) {
	if(c == '*') {
		buffer = "*";
	} else if(c == '/' && buffer == "*") {
		buffer.clear();
		state = State::INIT;
	} else {
		buffer.clear();
	}
}

bool stateStringReadEscape(std::string& buffer, TokenMetaData& meta, char c, std::string& escape, bool& readEscape) {
	static const std::map<char, char> escapeCodes = {
		{'\\', '\\'}, {'\'', '\''},
		{'\"', '\"'}, {'n', '\n'},
		{'r', '\r'}, {'t', '\t'},
		{'b', '\b'}, {'f', '\f'},
		{'a', '\a'}, {'v', '\v'},
		{'?', '\?'}
	};
	static bool escapeOctal;

	if(escape == "") {
		if(escapeCodes.contains(c)) {
			buffer += escapeCodes.at(c);
			readEscape = false;
		} else if(c == 'x') {
			escape += ' ';
			escapeOctal = false;
		} else if(std::isdigit(c) != 0 && c != '8' && c != '9') {
			escape += c;
			escapeOctal = true;
		} else {
			throw std::runtime_error("Error: Unknown escape code '\\" +
							std::to_string(c) +
							"'. Line: " +
							std::to_string(meta.lineEnd) + 
							", Column: " + 
							std::to_string(meta.columnEnd) +
							".");
		}
	} else {
		if(escapeOctal) {
			if(escape.length() < 3 && std::isdigit(c) != 0 && c != '8' && c != '9') {
				escape += c;
			} else {
				constexpr int octalBase = 8;
				buffer += static_cast<char>(std::stoi(escape, nullptr, octalBase)); // TODO: handle overflow
				return true;
			}
		} else {
			if(std::isxdigit(c) != 0) {
				escape += c;
			} else if(escape == " ") {
				throw std::runtime_error("Error: '\\x' escape code used with no following hex digits. Line: " +
							 std::to_string(meta.lineEnd) + 
							 ", Column: " + 
							 std::to_string(meta.columnEnd) +
							 ".");
			} else {
				constexpr int hexBase = 16;
				buffer += static_cast<char>(std::stoi(escape, nullptr, hexBase)); // TODO: handle overflow
				return true;
			}
		}
	}
	return false;
}

void stateString(std::list<Token>& tokens, State& state, std::string& buffer, TokenMetaData& meta, char c) {
	static std::string escape;
	static bool readEscape = false;

	if(readEscape) {
		if(!stateStringReadEscape(buffer, meta, c, escape, readEscape)) {
			return;
		}
	}  

	if(c == '"') {
		emit(tokens, TokenType::STRING, meta, buffer);
		state = State::INIT;
	} else if(c == '\\') {
		escape.clear();
		readEscape = true;
	} else if(c == '\n') {
		throw std::runtime_error("Error: Missing string literal termination character '\"'. Line: " +
						   std::to_string(meta.lineEnd) + 
						   ", Column: " + 
						   std::to_string(meta.columnEnd) +
						   ".");
	} else {
		buffer += c;
	}
}

void stateNumeral(std::list<Token>& tokens, State& state, std::string& buffer, TokenMetaData& meta, char c) {
	static bool dot;
	static bool base;

	if(buffer.length() == 1) {
		dot = buffer == ".";
		base = false;
	}

	if(std::isdigit(c) != 0) { 
		buffer += c;
	} else if(c == '.') {
		if(dot) {
			throw std::runtime_error("Error: Unexpected second '.' character in numerical value literal. Line: " +
							std::to_string(meta.lineEnd) + 
							", Column: " + 
							std::to_string(meta.columnEnd) +
							".");
		}
		dot = true;
		buffer += c;
	} else if(c == 'b') {
		if(base) {
			throw std::runtime_error("Error: Unexpected second base sufix character in numerical value literal. Line: " +
							std::to_string(meta.lineEnd) + 
							", Column: " + 
							std::to_string(meta.columnEnd) +
							".");
		}
		base = true;
		buffer += c;
	} else if(std::isalpha(c) != 0) {
		throw std::runtime_error("Error: Unexpected character '" +
						   std::to_string(c) + 
						   "' in numerical value literal. Line: " +
						   std::to_string(meta.lineEnd) + 
						   ", Column: " + 
						   std::to_string(meta.columnEnd) +
						   ".");
	} else {
		emit(tokens, TokenType::NUMERAL, meta, buffer);
		state = State::INIT;
		stateInit(tokens, state, buffer, meta, c);
	}
}

void stateIdent(std::list<Token>& tokens, State& state, std::string& buffer, TokenMetaData& meta, char c) {
	if(std::isalnum(c) != 0 || c == '_') {
		buffer += c;
	} else {
		emit(tokens, TokenType::SYMBOL, meta, buffer);
		state = State::INIT;
		stateInit(tokens, state, buffer, meta, c);
	}
}

void stateSymbol(std::list<Token>& tokens, State& state, std::string& buffer, TokenMetaData& meta, char c) {
	if(std::ispunct(c) != 0) {
		buffer += c;
	} else {
		emit(tokens, TokenType::SYMBOL_CLUSTER, meta, buffer);
		state = State::INIT;
		stateInit(tokens, state, buffer, meta, c);
	}
}

void stateDot(std::list<Token>& tokens, State& state, std::string& buffer, TokenMetaData& meta, char c) {
	if(std::isdigit(c) != 0) {
		state = State::NUMERAL;
		stateNumeral(tokens, state, buffer, meta, c);
	} else {
		state = State::SYMBOL;
		stateSymbol(tokens, state, buffer, meta, c);
	}
}

void stateSlash(std::list<Token>& tokens, State& state, std::string& buffer, TokenMetaData& meta, char c) {
	if(c == '/') {
		buffer.clear();
		state = State::COMMENT_LINE;
	} else if(c == '*') {
		buffer.clear();
		state = State::COMMENT_BLOCK;
	} else {
		state = State::SYMBOL;
		stateSymbol(tokens, state, buffer, meta, c);
	}
}

}

std::list<Token> lex(std::istream& stream) {
	State state = State::INIT;
	std::list<Token> tokens;
	std::string buffer;
	TokenMetaData meta = {.lineStart=1, .columnStart=1, .lineEnd=1, .columnEnd=1};

	tokens.emplace_back(TokenType::START, "", TokenMetaData{});

	for(int c = 0; stream >> c;) {
		switch (state) {
			case State::INIT:
				stateInit(tokens, state, buffer, meta, static_cast<char>(c));
				break;
			case State::COMMENT_LINE:
				stateCommentLine(state, static_cast<char>(c));
				break;
			case State::COMMENT_BLOCK:
				stateCommentBlock(state, buffer, static_cast<char>(c));
				break;
			case State::STRING:
				stateString(tokens, state, buffer, meta, static_cast<char>(c));
				break;
			case State::NUMERAL:
				stateNumeral(tokens, state, buffer, meta, static_cast<char>(c));
				break;
			case State::IDENT:
				stateIdent(tokens, state, buffer, meta, static_cast<char>(c));
				break;
			case State::SYMBOL:
				stateSymbol(tokens, state, buffer, meta, static_cast<char>(c));
				break;
			case State::DOT:
				stateDot(tokens, state, buffer, meta, static_cast<char>(c));
				break;
			case State::SLASH:
				stateSlash(tokens, state, buffer, meta, static_cast<char>(c));
				break;
		}
		if(c == '\n') {
			meta.lineEnd++;
			meta.columnEnd = 1;
		} else {
			meta.columnEnd++;
		}
	}

	tokens.emplace_back(TokenType::END, "", meta);
	return tokens;
}

}
