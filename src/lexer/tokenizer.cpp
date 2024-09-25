#include "tokenizer.hpp"
#include "lexer/states.hpp"

#include "tokens.hpp"
#include <cctype>
#include <iostream>
#include <istream>
#include <sstream>
#include <vector>
#include <utility>

namespace elc {

Tokenizer::Tokenizer(std::istream* src) : source(src) {
	changeState<state::Init>();
}

std::vector<Token> Tokenizer::tokenize() {
	auto c = source->get();
	while(!source->eof()) {
		state->process(*this, c);
		if(rep) { rep = false; continue; }
		if(c == '\n') {
			line++;
			column = 1;
		} else {
			column++;
		}
		c = source->get();
	}
	return std::move(tokens);
}

void Tokenizer::pushToken(TokenType type) {
	tokens.emplace_back(type, line, column - buffer.str().size(), buffer.str().size(), buffer.str());
	buffer.str("");
	buffer.clear();
	changeState<state::Init>();
}

void Tokenizer::add(int c) {
	buffer << static_cast<char>(c);
}

void Tokenizer::repeat() {
	rep = true;
}

}
