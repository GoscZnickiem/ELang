#include "ast.hpp"

#include <string>

namespace elc::ast {

std::string Numeral::toString() const {
	return value;
}

std::string BiOperator::toString() const {
	return left->toString() + " " + operation + " " + right->toString();
}


}
