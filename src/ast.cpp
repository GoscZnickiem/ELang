#include "ast.hpp"
#include "tokens.hpp"

#include <string>
#include <utility>
#include <memory>

namespace elc::ast {

std::pair<int, bool> getBiOperatorData(TokenType token) {
	switch (token) {
        case TokenType::OP_PLUS:
        case TokenType::OP_MINUS:	return {3, true};
        case TokenType::OP_MULT:
        case TokenType::OP_DIV:		return {5, true};	// NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
		default: return {-1, false};
	}
}

std::pair<int, int> getUOperatorData(TokenType token) {
	switch (token) {
        case TokenType::OP_MINUS:	return {4, -1};
		default: return {-1, -1};
	}
}



std::string Declaration::toString() const {
	return "Declare " + name + " of type " + type;
}

Declaration::Declaration(std::string t, std::string n) : type(std::move(t)), name(std::move(n)) {}



std::string Numeral::toString() const {
	return value;
}

Numeral::Numeral(std::string v) : value(std::move(v)) {}



std::string ULeftOperator::toString() const {
	return "(" + op + " " + expr->toString() + ")";
}

ULeftOperator::ULeftOperator(std::unique_ptr<NumericalExpression>&& e, std::string o)
	: expr(std::move(e)), op(std::move(o)) {}

ULeftOperator::ULeftOperator(std::unique_ptr<NumericalExpression>& e, std::string o)
	: expr(std::move(e)), op(std::move(o)) {}



std::string BiOperator::toString() const {
	return "(" + left->toString() + " " + op + " " + right->toString() + ")";
}

BiOperator::BiOperator(std::unique_ptr<NumericalExpression>&& l, std::unique_ptr<NumericalExpression>&& r, std::string o)
	: left(std::move(l)), right(std::move(r)), op(std::move(o)) {}

BiOperator::BiOperator(std::unique_ptr<NumericalExpression>& l, std::unique_ptr<NumericalExpression>& r, std::string o)
	: left(std::move(l)), right(std::move(r)), op(std::move(o)) {}



std::string Bool::toString() const {
	return value ? "true" : "false";
}

Bool::Bool(bool v) : value(v) {}


}
