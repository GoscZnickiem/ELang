#include "lexer.hpp"
#include "data/tokens.hpp"
#include <istream>
#include <list>
#include <string>

namespace elc {

enum class State {
	INIT, COMMENT_LINE, COMMENT_BLOCK,

	STRING, NUMERAL, IDENT, SYMBOL, 

	DOT
};

struct MetaData {
	std::size_t startLine;
	std::size_t startColumn;
	std::size_t line;
	std::size_t column;
};

namespace {

void emit(std::list<Token>& tokens, std::string& data, TokenType type, MetaData& meta) {
	tokens.emplace_back(type, meta.startLine, meta.startColumn, data);
	data.clear();
	meta.startLine = meta.line;
	meta.startColumn = meta.column;
}

void emit(std::list<Token>& tokens, TokenType type, MetaData& meta) {
	tokens.emplace_back(type, meta.startLine, meta.startColumn, "");
	meta.startLine = meta.line;
	meta.startColumn = meta.column;
}

void stateInit(std::list<Token>& tokens, State& state, std::string& buffer, MetaData& meta, char c) {
	if(std::isspace(c) != 0) {
		meta.startLine = meta.line;
		meta.startColumn = meta.column;
		return;
	}
	if(c == '"') { 
		state = State::STRING;
	} else if(std::isdigit(c) != 0) {
		buffer += c;
		state = State::NUMERAL;
	} else if(std::isalpha(c) != 0) {
		buffer += c;
		state = State::IDENT;
	} else if(c == '.') {
		buffer += c;
		state = State::DOT;
	} else if(c == '{') {
		emit(tokens, TokenType::BRACE_L, meta);
	}
}

}

std::list<Token> tokenize(std::istream& stream) {
	State state = State::INIT;
	std::list<Token> tokens;
	std::string buffer;
	MetaData meta = {.startLine=1, .startColumn=1, .line=1, .column=1};

	tokens.emplace_back(TokenType::START, 0, 0, "");

	for(int c = 0; stream >> c;) {
		switch (state) {
			case State::INIT:
				stateInit(tokens, state, buffer, meta, static_cast<char>(c));
				break;
			case State::COMMENT_LINE:
				break;
			case State::COMMENT_BLOCK:
				break;
			case State::STRING:
				break;
			case State::NUMERAL:
				break;
			case State::IDENT:
				break;
			case State::SYMBOL:
				break;
			case State::DOT:
				break;
		}
	}

	tokens.emplace_back(TokenType::END, line, column, "");
	return tokens;
}

}
