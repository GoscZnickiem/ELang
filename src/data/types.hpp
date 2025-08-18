#ifndef _ELC_DATA_TYPES_
#define _ELC_DATA_TYPES_

#include <cstddef>
#include <memory>
#include <set>
#include <string>
#include <variant>
#include <vector>

namespace elc::type {

struct IntegralC; struct StructC; struct UnionC; struct ArrayC; struct PointerC; struct FunctionC;
using Integral = std::unique_ptr<IntegralC>;
using Struct = std::unique_ptr<StructC>;
using Array = std::unique_ptr<ArrayC>;
using Pointer = std::unique_ptr<PointerC>;
using Function = std::unique_ptr<FunctionC>;

using CompiledType = std::variant<Integral, Pointer, Struct, Array, Function>;

enum struct IntegralType {
	Int8, Int16, Int32, Int64,
	Uint8, Uint16, Uint32, Uint64,
	Float32, Float64,
	Bool
};

struct IntegralC {
	IntegralType type;
};

struct StructC {
	std::string name;
	std::vector<CompiledType> members;
};

struct ArrayC {
	CompiledType element;
	std::size_t size;
};

struct PointerC {
	CompiledType pointed;
};

struct FunctionC {
	CompiledType returnType;
	std::vector<CompiledType> argTypes;
};

struct AmbiguousC {
	std::set<CompiledType> possibleTypes;

	CompiledType unify();
	CompiledType unify(CompiledType desired);
};

bool compiledTypesOrder(const CompiledType& a, const CompiledType& b);
bool compiledTypesCompare(const CompiledType& a, const CompiledType& b);

}

#endif // !_ELC_DATA_TYPES_
