#include "states.hpp"
#include "tokenizer.hpp"
#include "tokens.hpp"
#include <cctype>
#include <map>
#include <stdexcept>
#include <string>

#define PROCESS(s) void s::process(Tokenizer& t, int c)

namespace elc::state {

void Init::process(Tokenizer& t, int c) {
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
	} else if(c == '.') {
		t.add(c); 
		t.changeState<Dot>();
	} else if(std::ispunct(c) != 0) {
		t.changeState<Symbol>();
		t.repeat();
	} else {
		t.buffer.clear();
		t.pushToken(TokenType::END);
	}
	// else { 
	// 	std::string err = "Unexpected character '";
	// 	err += std::to_string(c);
	// 	err += " ";
	// 	err += static_cast<char>(c);
	// 	err += "' at line " + std::to_string(t.line) + ", column " + std::to_string(t.column);
	// 	throw std::runtime_error(err);
	// }
}

void String::process(Tokenizer& t, int c) {
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
		t.resetState();
	} else if(c == '\\') { 
		slash = true;
	} else if(c == '\n') { 
		return;
	} else { 
		t.add(c);
	}
}

Numeral::Numeral(bool d) : dot(d) {}

void Numeral::process(Tokenizer& t, int c) {
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
		t.resetState();
		t.repeat();
	}
}

const std::map<std::string, TokenType> keywords = {
	{"fun", TokenType::KEY_FUN}, 
	{"return", TokenType::KEY_RETURN} 
};

void Ident::process(Tokenizer& t, int c) {
	if(std::isalnum(c) != 0 || c == '_') {
		t.add(c);
	} else {
		if(keywords.contains(t.buffer)) {
			t.buffer.clear();
			t.pushToken(keywords.at(t.buffer));
		} else {
			t.pushToken(TokenType::IDENTIFIER);
		}
		t.resetState();
		t.repeat();
	}
}

const std::map<std::string, TokenType> symbols = {
	{";", TokenType::SEMICOLON}, 
	{"(", TokenType::PAREN_L},
	{")", TokenType::PAREN_R},
	{"+", TokenType::OP_PLUS},
	{"-", TokenType::OP_MINUS}
};

void Symbol::process(Tokenizer& t, int c) {
	if(std::ispunct(c) != 0 && c != '\"' && (t.buffer.empty() || c != '.')) {
		t.add(c);
		return;
	}
	std::string buffer = t.buffer;
	t.buffer.clear();

	while(!buffer.empty()) {
		std::string longestMatch;
		TokenType tokenType = TokenType::INVALID;
		for (const auto& [mapStr, mapType] : symbols) {
			if (buffer.starts_with(mapStr)) {
				longestMatch = mapStr;
				tokenType = mapType;
			}
		}

		if (tokenType == TokenType::INVALID) {
			std::string err = "Invalid character '";
			err += buffer;
			err += "' could not be interpreted as a symbol at line " + std::to_string(t.line) + ", column " + std::to_string(t.column);
			throw std::runtime_error(err);
		}

		buffer = buffer.substr(longestMatch.length());
		t.pushToken(tokenType);
	}

	t.resetState();
	t.repeat();
}

void Dot::process(Tokenizer& t, int c) {
	if(std::isdigit(c) != 0) {
		t.add(c);
		t.changeState<Numeral>(true);
	} else {
		t.changeState<Symbol>();
		t.repeat();
	}
}


}
