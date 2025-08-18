#ifndef _ELC_DATA_AST_
#define _ELC_DATA_AST_

#include "help/visitor.hpp"
#include "help/variant.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace elc::ast {

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

// Expressions
struct IntegralC; struct FunCallC;
using Integral = std::unique_ptr<IntegralC>;
using FunCall = std::unique_ptr<FunCallC>;
// Statements
struct ReturnC; struct BlockC;
using Return = std::unique_ptr<ReturnC>;
using Block = std::unique_ptr<BlockC>;
// Declarations
struct DeclarationC;
struct VarDeclC; struct FunDeclC; struct OpDeclC; struct FunctorDeclC; struct FunctorOpDeclC;
struct TypeDeclC; struct AssocDeclC; struct FieldDeclC;
using Declaration = std::unique_ptr<DeclarationC>;
using VarDecl = std::unique_ptr<VarDeclC>;
using FunDecl = std::unique_ptr<FunDeclC>;
using OpDecl = std::unique_ptr<OpDeclC>;
using FunctorDecl = std::unique_ptr<FunctorDeclC>;
using FunctorOpDecl = std::unique_ptr<FunctorOpDeclC>;
using TypeDecl = std::unique_ptr<TypeDeclC>;
using AssocDecl = std::unique_ptr<AssocDeclC>;
using FieldDecl = std::unique_ptr<FieldDeclC>;
// Types
struct StructTypeC; struct ArrayTypeC; struct PointerTypeC; struct FunctionTypeC;
struct ModifierSectionC; struct FieldConstructorC;
using StructType = std::unique_ptr<StructTypeC>;
using ArrayType = std::unique_ptr<ArrayTypeC>;
using PointerType = std::unique_ptr<PointerTypeC>;
using FunctionType = std::unique_ptr<FunctionTypeC>;
using ModifierSection = std::unique_ptr<ModifierSectionC>; 
using FieldConstructor = std::unique_ptr<FieldConstructorC>; 
// Whatever the fuck this is
struct FunctionPackC; struct FunctionFormC;
using FunctionPack = std::unique_ptr<FunctionPackC>;
using FunctionForm = std::unique_ptr<FunctionFormC>;

struct CommaC; struct DisjunctionC;
using Comma = std::unique_ptr<CommaC>;
using Disjunction = std::unique_ptr<DisjunctionC>;
struct FunctorKindC;
using FunctorKind = std::unique_ptr<FunctorKindC>;
// Other
struct IdentifierC; struct FunctorCallC;
using Identifier = std::unique_ptr<IdentifierC>;
using FunctorCall = std::unique_ptr<FunctorCallC>;

// ========================================================== //
//                      Basic structures
// ========================================================== //

using Expression = variant<
	Integral, Identifier, FunCall, FunctorCall
>;

using Declarator = variant<
	VarDecl, FunDecl, OpDecl, FunctorDecl, FunctorOpDecl, TypeDecl, AssocDecl, FieldDecl
>;

using Statement = variant<
	Expression, Block, Return
>;

using Instruction = variant<
	Statement, Declaration
>;

using Type = variant<
	Identifier, StructType, ArrayType, PointerType, FunctionType, FunctorCall
>;

using Value = variant<
	Expression, Identifier, Type, FunctorCall
>;

using Kind = variant<
	Identifier, Type, FunctorKind, FunctorCall
>;

using Archetype = variant<
	Kind, Comma, Disjunction
>;

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

struct IntegralC : public Item {
	explicit IntegralC(std::string v);
	std::string value;
	[[nodiscard]] std::string toString() const final;
	[[nodiscard]] uint32_t getI32() const;
};

struct FunCallC : public Item {
	FunCallC(Identifier&& n, FunctionPack&& args);
	FunCallC(Identifier& n, FunctionPack& args);
	Identifier name;
	FunctionPack arguments;
	[[nodiscard]] std::string toString() const final;
};

// Statements:

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

// Declarations

struct DeclarationC : public Item {
	DeclarationC(Declarator&& d, std::vector<Identifier>&& m);
	DeclarationC(Declarator& d, std::vector<Identifier>& m);
	Declarator decl;
	std::vector<Identifier> modifiers;
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
	FunDeclC(Identifier&& n, Type&& t, FunctionForm&& args, Block&& b);
	FunDeclC(Identifier& n, Type& t, FunctionForm& args, Block& b);
	Identifier name;
	Type returnType;
	FunctionForm arguments;
	Block body;
	[[nodiscard]] std::string toString() const final;
};

struct OpDeclC : public Item {
	OpDeclC(Identifier&& n, Type&& t, FunctionForm&& args, Block&& b);
	OpDeclC(Identifier& n, Type& t, FunctionForm& args, Block& b);
	Identifier name;
	Type returnType;
	FunctionForm arguments;
	Block body;
	[[nodiscard]] std::string toString() const final;
};

// struct FunctorDeclC : public Item {
// 	FunctorDeclC(Identifier&& n, Type&& t, FunctionForm&& args, Block&& b);
// 	FunctorDeclC(Identifier& n, Type& t, FunctionForm& args, Block& b);
// 	Identifier name;
// 	Kind resultKind; 
// 	FunctorForm arguments;
// };

// struct FunctorOpDeclC : public Item {

struct TypeDeclC : public Item {
	TypeDeclC(Identifier&& n, Type&& t);
	TypeDeclC(Identifier& n, Type& t);
	Identifier name;
	Type value;
	[[nodiscard]] std::string toString() const final;
};

struct AssocDeclC : public Item {
	explicit AssocDeclC(Identifier&& n);
	explicit AssocDeclC(Identifier& n);
	AssocDeclC& withSide(Identifier&& v);
	AssocDeclC& withSide(Identifier& v);
	AssocDeclC& withAbove(std::vector<Identifier>&& v);
	AssocDeclC& withAbove(std::vector<Identifier>& v);
	AssocDeclC& withBelow(std::vector<Identifier>&& v);
	AssocDeclC& withBelow(std::vector<Identifier>& v);
	Identifier name;
	std::optional<Identifier> side;
	std::optional<std::vector<Identifier>> above;
	std::optional<std::vector<Identifier>> below;
};

struct FieldDeclC : public Item {
	FieldDeclC(Identifier&& n, FieldConstructor&& v);
	FieldDeclC(Identifier& n, FieldConstructor& v);
	Identifier name;
	FieldConstructor value;
};

// Types

struct StructTypeC : public Item {
	std::vector<variant<ModifierSection, FieldConstructor>> fields;
	std::string currentModifier;
};

struct ArrayTypeC : public Item {
	ArrayTypeC(Type&& t, std::size_t s);
	ArrayTypeC(Type& t, std::size_t s);
	Type type;
	std::size_t size;
};

struct PointerTypeC : public Item {
	explicit PointerTypeC(Type&& t);
	explicit PointerTypeC(Type& t);
	Type type;
}; 

struct FunctionTypeC : public Item {
	FunctionTypeC(Type&& t, FunctionForm&& a);
	FunctionTypeC(Type& t, FunctionForm& a);
	Type returnType;
	FunctionForm arguments;
};

struct ModifierSectionC : public Item {
	explicit ModifierSectionC(Identifier&& n);
	explicit ModifierSectionC(Identifier& n);
	Identifier name;
};

struct FieldConstructorC : public Item {
	explicit FieldConstructorC(Type&& t);
	explicit FieldConstructorC(Type& t);
}; 

// Other

struct IdentifierC : public Item {
	explicit IdentifierC(std::string n);
	std::string name;
	[[nodiscard]] std::string toString() const final;
};

}

#endif // !_ELC_DATA_AST_
