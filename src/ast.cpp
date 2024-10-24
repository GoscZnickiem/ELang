#include "ast.hpp"
#include "tokens.hpp"

#include <string>
#include <utility>
#include <memory>

namespace elc::ast {

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
std::pair<int, bool> getBiOperatorData(TokenType token) {
	switch (token) {
		case TokenType::OP_ASSIGN:			return {1, false};
		// other assigns
		case TokenType::OP_OR:				return {2, true};
		case TokenType::OP_XOR:				return {3, true};
		case TokenType::OP_AND:				return {4, true};
		case TokenType::OP_BITOR:			return {5, true};
		case TokenType::OP_BITXOR:			return {6, true};
		case TokenType::OP_BITAND:			return {7, true};
		case TokenType::OP_EQUALS:
		case TokenType::OP_NOTEQUALS:		return {8, true};
		case TokenType::OP_LESS:
		case TokenType::OP_LESSEQUAL:
		case TokenType::OP_GREATER:
		case TokenType::OP_GREATEREQUAL:	return {9, true};
		case TokenType::OP_BITSHIFTLEFT:
		case TokenType::OP_BITSHIFTRIGHT:	return {10, true};
        case TokenType::OP_PLUS:
        case TokenType::OP_MINUS:			return {11, true};
        case TokenType::OP_MULT:
        case TokenType::OP_DIV:
        case TokenType::OP_MODULO:			return {12, true};	
		default: return {-1, false};
	}
}

std::pair<int, int> getUOperatorData(TokenType token) {
	switch (token) {
        case TokenType::OP_MINUS:
		case TokenType::OP_BITNEGATE:
		case TokenType::OP_NEGATE:			return {13, -1};
		default: return {-1, -1};
	}
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)



Numeral::Numeral(std::string v) : value(std::move(v)) {}
std::string Numeral::toString() const {
	return value;
}

Bool::Bool(bool v) : value(v) {}
std::string Bool::toString() const {
	return value ? "true" : "false";
}

Identifier::Identifier(std::string n) : name(std::move(n)) {}
std::string Identifier::toString() const {
	return name;
}



ULeftOperator::ULeftOperator(std::unique_ptr<Expression>&& e, std::string o)
: expr(std::move(e)), op(std::move(o)) {}
ULeftOperator::ULeftOperator(std::unique_ptr<Expression>& e, std::string o)
: expr(std::move(e)), op(std::move(o)) {}
std::string ULeftOperator::toString() const {
	return "(" + op + " " + expr->toString() + ")";
}



BiOperator::BiOperator(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r, std::string o)
: left(std::move(l)), right(std::move(r)), op(std::move(o)) {}
BiOperator::BiOperator(std::unique_ptr<Expression>& l, std::unique_ptr<Expression>& r, std::string o)
: left(std::move(l)), right(std::move(r)), op(std::move(o)) {}
std::string BiOperator::toString() const {
	return "(" + left->toString() + " " + op + " " + right->toString() + ")";
}



FunCall::FunCall(std::unique_ptr<Identifier>&& n, ArgumentList&& args)
: name(std::move(n)), arguments(std::move(args)) {}
FunCall::FunCall(std::unique_ptr<Identifier>& n, ArgumentList& args)
: name(std::move(n)), arguments(std::move(args)) {}
std::string FunCall::toString() const {
	std::string args;
	bool first = true;
	for(const auto& arg : arguments) {
		if(first) { first = false; } else { args += ", "; }
		args += arg->toString();
	}
	return "call " + name->toString() + " with (" + args + ")";
}


Return::Return(std::unique_ptr<Expression>&& e) : expr(std::move(e)) {}
Return::Return(std::unique_ptr<Expression>& e) : expr(std::move(e)) {}
std::string Return::toString() const {
	return "return " + expr->toString();
}



std::string Block::toString() const {
	std::string r;
	for(const auto& i : instructions) {
		r += "\t" + i->toString() + "\n";
	}
	return r;
}



Type::Type(std::string n) : name(std::move(n)) {}
std::string Type::toString() const {
	return name;
}



VarDecl::VarDecl(std::unique_ptr<Type>&& t, std::unique_ptr<Identifier>&& n)
: type(std::move(t)), name(std::move(n)) {}
VarDecl::VarDecl(std::unique_ptr<Type>& t, std::unique_ptr<Identifier>& n) 
: type(std::move(t)), name(std::move(n)) {}
std::string VarDecl::toString() const {
	return "variable " + name->toString() + " of type " + type->toString();
}

VarDeclAssign::VarDeclAssign(std::unique_ptr<Type>&& t, std::unique_ptr<Identifier>&& n, std::unique_ptr<Expression>&& e)
: type(std::move(t)), name(std::move(n)), expr(std::move(e)) {}
VarDeclAssign::VarDeclAssign(std::unique_ptr<Type>& t, std::unique_ptr<Identifier>& n, std::unique_ptr<Expression>& e) 
: type(std::move(t)), name(std::move(n)), expr(std::move(e)) {}
std::string VarDeclAssign::toString() const {
	return "variable " + name->toString() + " of type " + type->toString() + " = " + expr->toString();
}

FunDecl::FunDecl(std::unique_ptr<Identifier>&& n, std::unique_ptr<Type>&& t, ArgumentDeclList&& args, Block&& b)
: name(std::move(n)), returnType(std::move(t)), arguments(std::move(args)), body(std::move(b)) {}
FunDecl::FunDecl( std::unique_ptr<Identifier>& n, std::unique_ptr<Type>& t, ArgumentDeclList& args, Block& b)
: name(std::move(n)), returnType(std::move(t)), arguments(std::move(args)), body(std::move(b)) {}
std::string FunDecl::toString() const {
	std::string args;
	bool first = true;
	for(const auto& arg : arguments) {
		if(first) { first = false; } else { args += ", "; }
		args += arg.first->toString() + " " + arg.second->toString();
	}
	return "function " + name->toString() + " of type (" + args + ") -> " + returnType->toString() + " = {\n" + body.toString() + "}";
}

}
