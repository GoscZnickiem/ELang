#include "lexer.hpp"
#include "tokens.hpp"
#include <cctype>
#include <cstddef>
#include <iostream>
#include <istream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <utility>

namespace elc {

class Tokenizer {
public:
	explicit Tokenizer(std::istream* src) : source(src) {}

	std::vector<Token> tokenize() {
		auto c = source->get();
		auto add = [this, &c](){
			buffer << static_cast<char>(c);
		};
		while(!source->eof()) {
			switch(state) {
				case State::Init:
					if(std::isspace(c) != 0) { }
					else if(c == '"') { add(); state = State::String; }
					else if(std::isdigit(c) != 0) { add(); state = State::Numeral10; }
					else if(std::isalpha(c) != 0) { add(); state = State::Ident; }
					else { throw std::runtime_error("Unexpected character at line " + std::to_string(line) + ", column " + std::to_string(column)); }
					break;
				case State::String:
					if(c == '"') { add(); pushToken(TokenType::STRING); }
					else if(c == '\\') { add(); state = State::StringAll; }
					else { add(); }
					break;
				case State::StringAll:
					add();
					state = State::String;
					break;
				case State::Numeral10:
					if(std::isdigit(c) != 0) { add(); }
					else if(std::isalpha(c) != 0) { throw std::runtime_error("Invalid digit '" + std::to_string(c) + "' in numeral token at line " + std::to_string(line) + ", column " + std::to_string(column)); }
					else { pushToken(TokenType::NUMERAL); continue; }
					break;
				case State::Ident:
					if(std::isalnum(c) != 0 || c == '_') { add(); }
					else { pushToken(TokenType::IDENTIFIER); continue; }
					break;
			}

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

	void pushToken(TokenType type) {
		tokens.emplace_back(type, line, column - buffer.str().size(), buffer.str().size(), buffer.str());
		buffer.str("");
		buffer.clear();
		state = State::Init;
	}
private:
	enum class State {
		Init, String, StringAll, Numeral10, Ident
	} state = State::Init;
	std::istream* source;
	std::vector<Token> tokens;
	std::stringstream buffer;
	std::size_t line = 1;
	std::size_t column = 1;
};


std::vector<Token> tokenize(std::istream& stream) {
	Tokenizer t(&stream);
	return t.tokenize();
}

}
