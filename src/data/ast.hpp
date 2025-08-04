#ifndef _ELC_DATA_AST_
#define _ELC_DATA_AST_

#include "tokens.hpp"
#include "help/visitor.hpp"

#include <cstdint>
#include <memory>
#include <optional>
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

template <typename T, typename Variant>
struct matchesVariantT;

template <typename T, typename... Types>
struct matchesVariantT<T, std::variant<Types...>> 
	: std::disjunction<std::is_same<T, Types>...> {};

template <typename VariantSubset, typename T>
std::optional<VariantSubset> matchesVariant(const T& value) {
	if constexpr (matchesVariantT<T, VariantSubset>::value) {
		return VariantSubset{value};
	} else {
		return std::nullopt;
	}
}

// ========================================================== //
//              Atomic types forward declarations
// ========================================================== //

struct Item {
	Item() = default;
	Item(const Item&) = default;
	Item(Item&&) = delete;
	Item &operator=(const Item&) = default;
	Item &operator=(Item&&) = default;
	virtual ~Item() = default;
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
// Atomic statements
struct ReturnC; struct BlockC; struct VarDeclC; struct FunDeclC;
using Return = std::unique_ptr<ReturnC>;
using Block = std::unique_ptr<BlockC>;
using VarDecl = std::unique_ptr<VarDeclC>;
using FunDecl = std::unique_ptr<FunDeclC>;

// ========================================================== //
//                      Basic structures
// ========================================================== //

using Expression = variant<
	Numeral, Bool, Identifier, ULeftOperator, BiOperator, FunCall
>;

// using MetaDeclaration = variant<
//	FunctorDecl
// >;

using Declaration = variant<
	VarDecl, FunDecl
>;

using Statement = variant<
	Expression, Block, Return
>;

using Instruction = variant<
	Statement, Declaration //, MetaDeclaration
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

struct NumeralC : public Item {
	explicit NumeralC(std::string v);
	std::string value;
	[[nodiscard]] std::string toString() const final;
	[[nodiscard]] uint32_t getI32() const;
};

struct BoolC : public Item {
	explicit BoolC(bool v);
	bool value;
	[[nodiscard]] std::string toString() const final;
};

struct IdentifierC : public Item {
	explicit IdentifierC(std::string n);
	std::string name;
	[[nodiscard]] std::string toString() const final;
};

struct ULeftOperatorC : public Item {
	ULeftOperatorC(Expression&& e, std::string o);
	ULeftOperatorC(Expression& e, std::string o);
	Expression expr;
	std::string op;
	[[nodiscard]] std::string toString() const final;
};

struct BiOperatorC : public Item {
	BiOperatorC(Expression&& l, Expression&& r, std::string o);
	BiOperatorC(Expression& l, Expression& r, std::string o);
	Expression left;
	Expression right;
	std::string op;
	[[nodiscard]] std::string toString() const final;
};

struct FunCallC : public Item {
	FunCallC(Identifier&& n, ArgumentList&& args);
	FunCallC(Identifier& n, ArgumentList& args);
	Identifier name;
	ArgumentList arguments;
	[[nodiscard]] std::string toString() const final;
};

// Instructions:

struct ReturnC : public Item {
	explicit ReturnC(Expression&& e);
	explicit ReturnC(Expression& e);
	ReturnC() = default;
	Expression expr;
	[[nodiscard]] std::string toString() const final;
};

struct BlockC : public Item {
	BlockC() = default;
	std::vector<Statement> instructions;
	[[nodiscard]] std::string toString() const final;
};

struct VarDeclC : public Item {
	VarDeclC(Type&& t, Identifier&& n);
	VarDeclC(Type& t, Identifier& n);
	Type type;
	Identifier name;
	std::optional<Expression> expr;
	[[nodiscard]] std::string toString() const final;
};

struct FunDeclC : public Item {
	FunDeclC(Identifier&& n, Type&& t, ArgumentDeclList&& args, Block&& b);
	FunDeclC(Identifier& n, Type& t, ArgumentDeclList& args, Block& b);
	Identifier name;
	Type returnType;
	ArgumentDeclList arguments;
	Block body;
	[[nodiscard]] std::string toString() const final;
};

}

#endif // !_ELC_DATA_AST_
