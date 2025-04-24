#ifndef _ELC_COMPILER_TYPES_
#define _ELC_COMPILER_TYPES_

#include <cstddef>
#include <memory>
#include <set>
#include <string>
#include <variant>
#include <vector>

namespace elc::type {

struct IntegerC; struct StructC; struct UnionC; struct ArrayC; struct PointerC; struct FunctionC;
using Integer = std::unique_ptr<IntegerC>;
using Struct = std::unique_ptr<StructC>;
using Union = std::unique_ptr<UnionC>;
using Array = std::unique_ptr<ArrayC>;
using Pointer = std::unique_ptr<PointerC>;
using Function = std::unique_ptr<FunctionC>;

using CompiledType = std::variant<Integer, Struct, Union, Array, Pointer, Function>;

enum struct IntegerType {
	Int8, Int16, Int32, Int64,
	Uint8, Uint16, Uint32, Uint64,
	Float32, Float64
};

struct IntegerC {
	IntegerType type;
};

struct StructC {
	std::string name;
	std::vector<CompiledType> members;
};

struct UnionC {
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

}

#endif // !_ELC_COMPILER_TYPES_
