#include "parser.hpp"

#include "data/ast.hpp"
#include "data/astBuilding.hpp"
#include "data/tokens.hpp"
#include "help/variant.hpp"
#include "help/visitor.hpp"

#include <cstddef>
#include <iterator>
#include <list>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <stdexcept>

namespace elc {
using ast::build::Context;

namespace {

std::list<Token>::iterator findRightBrace(std::list<Token>::iterator it) {
	for(std::size_t braceBalance = 0; true; it++) {
		if(it->type == TokenType::END) {
			throw std::runtime_error("Error: expected '}'. Line: " +
							std::to_string(it->metadata.lineEnd) + 
							", Column: " + 
							std::to_string(it->metadata.columnEnd) +
							".");
		}
		if(it->type == TokenType::BRACE_L) {
			braceBalance++;
		} else if(it->type == TokenType::BRACE_R) {
			braceBalance--;
			if(braceBalance == 0) {
				return ++it;
			}
			if(braceBalance < 0) {
				throw std::runtime_error("Error: unexpected '}' encountered. Line: " +
							 std::to_string(it->metadata.lineEnd) + 
							 ", Column: " + 
							 std::to_string(it->metadata.columnEnd) +
							 ".");
			}
		}
	}
}

std::list<Token>::iterator findEndOfDeclaration(std::list<Token>& tokens) {
	auto it = tokens.begin();
	while(it->type != TokenType::SEMICOLON) {
		if(it->type == TokenType::BRACE_L) {
			it = findRightBrace(it);
		} else {
			it++;
		}
	}
	return ++it;
}

ast::build::Stub splitStub(std::list<Token>& tokens) {
	ast::build::Namespace stub;
	stub.tokens.splice(stub.tokens.end(), tokens, tokens.begin(), findEndOfDeclaration(tokens));
	return stub;
}

void identifyNamespaceContents(ast::build::Namespace& space, Context& context) {
	auto& tokens = space.tokens;
	while(!tokens.empty()) {
		auto stub = splitStub(tokens);

	}
}

}

std::vector<ast::Declaration> parse(std::list<Token>& tokens) {
	Context context;

	ast::build::Namespace global;
	global.name = "";
	global.tokens = std::move(tokens);
	context.currentNamespace = &global;
	identifyNamespaceContents(global, context);


}

// Parser::Parser(CompilationUnit* parent) : root(parent) { }
//
// struct Parser {
// 	explicit Parser(std::vector<Token>& t) : tokens(&t), token((*tokens)[0]) {}
// 	std::vector<Token>* tokens;
// 	std::size_t index{};
// 	Token token;
//
// 	[[nodiscard]] TokenType next(std::size_t i = 1) const {
// 		return (*tokens)[index + i].type;
// 	}
// 	void eat() {
// 		if(next() != TokenType::END) {
// 			token = (*tokens)[++index];
// 		}
// 	}
// 	void eat(TokenType type) {
// 		if(next() != type) {
// 			throw std::runtime_error("error (at line: " 
// 							+ std::to_string((*tokens)[index+1].line) + 
// 							", column: " + std::to_string((*tokens)[index+1].column) + 
// 							"): expected " + tokenTypeToString(type) + 
// 							" but " + tokenTypeToString(next()) + " encountered");
// 		}
// 		eat();
// 	}
// 	void revert(std::size_t i) {
// 		index = i;
// 		token = (*tokens)[index];
// 	}
//
// 	ast::Expression Exp(int p) {
// 		auto t = Exp();
// 		while(true) {
// 			const auto [prec, leftAssoc] = ast::getBiOperatorData(next());
// 			if(prec == -1 || prec < p) break;
// 			eat();
// 			auto op = token.data;
// 			auto t1 = Exp(leftAssoc ? prec + 1 : prec);
// 			t = std::make_unique<ast::BiOperatorC>(t, t1, op);
// 		}
// 		return t;
// 	}
//
// 	ast::Expression Exp() {
// 		if(next() == TokenType::OP_MINUS) {
// 			const auto [q, _] = ast::getUOperatorData(next());
// 			eat();
// 			auto op = token.data;
// 			auto t = Exp(q);
// 			return std::make_unique<ast::ULeftOperatorC>(t, op);
// 		}
// 		if(next() == TokenType::PAREN_L) {
// 			eat();
// 			auto t = Exp(0);
// 			eat(TokenType::PAREN_R);
// 			return t;
// 		}
// 		if(next() == TokenType::NUMERAL) {
// 			eat();
// 			return std::make_unique<ast::NumeralC>(token.data);
// 		}
// 		if(next() == TokenType::BOOL) {
// 			eat();
// 			return std::make_unique<ast::BoolC>(token.data == "true");
// 		}
// 		if(next() == TokenType::IDENTIFIER) {
// 			eat();
// 			if(next() == TokenType::PAREN_L) {
// 				auto name = std::make_unique<ast::IdentifierC>(token.data);
// 				auto args = ArgList();
// 				return std::make_unique<ast::FunCallC>(name, args);
// 			}
// 			return std::make_unique<ast::IdentifierC>(token.data);
// 		}
// 		eat();
// 		throw std::runtime_error("Error: expected expression. line: " + std::to_string(token.line) + ", column " + std::to_string(token.column));
// 	}
//
// 	std::optional<ast::Type> Type(std::size_t size) {
// 		if(next() == TokenType::IDENTIFIER && size == 1) {
// 			eat();
// 			return std::make_unique<ast::IdentifierC>(token.data);
// 		}
// 		return std::nullopt;
// 	}
//
// 	ast::ArgumentDeclList ArgDeclList() {
// 		ast::ArgumentDeclList list;
// 		eat(TokenType::PAREN_L);
// 		if(next() != TokenType::PAREN_R) {
// 		loop:
// 			auto checkpoint = index;
// 			std::size_t size = 1;
// 			for(auto type = Type(size); type.has_value(); type = Type(++size)) {
// 				if(next() == TokenType::IDENTIFIER && (next(2) == TokenType::COMMA || next(2) == TokenType::PAREN_R)) {
// 					eat();
// 					auto name = std::make_unique<ast::IdentifierC>(token.data);
// 					list.emplace_back(std::move(type.value()), std::move(name));
// 					if(next() == TokenType::COMMA) {
// 						eat();
// 						goto loop;
// 					}
// 					goto end;
// 				}
// 				revert(index);
// 			}
// 			throw std::runtime_error("Error: expected argument declarator or ')'. line: " + std::to_string(token.line) + ", column " + std::to_string(token.column));
// 		}
// 	end:
// 		eat(TokenType::PAREN_R);
// 		return std::move(list);
// 	}
//
// 	ast::ArgumentList ArgList() {
// 		ast::ArgumentList list;
// 		eat(TokenType::PAREN_L);
// 		while(next() != TokenType::PAREN_R) {
// 			list.emplace_back(Exp(0));
// 			if(next() == TokenType::COMMA) {
// 				eat();
// 				continue;
// 			}
// 			break;
// 		}
// 		eat(TokenType::PAREN_R);
// 		return std::move(list);
// 	}
//
// 	ast::Block Block() {
// 		ast::Block block = std::make_unique<ast::BlockC>();
// 		eat(TokenType::BRACE_L);
// 		const Token temp = token;
// 		while(next() != TokenType::BRACE_R) {
// 			if(next() == TokenType::END) {
// 				throw std::runtime_error("Error: Brace at line: " + std::to_string(temp.line) + ", column " + std::to_string(temp.column) + "isn't closed.");
// 			}
// 			block->instructions.emplace_back(Instr()); 
// 		}
// 		eat();
// 		return std::move(block);
// 	}
//
// 	ast::Instruction Instr() {
// 		if(next() == TokenType::KEY_FUN) {
// 			eat();
// 			eat(TokenType::IDENTIFIER);
// 			auto name = std::make_unique<ast::IdentifierC>(token.data);
// 			auto args = ArgDeclList();
// 			eat(TokenType::ARROW_RIGHT);
// 			eat(TokenType::IDENTIFIER);
// 			auto returnType = std::make_unique<ast::IdentifierC>(token.data);
// 			auto block = Block();
// 			return std::make_unique<ast::FunDeclC>(std::move(name), std::move(returnType), std::move(args), std::move(block));
// 		}
// 		if(next() == TokenType::KEY_RETURN) {
// 			eat();
// 			if(next() == TokenType::SEMICOLON) {
// 				eat();
// 				return std::make_unique<ast::ReturnC>();
// 			}
// 			auto e = Exp(0);
// 			eat(TokenType::SEMICOLON);
// 			return std::make_unique<ast::ReturnC>(std::move(e));
// 		}
// 		if(next() == TokenType::BRACE_L) {
// 			return Block();
// 		}
//
// 		const auto checkpoint = index;
// 		std::size_t size = 1;
// 		for(auto typeOpt = Type(size); typeOpt.has_value(); typeOpt = Type(++size)) {
// 			ast::Type type = std::move(typeOpt.value());
// 			if(next() == TokenType::IDENTIFIER) {
// 				eat();
// 				ast::Identifier name = std::make_unique<ast::IdentifierC>(token.data);
// 				if(next() == TokenType::OP_ASSIGN) {
// 					eat();
// 					auto exp = Exp(0);
// 					eat(TokenType::SEMICOLON);
// 					return std::make_unique<ast::VarDeclAssignC>(type, name, exp);
// 				}
// 				if(next() == TokenType::SEMICOLON) {
// 					eat();
// 					return std::make_unique<ast::VarDeclC>(type, name);
// 				}
// 			}
// 			revert(checkpoint);
// 		}
//
// 		auto e = Exp(0);
// 		eat(TokenType::SEMICOLON);
//
// 		return std::visit(
// 			[&](auto& arg) -> ast::Instruction { return std::move(arg); }
// 		, e);
// 	}
//
// 	ast::Declaration TopInstr() {
// 		auto i = Instr();
// 		ast::Declaration decl;
// 		std::visit(visitor{
// 			[&](ast::VarDecl& i) { decl = std::move(i); },
// 			[&](ast::VarDeclAssign& i) { decl = std::move(i); },
// 			[&](ast::FunDecl& i) { decl = std::move(i); },
// 			[&]([[maybe_unused]] auto& i) { 
// 				eat();
// 				throw std::runtime_error("Error: expected global symbol declaration. line: " + std::to_string(token.line) + ", column " + std::to_string(token.column));
// 			}
// 		}, i);
// 		return decl;
// 	}
//
//
// 	ast::Unit parse() {
// 		ast::Unit u;
// 		while(next() != TokenType::END) {
// 			u.globals.push_back(TopInstr()); 
// 		}
// 		eat(TokenType::END);
// 		return u;
// 	}
// };
//
//
//
// ast::Unit parse(std::vector<Token> &tokens) {
// 	Parser parser(tokens);
// 	return parser.parse();
// }

} // namespace elc
