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



struct Instruction : public Node { };



struct Expression : public Instruction { };

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

struct Identifier : public Expression {
	explicit Identifier(std::string n);
	std::string name;
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

using ArgumentList = std::vector<std::unique_ptr<Expression>>;

struct FunCall : public Expression {
	explicit FunCall(std::unique_ptr<Identifier>&& n, ArgumentList&& args);
	explicit FunCall(std::unique_ptr<Identifier>& n, ArgumentList& args);
	std::unique_ptr<Identifier> name;
	ArgumentList arguments;
	[[nodiscard]] std::string toString() const final;
};



struct Return : public Instruction {
	explicit Return(std::unique_ptr<Expression>&& e);
	explicit Return(std::unique_ptr<Expression>& e);
	std::unique_ptr<Expression> expr;
	[[nodiscard]] std::string toString() const final;
};





struct Block : public Instruction {
	Block() = default;
	std::vector<std::unique_ptr<Instruction>> instructions;
	[[nodiscard]] std::string toString() const final;
};



struct Type : public Node {
	explicit Type(std::string n);
	std::string name;
	[[nodiscard]] std::string toString() const final;
};



struct Declaration : public Instruction { };

struct VarDecl : public Declaration {
	explicit VarDecl(std::unique_ptr<Type>&& t, std::unique_ptr<Identifier>&& n);
	explicit VarDecl(std::unique_ptr<Type>& t, std::unique_ptr<Identifier>& n);
	std::unique_ptr<Type> type;
	std::unique_ptr<Identifier> name;
	[[nodiscard]] std::string toString() const final;
};

struct VarDeclAssign: public Declaration {
	explicit VarDeclAssign(std::unique_ptr<Type>&& t, std::unique_ptr<Identifier>&& n, std::unique_ptr<Expression>&& e);
	explicit VarDeclAssign(std::unique_ptr<Type>& t, std::unique_ptr<Identifier>& n, std::unique_ptr<Expression>& e);
	std::unique_ptr<Type> type;
	std::unique_ptr<Identifier> name;
	std::unique_ptr<Expression> expr;
	[[nodiscard]] std::string toString() const final;
};

using ArgumentDeclList = std::vector<std::pair<std::unique_ptr<Type>, std::unique_ptr<Identifier>>>;

struct FunDecl : public Declaration {
	explicit FunDecl(std::unique_ptr<Identifier>&& n, std::unique_ptr<Type>&& t, ArgumentDeclList&& args, Block&& b);
	explicit FunDecl( std::unique_ptr<Identifier>& n, std::unique_ptr<Type>& t, ArgumentDeclList& args, Block& b);
	std::unique_ptr<Identifier> name;
	std::unique_ptr<Type> returnType;
	ArgumentDeclList arguments;
	Block body;
	[[nodiscard]] std::string toString() const final;
};




struct Unit {
	std::vector<std::unique_ptr<Instruction>> globals;
};


}

#endif // !_ELC_AST_
