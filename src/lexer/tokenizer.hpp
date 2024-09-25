#ifndef _ELC_LEXER_TOKENIZER_
#define _ELC_LEXER_TOKENIZER_

#include "tokens.hpp"
#include "states.hpp"
#include <cctype>
#include <cstddef>
#include <istream>
#include <memory>
#include <sstream>
#include <vector>

namespace elc {

class State;

class Tokenizer {
public:
	explicit Tokenizer(std::istream* src);

	std::vector<Token> tokenize();
	void pushToken(TokenType type);
	void add(int c);
	void repeat();

	template<typename T, typename... Args>
	void changeState(Args... args) {
		state = std::make_unique<T>(args...);
	}

	std::istream* source;
	std::vector<Token> tokens;
	std::unique_ptr<State> state;
	std::stringstream buffer;
	std::size_t line = 1;
	std::size_t column = 1;
	bool rep = false;
};

}

#endif // !_ELC_LEXER_TOKENIZER_
