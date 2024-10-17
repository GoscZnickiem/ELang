#include "parser.hpp"

#include "ast.hpp"
#include "tokens.hpp"
#include <cstddef>
#include <map>
#include <memory>
#include <sstream>
#include <utility>
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

	static inline const std::map<TokenType, std::pair<int, bool>> operatorPrec = {
		{TokenType::OP_PLUS, {3, true}},
		{TokenType::OP_MINUS, {3, true}},
		{TokenType::OP_MULT, {5, true}},
		{TokenType::OP_DIV, {5, true}}
	};

	static inline const std::map<TokenType, ast::BiOp> tokenToBiOp = {
		{TokenType::OP_PLUS, ast::BiOp::PLUS},
		{TokenType::OP_MINUS, ast::BiOp::MINUS},
		{TokenType::OP_MULT, ast::BiOp::MULT},
		{TokenType::OP_DIV, ast::BiOp::DIV},
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
			eat();
			auto op = token.type;
			auto t1 = Exp(q);
			t = std::make_unique<ast::BiOperator>(t, t1, tokenToBiOp.at(op));
		}
		return t;
	}

	std::unique_ptr<ast::Expression> P() {
		if(next() == TokenType::OP_MINUS) {
			const int q = 5; //precedence(next());
			eat();
			auto t = Exp(q);
			return std::make_unique<ast::ULeftOperator>(t, ast::ULeftOp::MINUS);
		}
		if(next() == TokenType::PAREN_L) {
			eat();
			auto t = Exp(0);
			expect(TokenType::PAREN_R);
			return t;
		}
		if(next() == TokenType::NUMERAL) {
			eat();
			return std::make_unique<ast::Numeral>(token.data);
		}
		eat();
		std::stringstream ss;
		ss << token;
		throw std::runtime_error("some error in P. I encountered " + ss.str() + ")");
	}

	ast::Unit parse() {
		ast::Unit u;
		while(next() != TokenType::END) {
			auto e = Exp(0);
			u.expressions.push_back(std::move(e));
			expect(TokenType::SEMICOLON);
		}
		expect(TokenType::END);
		return u;
	}
};



ast::Unit parse(std::vector<Token> &tokens) {
	std::cout << "parsing...\n";
	Parser parser(tokens);
	return parser.parse();
}

}
