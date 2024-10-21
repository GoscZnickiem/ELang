#include "ast.hpp"

#include <string>
#include <utility>
#include <memory>

namespace elc::ast {

std::string opToString(ULeftOp op) {
	switch (op) {
		case ULeftOp::MINUS:
			return "-";
	}
}
std::string opToString(BiOp op) {
	switch (op) {
		case BiOp::PLUS:
			return "+";
		case BiOp::MINUS:
			return "-";
		case BiOp::MULT:
			return "*";
		case BiOp::DIV:
			return "/";
	}
}



std::string Numeral::toString() const {
	return value;
}

Numeral::Numeral(std::string v) : value(std::move(v)) {}



std::string ULeftOperator::toString() const {
	return "(" + opToString(op)+ " " + expr->toString() + ")";
}

ULeftOperator::ULeftOperator(std::unique_ptr<Expression>&& e, ULeftOp o)
	: expr(std::move(e)), op(o) {}

ULeftOperator::ULeftOperator(std::unique_ptr<Expression>& e, ULeftOp o)
	: expr(std::move(e)), op(o) {}



std::string BiOperator::toString() const {
	return "(" + left->toString() + " " + opToString(op)+ " " + right->toString() + ")";
}

BiOperator::BiOperator(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r, BiOp o)
	: left(std::move(l)), right(std::move(r)), op(o) {}

BiOperator::BiOperator(std::unique_ptr<Expression>& l, std::unique_ptr<Expression>& r, BiOp o)
	: left(std::move(l)), right(std::move(r)), op(o) {}


}
