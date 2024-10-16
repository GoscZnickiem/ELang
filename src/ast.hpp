#ifndef _ELC_AST_
#define _ELC_AST_

#include <memory>
#include <string>

namespace elc::ast {

struct Expression {
	Expression(const Expression &) = default;
	Expression(Expression &&) = delete;
	Expression &operator=(const Expression &) = default;
	Expression &operator=(Expression &&) = delete;
	virtual ~Expression() = default;

	[[nodiscard]] virtual std::string toString() const = 0;
};

struct Numeral : public Expression {
	char type;
	std::string value;

	[[nodiscard]] std::string toString() const final;
};

struct BiOperator : public Expression {
	std::unique_ptr<Expression> left;
	std::unique_ptr<Expression> right;
	std::string operation;

	[[nodiscard]] std::string toString() const final;
};

}

#endif // !_ELC_AST_
