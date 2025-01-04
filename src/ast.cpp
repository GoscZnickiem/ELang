#include "ast.hpp"
#include "tokens.hpp"

#include <string>
#include <utility>
#include <memory>
#include <variant>

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



NumeralC::NumeralC(std::string v) : value(std::move(v)) {}
std::string NumeralC::toString() const {
	return value;
}

BoolC::BoolC(bool v) : value(v) {}
std::string BoolC::toString() const {
	return value ? "true" : "false";
}

IdentifierC::IdentifierC(std::string n) : name(std::move(n)) {}
std::string IdentifierC::toString() const {
	return name;
}



ULeftOperatorC::ULeftOperatorC(Expression&& e, std::string o)
: expr(std::move(e)), op(std::move(o)) {}
ULeftOperatorC::ULeftOperatorC(Expression& e, std::string o)
: expr(std::move(e)), op(std::move(o)) {}
std::string ULeftOperatorC::toString() const {
	return "(" + op + " " + astToString(expr) + ")";
}



BiOperatorC::BiOperatorC(Expression&& l, Expression&& r, std::string o)
: left(std::move(l)), right(std::move(r)), op(std::move(o)) {}
BiOperatorC::BiOperatorC(Expression& l, Expression& r, std::string o)
: left(std::move(l)), right(std::move(r)), op(std::move(o)) {}
std::string BiOperatorC::toString() const {
	return "(" + astToString(left) + " " + op + " " + astToString(right) + ")";
}



FunCallC::FunCallC(Identifier&& n, ArgumentList&& args)
: name(std::move(n)), arguments(std::move(args)) {}
FunCallC::FunCallC(Identifier& n, ArgumentList& args)
: name(std::move(n)), arguments(std::move(args)) {}
std::string FunCallC::toString() const {
	std::string args;
	bool first = true;
	for(const auto& arg : arguments) {
		if(first) { first = false; } else { args += ", "; }
		args += astToString(arg);
	}
	return "call " + name->toString() + " with (" + args + ")";
}


ReturnC::ReturnC(Expression&& e) : expr(std::move(e)) {}
ReturnC::ReturnC(Expression& e) : expr(std::move(e)) {}
std::string ReturnC::toString() const {
	return std::visit( [&](auto& e) {
		return "return " + (e == nullptr ? "" : e->toString());
	} , expr);
}



std::string BlockC::toString() const {
	std::string r;
	for(const auto& i : instructions) {
		r += "\t" + astToString(i) + "\n";
	}
	return "{\n" + r + "}";
}



TypeIdentC::TypeIdentC(std::string n) : name(std::move(n)) {}
std::string TypeIdentC::toString() const {
	return name;
}



VarDeclC::VarDeclC(Type&& t, Identifier&& n)
: type(std::move(t)), name(std::move(n)) {}
VarDeclC::VarDeclC(Type& t, Identifier& n) 
: type(std::move(t)), name(std::move(n)) {}
std::string VarDeclC::toString() const {
	return "variable " + name->toString() + " of type " + astToString(type);
}

VarDeclAssignC::VarDeclAssignC(Type&& t, Identifier&& n, Expression&& e)
: type(std::move(t)), name(std::move(n)), expr(std::move(e)) {}
VarDeclAssignC::VarDeclAssignC(Type& t, Identifier& n, Expression& e) 
: type(std::move(t)), name(std::move(n)), expr(std::move(e)) {}
std::string VarDeclAssignC::toString() const {
	return "variable " + name->toString() + " of type " + astToString(type) + " = " + astToString(expr);
}

FunDeclC::FunDeclC(Identifier&& n, Type&& t, ArgumentDeclList&& args, Block&& b)
: name(std::move(n)), returnType(std::move(t)), arguments(std::move(args)), body(std::move(b)) {}
FunDeclC::FunDeclC(Identifier& n, Type& t, ArgumentDeclList& args, Block& b)
: name(std::move(n)), returnType(std::move(t)), arguments(std::move(args)), body(std::move(b)) {}
std::string FunDeclC::toString() const {
	std::string args;
	bool first = true;
	for(const auto& arg : arguments) {
		if(first) { first = false; } else { args += ", "; }
		args += astToString(arg.first) + " " + arg.second->toString();
	}
	return "function " + name->toString() + " of type (" + args + ") -> " + astToString(returnType) + " = " + body->toString();
}

}
