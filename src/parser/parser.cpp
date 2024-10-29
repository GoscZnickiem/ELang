#include "parser.hpp"

#include "ast.hpp"
#include "tokens.hpp"
#include "help/visitor.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>
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
			throw std::runtime_error("error (at line: " 
							+ std::to_string((*tokens)[index+1].line) + 
							", column: " + std::to_string((*tokens)[index+1].column) + 
							"): expected " + tokenTypeToString(type) + 
							" but " + tokenTypeToString(next()) + " encountered");
		}
		eat();
	}
	void throwError() {

	}

	ast::Expression Exp(int p) {
		auto t = Exp();
		while(true) {
			const auto [prec, leftAssoc] = ast::getBiOperatorData(next());
			if(prec == -1 || prec < p) break;
			eat();
			auto op = token.data;
			auto t1 = Exp(leftAssoc ? prec + 1 : prec);
			t = std::make_unique<ast::BiOperatorC>(t, t1, op);
		}
		return t;
	}

	ast::Expression Exp() {
		if(next() == TokenType::OP_MINUS) {
			const auto [q, _] = ast::getUOperatorData(next());
			eat();
			auto op = token.data;
			auto t = Exp(q);
			return std::make_unique<ast::ULeftOperatorC>(t, op);
		}
		if(next() == TokenType::PAREN_L) {
			eat();
			auto t = Exp(0);
			expect(TokenType::PAREN_R);
			return t;
		}
		if(next() == TokenType::NUMERAL) {
			eat();
			return std::make_unique<ast::NumeralC>(token.data);
		}
		if(next() == TokenType::BOOL) {
			eat();
			return std::make_unique<ast::BoolC>(token.data == "true");
		}
		if(next() == TokenType::IDENTIFIER) {
			eat();
			if(next() == TokenType::PAREN_L) {
				auto name = std::make_unique<ast::IdentifierC>(token.data);
				auto args = ArgList();
				return std::make_unique<ast::FunCallC>(name, args);
			}
			return std::make_unique<ast::IdentifierC>(token.data);
		}
		eat();
		throw std::runtime_error("Error: expected expression. line: " + std::to_string(token.line) + ", column " + std::to_string(token.column));
	}

	ast::ArgumentDeclList ArgDeclList() {
		ast::ArgumentDeclList list;
		expect(TokenType::PAREN_L);
		while(next() != TokenType::PAREN_R) {
			expect(TokenType::IDENTIFIER);
			auto type = std::make_unique<ast::TypeC>(token.data);
			expect(TokenType::IDENTIFIER);
			auto name = std::make_unique<ast::IdentifierC>(token.data);
			list.emplace_back(std::move(type), std::move(name));
			if(next() == TokenType::COMMA) {
				eat();
				continue;
			}
			break;
		}
		expect(TokenType::PAREN_R);
		return std::move(list);
	}

	ast::ArgumentList ArgList() {
		ast::ArgumentList list;
		expect(TokenType::PAREN_L);
		while(next() != TokenType::PAREN_R) {
			list.emplace_back(Exp(0));
			if(next() == TokenType::COMMA) {
				eat();
				continue;
			}
			break;
		}
		expect(TokenType::PAREN_R);
		return std::move(list);
	}

	ast::Block Block() {
		ast::Block block = std::make_unique<ast::BlockC>();
		expect(TokenType::BRACE_L);
		while(next() != TokenType::BRACE_R) {
			block->instructions.emplace_back(Instr()); 
		}
		eat();
		return std::move(block);
	}

	ast::Instruction Instr() {
		if(next() == TokenType::IDENTIFIER && next(2) == TokenType::IDENTIFIER) {
			eat();
			ast::Type type = std::make_unique<ast::TypeC>(token.data);
			eat();
			ast::Identifier name = std::make_unique<ast::IdentifierC>(token.data);
			if(next() == TokenType::OP_ASSIGN) {
				eat();
				auto exp = Exp(0);
				expect(TokenType::SEMICOLON);
				return std::make_unique<ast::VarDeclAssignC>(type, name, exp);
			}
			expect(TokenType::SEMICOLON);
			return std::make_unique<ast::VarDeclC>(type, name);
		}
		if(next() == TokenType::KEY_FUN) {
			eat();
			expect(TokenType::IDENTIFIER);
			auto name = std::make_unique<ast::IdentifierC>(token.data);
			auto args = ArgDeclList();
			expect(TokenType::ARROW_RIGHT);
			expect(TokenType::IDENTIFIER);
			auto returnType = std::make_unique<ast::TypeC>(token.data);
			auto block = Block();
			return std::make_unique<ast::FunDeclC>(name, returnType, args, block);
		}
		if(next() == TokenType::KEY_RETURN) {
			eat();
			if(next() == TokenType::SEMICOLON) {
				eat();
				return std::make_unique<ast::ReturnC>();
			}
			auto e = Exp(0);
			expect(TokenType::SEMICOLON);
			return std::make_unique<ast::ReturnC>(std::move(e));
		}
		auto e = Exp(0);
		expect(TokenType::SEMICOLON);
		return std::visit(
			[&](auto& arg) -> ast::Instruction { return std::move(arg); }
		, e);
	}

	ast::Declaration TopInstr() {
		auto i = Instr();
		ast::Declaration decl;
		std::visit(visitor{
			[&](ast::VarDecl& i) { decl = std::move(i); },
			[&](ast::VarDeclAssign& i) { decl = std::move(i); },
			[&](ast::FunDecl& i) { decl = std::move(i); },
			[&]([[maybe_unused]] auto& i) { 
				eat();
				throw std::runtime_error("Error: expected global symbol declaration. line: " + std::to_string(token.line) + ", column " + std::to_string(token.column));
			}
		}, i);
		return decl;
	}


	ast::Unit parse() {
		ast::Unit u;
		while(next() != TokenType::END) {
			u.globals.push_back(TopInstr()); 
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
