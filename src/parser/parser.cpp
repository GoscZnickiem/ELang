#include "parser.hpp"

#include "ast.hpp"
#include "tokens.hpp"
#include <cstddef>
#include <map>
#include <memory>
#include <vector>
#include <iostream>
#include <stdexcept>

namespace elc {

struct Parser {
	explicit Parser(std::vector<Token>& t) : tokens(&t), token((*tokens)[0]) {}
	std::vector<Token>* tokens;
	std::size_t index{};
	Token token;

	[[nodiscard]] TokenType next() const {
		return (*tokens)[index + 1].type;
	}
	void eat() {
		if(next() != TokenType::END) {
			token = (*tokens)[++index];
		}
	}
	void expect(TokenType type) {
		if(next()!= type) {
			throw std::runtime_error("error: expected " + tokenTypeToString(type) + " but " + tokenTypeToString(next()) + " encountered");
		}
		eat();
	}

	std::unique_ptr<ast::Expression> P() {

	}

	static inline const std::map<TokenType, std::pair<int, bool>> operatorPrec = {
		{TokenType::OP_PLUS, {2, true}}
	};

	static int precedence(TokenType token) {
		if(operatorPrec.contains(token)) {
			return operatorPrec.at(token).first;
		}
		return -1;
	}

	static bool leftAssoc(TokenType token) {
		return operatorPrec.at(token).second;
	}

	std::unique_ptr<ast::Expression> Exp(int p) {
		auto t = P();
		while(true) {
			const int prec = precedence(next());
			if(prec == -1 || prec < p) { break; }
			const int q = leftAssoc(next()) ? prec + 1 : prec;
			const auto t1 = Exp(q);
			t = std::make_unique<ast::BiOperator>(t, t1, "a");
		}
		return t;
	}

	ast::Unit parse() {
		ast::Unit u;
		while(next()!= TokenType::END) {
			u.expressions.push_back({});
			expect(TokenType::SEMICOLON);
		}
		expect(TokenType::END);
		return u;
	}
};



ast::Unit parse(std::vector<Token> &tokens) {
	Parser parser(tokens);
	return parser.parse();
}

}
