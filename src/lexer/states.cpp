#include "states.hpp"
#include "tokenizer.hpp"
#include "tokens.hpp"
#include <cctype>
#include <cstddef>
#include <set>
#include <stdexcept>
#include <string>

#define PROCESS(s) void s::process(Tokenizer& t, int c)

namespace elc::state {

PROCESS(Init) {
	if(std::isspace(c) != 0) { return; }
	if(c == '"') { 
		t.add(c); 
		t.changeState<String>();
	} else if(std::isdigit(c) != 0) {
		t.add(c); 
		t.changeState<Numeral>();
	} else if(std::isalpha(c) != 0) {
		t.add(c); 
		t.changeState<Ident>();
	} else if(c == '-') {
		t.add(c); 
		t.changeState<Minus>();
	} else if(c == '.') {
		t.add(c); 
		t.changeState<Dot>();
	} else if(std::ispunct(c) != 0) {
		t.changeState<Symbol>();
		t.repeat();
	} else { 
		std::string err = "Unexpected character '";
		err += static_cast<char>(c);
		err += "' at line " + std::to_string(t.line) + ", column " + std::to_string(t.column);
		throw std::runtime_error(err);
	}
}

PROCESS(String) {
	if(slash) {
		switch(c) {
			case '\'':
				t.add('\'');
				break;
			case 'a':
				t.add('\a');
				break;
			case 'b':
				t.add('\b');
				break;
			case 'f':
				t.add('\f');
				break;
			case 'n':
				t.add('\n');
				break;
			case 'r':
				t.add('\r');
				break;
			case 't':
				t.add('\t');
				break;
			case 'v':
				t.add('\v');
				break;
			default:
				t.add(c);
		}
		slash = false;
	} else if(c == '"') { 
		t.add(c); 
		t.pushToken(TokenType::STRING); 
	} else if(c == '\\') { 
		slash = true;
	} else { 
		t.add(c); 
	}
}

Numeral::Numeral(bool d) : dot(d) {}

PROCESS(Numeral) {
	if(std::isdigit(c) != 0) { 
		t.add(c);
	} else if(c == '.') {
		if(dot) {
			const std::string err = "Second dot symbol in numeral token at line " + std::to_string(t.line) + ", column " + std::to_string(t.column);
			throw std::runtime_error(err);
		}  
		dot = true;
		t.add(c);
	} else if(std::isalpha(c) != 0) {
		std::string err = "Invalid digit '";
		err += static_cast<char>(c);
		err += "' in numeral token at line " + std::to_string(t.line) + ", column " + std::to_string(t.column);
		throw std::runtime_error(err);
	} else {
		t.pushToken(TokenType::NUMERAL);
		t.repeat();
	}
}

const std::set<std::string> keywords = { "fun", "return" };

PROCESS(Ident) {
	if(std::isalnum(c) != 0 || c == '_') {
		t.add(c);
	} else {
		t.pushToken(keywords.contains(t.buffer.str()) ? TokenType::KEYWORD : TokenType::IDENTIFIER);
		t.repeat();
	}
}

const std::set<std::string> symbols = { ";", "(", ")", "[", "]", "{", "}", "->", "=", "+", "-", "==" };

bool resembles(const std::set<std::string>& symbols, const std::string& str) {
	auto it = *symbols.lower_bound(str);
	if(str.size() > it.size()) { return false; }
	for(std::size_t i = 0; i < str.size(); i++) {
		if(str[i] != it[i]) { return false; }
	}
	return true;
}

Symbol::Symbol(bool e) : empty(e) {}

PROCESS(Symbol) {
	std::string symbol = t.buffer.str();
	symbol += static_cast<char>(c);
	const bool isSym = resembles(symbols, symbol);
	if(!isSym && empty) {
		std::string err = "Unexpected symbol '";
		err += symbol;
		err += "' at line " + std::to_string(t.line) + ", column " + std::to_string(t.column);
		throw std::runtime_error(err);
	} 
	if(isSym) {
		t.add(c);
		empty = false;
	} else {
		t.pushToken(TokenType::SYMBOL);
		t.repeat();
	}
}

PROCESS(Minus) {
	if(std::isdigit(c) != 0 || c == '.') {
		t.changeState<Numeral>();
		t.repeat();
	} else {
		t.changeState<Symbol>(false);
		t.repeat();
	}
}

PROCESS(Dot) {
	if(std::isdigit(c) != 0) {
		t.add(c);
		t.changeState<Numeral>(true);
	} else {
		t.changeState<Symbol>();
		t.repeat();
	}
}


}
