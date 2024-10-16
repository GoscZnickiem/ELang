#ifndef _ELC_AST_
#define _ELC_AST_

#include <memory>
#include <string>
#include <vector>

namespace elc::ast {

struct Node {
	Node(const Node&) = default;
	Node(Node&&) = delete;
	Node &operator=(const Node&) = default;
	Node &operator=(Node&&) = delete;
	virtual ~Node() = default;

	[[nodiscard]] virtual std::string toString() const = 0;
};

struct Expression : public Node { };

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

struct Unit {
	std::vector<std::unique_ptr<Expression>> expressions;
};

}

#endif // !_ELC_AST_
