#include "parser.hpp"

#include "ast.hpp"
#include "tokens.hpp"

#include <cstddef>
#include <memory>
#include <sstream>
#include <vector>
#include <iostream>
#include <stdexcept>

namespace elc {

struct Parser {
	explicit Parser(std::vector<Token>& t) : tokens(&t), token((*tokens)[0]) {}
	std::vector<Token>* tokens;
	std::size_t index{};
	Token token;

	[[nodiscard]] TokenType next(std::size_t i = 1) const {
		return (*tokens)[index + i].type;
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

	std::unique_ptr<ast::Expression> Exp(int p) {
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

	std::unique_ptr<ast::Expression> Exp() {
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
		if(next() == TokenType::BOOL) {
			eat();
			return std::make_unique<ast::Bool>(token.data == "true");
		}
		if(next() == TokenType::IDENTIFIER) {
			eat();
			return std::make_unique<ast::Variable>(token.data);
		}
		eat();
		std::stringstream ss;
		ss << token;
		throw std::runtime_error("some error in Exp. I encountered " + ss.str() + ")");
	}

	std::unique_ptr<ast::Instruction> Instr() {
		if(next() == TokenType::IDENTIFIER && next(2) == TokenType::IDENTIFIER) {
			eat();
			auto type = std::make_unique<ast::Type>(token.data);
			eat();
			auto name = std::make_unique<ast::Variable>(token.data);
			if(next() == TokenType::OP_ASSIGN) {
				eat();
				auto exp = Exp(0);
				expect(TokenType::SEMICOLON);
				return std::make_unique<ast::DeclAssign>(type, name, exp);
			}
			expect(TokenType::SEMICOLON);
			return std::make_unique<ast::Declaration>(type, name);
		}
		auto e = Exp(0);
		expect(TokenType::SEMICOLON);
		return e;
	}

	ast::Block parse() {
		ast::Block u;
		while(next() != TokenType::END) {
			u.instructions.push_back(Instr());
		}
		expect(TokenType::END);
		return u;
	}
};



ast::Block parse(std::vector<Token> &tokens) {
	std::cout << "parsing...\n";
	Parser parser(tokens);
	return parser.parse();
}

}
