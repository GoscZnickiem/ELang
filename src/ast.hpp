#ifndef _ELC_AST_
#define _ELC_AST_

#include "tokens.hpp"

#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace elc::ast {

std::pair<int, bool> getBiOperatorData(TokenType token);
std::pair<int, int> getUOperatorData(TokenType token);

// ========================================================== //
//				Atomic types forward declarations
// ========================================================== //

struct Tomasz {
	Tomasz() = default;
	Tomasz(const Tomasz&) = default;
	Tomasz(Tomasz&&) = delete;
	Tomasz &operator=(const Tomasz&) = default;
	Tomasz &operator=(Tomasz&&) = default;
	virtual ~Tomasz() = default;
	[[nodiscard]] virtual std::string toString() const = 0;
};

// Atomic expressions
struct NumeralC; struct BoolC; struct IdentifierC; struct ULeftOperatorC; struct BiOperatorC; struct FunCallC;
using Numeral = std::unique_ptr<NumeralC>;
using Bool = std::unique_ptr<BoolC>;
using Identifier = std::unique_ptr<IdentifierC>;
using ULeftOperator = std::unique_ptr<ULeftOperatorC>;
using BiOperator = std::unique_ptr<BiOperatorC>;
using FunCall = std::unique_ptr<FunCallC>;
// Atomic instructions
struct ReturnC; struct BlockC; struct VarDeclC; struct VarDeclAssignC; struct FunDeclC;
using Return = std::unique_ptr<ReturnC>;
using Block = std::unique_ptr<BlockC>;
using VarDecl = std::unique_ptr<VarDeclC>;
using VarDeclAssign = std::unique_ptr<VarDeclAssignC>;
using FunDecl = std::unique_ptr<FunDeclC>;
// other
struct TypeC;
using Type = std::unique_ptr<TypeC>;

// ========================================================== //
//						Basic structures
// ========================================================== //

using Expression = std::variant<
	Numeral, Bool, Identifier, ULeftOperator, BiOperator, FunCall
>;

using Declaration = std::variant<
	VarDecl, VarDeclAssign, FunDecl
>;

using Instruction = std::variant<
	Expression, Declaration, Block, Return
>;
std::string toString(const Instruction& instruction);

using ArgumentList = std::vector<Expression>;
using ArgumentDeclList = std::vector<std::pair<Type, Identifier>>;

// ========================================================== //
//						Atomic types
// ========================================================== //

// Expressions:

struct NumeralC : public Tomasz {
	explicit NumeralC(std::string v);
	std::string value;
	[[nodiscard]] std::string toString() const final;
	[[nodiscard]] static Numeral Create();
};

struct BoolC : public Tomasz {
	explicit BoolC(bool v);
	bool value;
	[[nodiscard]] std::string toString() const final;
	[[nodiscard]] static Bool Create();
};

struct IdentifierC : public Tomasz {
	explicit IdentifierC(std::string n);
	std::string name;
	[[nodiscard]] std::string toString() const final;
	[[nodiscard]] static Identifier Create();
};

struct ULeftOperatorC : public Tomasz {
	ULeftOperatorC(Expression&& e, std::string o);
	ULeftOperatorC(Expression& e, std::string o);
	Expression expr;
	std::string op;
	[[nodiscard]] std::string toString() const final;
	[[nodiscard]] static ULeftOperator Create();
};

struct BiOperatorC : public Tomasz {
	BiOperatorC(Expression&& l, Expression&& r, std::string o);
	BiOperatorC(Expression& l, Expression& r, std::string o);
	Expression left;
	Expression right;
	std::string op;
	[[nodiscard]] std::string toString() const final;
	[[nodiscard]] static BiOperator Create();
};

struct FunCallC : public Tomasz {
	FunCallC(Identifier&& n, ArgumentList&& args);
	FunCallC(Identifier& n, ArgumentList& args);
	Identifier name;
	ArgumentList arguments;
	[[nodiscard]] std::string toString() const final;
	[[nodiscard]] static FunCall Create();
};

// Instructions:

struct ReturnC : public Tomasz {
	explicit ReturnC(Expression&& e);
	explicit ReturnC(Expression& e);
	Expression expr;
	[[nodiscard]] std::string toString() const final;
	[[nodiscard]] static Return Create();
};

struct BlockC : public Tomasz {
	BlockC() = default;
	std::vector<Instruction> instructions;
	[[nodiscard]] std::string toString() const final;
	[[nodiscard]] static Block Create();
};

struct VarDeclC : public Tomasz {
	VarDeclC(Type&& t, Identifier&& n);
	VarDeclC(Type& t, Identifier& n);
	Type type;
	Identifier name;
	[[nodiscard]] std::string toString() const final;
	[[nodiscard]] static VarDecl Create();
};

struct VarDeclAssignC: public Tomasz {
	VarDeclAssignC(Type&& t, Identifier&& n, Expression&& e);
	VarDeclAssignC(Type& t, Identifier& n, Expression& e);
	Type type;
	Identifier name;
	Expression expr;
	[[nodiscard]] std::string toString() const final;
	[[nodiscard]] static VarDeclAssign Create();
};

struct FunDeclC : public Tomasz {
	FunDeclC(Identifier&& n, Type&& t, ArgumentDeclList&& args, Block&& b);
	FunDeclC(Identifier& n, Type& t, ArgumentDeclList& args, Block& b);
	Identifier name;
	Type returnType;
	ArgumentDeclList arguments;
	Block body;
	[[nodiscard]] std::string toString() const final;
	[[nodiscard]] static FunDecl Create();
};

// Other

struct TypeC : public Tomasz {
	explicit TypeC(std::string n);
	std::string name;
	[[nodiscard]] std::string toString() const final;
	[[nodiscard]] static Type Create();
};




// ========================================================== //
//						Compilation Unit
// ========================================================== //

struct Unit {
	std::vector<Instruction> globals;
};


}

#endif // !_ELC_AST_
