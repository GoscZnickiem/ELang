#ifndef _ELC_AST_
#define _ELC_AST_

#include "tokens.hpp"

#include <memory>
#include <string>
#include <vector>

namespace elc::ast {

std::pair<int, bool> getBiOperatorData(TokenType token);

std::pair<int, int> getUOperatorData(TokenType token);

struct Node {
	Node() = default;
	Node(const Node&) = default;
	Node(Node&&) = delete;
	Node &operator=(const Node&) = default;
	Node &operator=(Node&&) = default;
	virtual ~Node() = default;

	[[nodiscard]] virtual std::string toString() const = 0;
};

struct Declaration : public Node {
	explicit Declaration(std::string t, std::string n);
	std::string type;
	std::string name;

	[[nodiscard]] std::string toString() const final;
};


struct NumericalExpression : public Node { };

struct Numeral : public NumericalExpression {
	explicit Numeral(std::string v);
	std::string value;

	[[nodiscard]] std::string toString() const final;
};

struct ULeftOperator : public NumericalExpression {
	ULeftOperator(std::unique_ptr<NumericalExpression>&& e, std::string o);
	ULeftOperator(std::unique_ptr<NumericalExpression>& e, std::string o);

	std::unique_ptr<NumericalExpression> expr;
	std::string op;

	[[nodiscard]] std::string toString() const final;
};

struct BiOperator : public NumericalExpression {
	BiOperator(std::unique_ptr<NumericalExpression>&& l, std::unique_ptr<NumericalExpression>&& r, std::string o);
	BiOperator(std::unique_ptr<NumericalExpression>& l, std::unique_ptr<NumericalExpression>& r, std::string o);

	std::unique_ptr<NumericalExpression> left;
	std::unique_ptr<NumericalExpression> right;
	std::string op;

	[[nodiscard]] std::string toString() const final;
};



struct BoolExpression : public Node { };

struct Bool : public BoolExpression {
	explicit Bool(bool v);
	bool value;

	[[nodiscard]] std::string toString() const final;
};


struct Unit {
	std::vector<std::unique_ptr<NumericalExpression>> expressions;
};

}

#endif // !_ELC_AST_
