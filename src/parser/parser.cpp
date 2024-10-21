#include "parser.hpp"

#include "ast.hpp"
#include "tokens.hpp"

#include <cstddef>
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
		if(next() != type) {
			throw std::runtime_error("error: expected " + tokenTypeToString(type) + " but " + tokenTypeToString(next()) + " encountered");
		}
		eat();
	}

	std::unique_ptr<ast::NumericalExpression> Exp(int p) {
		auto t = Exp();
		while(true) {
			const auto [prec, leftAssoc] = ast::getBiOperatorData(next());
			if(prec == -1 || prec < p) { break; }
			eat();
			auto op = token.data;
			auto t1 = Exp(leftAssoc ? prec + 1 : prec);
			t = std::make_unique<ast::BiOperator>(t, t1, op);
		}
		return t;
	}

	std::unique_ptr<ast::NumericalExpression> Exp() {
		if(next() == TokenType::OP_MINUS) {
			const auto [q, _] = ast::getUOperatorData(next());
			eat();
			auto op = token.data;
			auto t = Exp(q);
			return std::make_unique<ast::ULeftOperator>(t, op);
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
		throw std::runtime_error("some error in Exp. I encountered " + ss.str() + ")");
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
