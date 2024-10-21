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


struct Expression : public Node { };

struct Numeral : public Expression {
	explicit Numeral(std::string v);
	std::string value;

	[[nodiscard]] std::string toString() const final;
};

struct Bool : public Expression {
	explicit Bool(bool v);
	bool value;

	[[nodiscard]] std::string toString() const final;
};

struct ULeftOperator : public Expression {
	ULeftOperator(std::unique_ptr<Expression>&& e, std::string o);
	ULeftOperator(std::unique_ptr<Expression>& e, std::string o);

	std::unique_ptr<Expression> expr;
	std::string op;

	[[nodiscard]] std::string toString() const final;
};

struct BiOperator : public Expression {
	BiOperator(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r, std::string o);
	BiOperator(std::unique_ptr<Expression>& l, std::unique_ptr<Expression>& r, std::string o);

	std::unique_ptr<Expression> left;
	std::unique_ptr<Expression> right;
	std::string op;

	[[nodiscard]] std::string toString() const final;
};



struct Unit {
	std::vector<std::unique_ptr<Expression>> expressions;
};

}

#endif // !_ELC_AST_
