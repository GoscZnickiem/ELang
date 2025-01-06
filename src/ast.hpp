#ifndef _ELC_AST_
#define _ELC_AST_

#include "tokens.hpp"
#include "help/visitor.hpp"

#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace elc::ast {

std::pair<int, bool> getBiOperatorData(TokenType token);
std::pair<int, int> getUOperatorData(TokenType token);

template<typename T>
concept stdvariant = requires(T a) {
	typename std::variant_size<T>::type;
};

template<stdvariant Var1, stdvariant Var2>
struct flatVariantBase2;
template<typename...T, typename...U>
struct flatVariantBase2<std::variant<T...>, std::variant<U...>> {
	using type = std::variant<T..., U...>;
};

template<typename...Vars>
struct flatVariantBase;
template<stdvariant V, typename...Vars>
struct flatVariantBase<V, Vars...>{
	using type = typename flatVariantBase<typename flatVariantBase2<V, typename flatVariantBase<Vars...>::type>::type>::type;
};
template<typename T, typename...Vars>
struct flatVariantBase<T, Vars...>{
	using type = typename flatVariantBase<std::variant<T>, Vars...>::type;
};
template<stdvariant V>
struct flatVariantBase<V>{
	using type = V;
};
template<typename T>
struct flatVariantBase<T>{
	using type = std::variant<T>;
};

template<typename...T>
using variant = flatVariantBase<T...>::type;


// ========================================================== //
//              Atomic types forward declarations
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
// Kinds
struct KindDeclC;
using KindDecl = std::unique_ptr<KindDeclC>;
struct KindValDeclC;
using KindValDecl = std::unique_ptr<KindValDeclC>;
struct FunctorDeclC;
using FunctorDecl = std::unique_ptr<FunctorDeclC>;
struct FunctorCallC;
using FunctorCall = std::unique_ptr<FunctorCallC>;
// Types
struct TypeDeclC;
using TypeDecl = std::unique_ptr<TypeDeclC>;

// ========================================================== //
//                      Basic structures
// ========================================================== //

using Expression = variant<
	Numeral, Bool, Identifier, ULeftOperator, BiOperator, FunCall
>;

using Declaration = variant<
	VarDecl, VarDeclAssign, FunDecl
>;

using Instruction = variant<
	Expression, Declaration, Block, Return
>;

using Kind = variant<
	Identifier 
>;

using Type = variant<
	Identifier 
>;

using ArgumentList = std::vector<Expression>;
using ArgumentDeclList = std::vector<std::pair<Type, Identifier>>;

template<stdvariant V>
std::string astToString(const V& arg) {
	return std::visit(visitor{
		[&](auto& e) {
			return e->toString();
		}
	}, arg);
}

// ========================================================== //
//                        Atomic types
// ========================================================== //

// Expressions:

struct NumeralC : public Tomasz {
	explicit NumeralC(std::string v);
	std::string value;
	[[nodiscard]] std::string toString() const final;
};

struct BoolC : public Tomasz {
	explicit BoolC(bool v);
	bool value;
	[[nodiscard]] std::string toString() const final;
};

struct IdentifierC : public Tomasz {
	explicit IdentifierC(std::string n);
	std::string name;
	[[nodiscard]] std::string toString() const final;
};

struct ULeftOperatorC : public Tomasz {
	ULeftOperatorC(Expression&& e, std::string o);
	ULeftOperatorC(Expression& e, std::string o);
	Expression expr;
	std::string op;
	[[nodiscard]] std::string toString() const final;
};

struct BiOperatorC : public Tomasz {
	BiOperatorC(Expression&& l, Expression&& r, std::string o);
	BiOperatorC(Expression& l, Expression& r, std::string o);
	Expression left;
	Expression right;
	std::string op;
	[[nodiscard]] std::string toString() const final;
};

struct FunCallC : public Tomasz {
	FunCallC(Identifier&& n, ArgumentList&& args);
	FunCallC(Identifier& n, ArgumentList& args);
	Identifier name;
	ArgumentList arguments;
	[[nodiscard]] std::string toString() const final;
};

// Instructions:

struct ReturnC : public Tomasz {
	explicit ReturnC(Expression&& e);
	explicit ReturnC(Expression& e);
	ReturnC() = default;
	Expression expr;
	[[nodiscard]] std::string toString() const final;
};

struct BlockC : public Tomasz {
	BlockC() = default;
	std::vector<Instruction> instructions;
	[[nodiscard]] std::string toString() const final;
};

struct VarDeclC : public Tomasz {
	VarDeclC(Type&& t, Identifier&& n);
	VarDeclC(Type& t, Identifier& n);
	Type type;
	Identifier name;
	[[nodiscard]] std::string toString() const final;
};

struct VarDeclAssignC: public Tomasz {
	VarDeclAssignC(Type&& t, Identifier&& n, Expression&& e);
	VarDeclAssignC(Type& t, Identifier& n, Expression& e);
	Type type;
	Identifier name;
	Expression expr;
	[[nodiscard]] std::string toString() const final;
};

struct FunDeclC : public Tomasz {
	FunDeclC(Identifier&& n, Type&& t, ArgumentDeclList&& args, Block&& b);
	FunDeclC(Identifier& n, Type& t, ArgumentDeclList& args, Block& b);
	Identifier name;
	Type returnType;
	ArgumentDeclList arguments;
	Block body;
	[[nodiscard]] std::string toString() const final;
};

// Kinds:

struct KindDeclC : public Tomasz {
	explicit KindDeclC(Identifier&& n, Kind&& k);
	Identifier name;
	Kind value;
	[[nodiscard]] std::string toString() const final;
};

struct KindValDeclC : public Tomasz {
	explicit KindValDeclC(Kind&& k, Identifier&& n, );
	Identifier name;
	Kind value;
	[[nodiscard]] std::string toString() const final;
};

// Types



// ========================================================== //
//                     Compilation Unit
// ========================================================== //

struct Unit {
	std::string name;
	std::vector<Declaration> globals;
};


}

#endif // !_ELC_AST_
