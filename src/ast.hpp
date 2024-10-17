#ifndef _ELC_AST_
#define _ELC_AST_

#include <memory>
#include <string>
#include <vector>

namespace elc::ast {

struct Node {
	Node() = default;
	Node(const Node&) = default;
	Node(Node&&) = delete;
	Node &operator=(const Node&) = default;
	Node &operator=(Node&&) = delete;
	virtual ~Node() = default;

	[[nodiscard]] virtual std::string toString() const = 0;
};

struct Expression : public Node { };

struct Numeral : public Expression {
	explicit Numeral(std::string v);
	std::string value;

	[[nodiscard]] std::string toString() const final;
};

enum class ULeftOp {
	MINUS
};

struct ULeftOperator : public Expression {
	ULeftOperator(std::unique_ptr<Expression>&& e, ULeftOp o);
	ULeftOperator(std::unique_ptr<Expression>& e, ULeftOp o);

	std::unique_ptr<Expression> expr;
	ULeftOp op;

	[[nodiscard]] std::string toString() const final;
};

enum class BiOp {
	PLUS, MINUS, MULT, DIV
};

struct BiOperator : public Expression {
	BiOperator(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r, BiOp o);
	BiOperator(std::unique_ptr<Expression>& l, std::unique_ptr<Expression>& r, BiOp o);

	std::unique_ptr<Expression> left;
	std::unique_ptr<Expression> right;
	BiOp op;

	[[nodiscard]] std::string toString() const final;
};

struct Unit {
	std::vector<std::unique_ptr<Expression>> expressions;
};

}

#endif // !_ELC_AST_
