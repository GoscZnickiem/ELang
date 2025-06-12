#include "types.hpp"
#include "help/visitor.hpp"
#include <variant>

namespace elc::type {

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
namespace {
int intTypeOrder(IntegerType t) {
	switch (t) {
		case IntegerType::Int32:	return 0;
		case IntegerType::Uint32:	return 1;
		case IntegerType::Int64:	return 2;
		case IntegerType::Uint64:	return 3;
		case IntegerType::Int16:	return 4;
		case IntegerType::Uint16:	return 5;
		case IntegerType::Int8:		return 6;
		case IntegerType::Uint8:	return 7;
		case IntegerType::Float32:	return 8;
		case IntegerType::Float64:	return 9;
	}
}

constexpr int typeOrderHelp(const CompiledType& t) {
	return std::visit(visitor{
		[&](const Integer&)		{ return 0; },
		[&](const Bool&)		{ return 1; },
		[&](const Pointer&)		{ return 2; },
		[&](const Struct&)		{ return 3; },
		[&](const Array&)		{ return 4; },
		[&](const Union&)		{ return 5; },
		[&](const Function&)	{ return 6; }
	}, t);
}

bool integerTypeCompare(const Integer& a, const Integer& b) {
	return intTypeOrder(a->type) < intTypeOrder(b->type);
}
constexpr bool pointerTypeCompare(const Pointer& a, const Pointer& b) {
	return compiledTypesOrder(a->pointed, b->pointed);
};
constexpr bool structTypeCompare(const Struct& a, const Struct& b) {
	if(a->members.size() == b->members.size()) {
		for(std::size_t i = 0; i < a->members.size(); i++) {
			if(compiledTypesOrder(a->members[i], b->members[i])) return true;
			if(compiledTypesOrder(b->members[i], a->members[i])) return false;
		}
		return a->name < b->name;
	}  
	return a->members.size() < b->members.size();
};
constexpr bool arrayTypeCompare(const Array& a, const Array& b) {
	if(compiledTypesOrder(a->element, b->element)) return true;
	if(compiledTypesOrder(b->element, a->element)) return false;
	return a->size < b->size;
};
constexpr bool unionTypeCompare(const Union& a, const Union& b) {
	if(a->members.size() != b->members.size()) return a->members.size() < b->members.size();
	for(std::size_t i = 0; i < a->members.size(); i++) {
		if(compiledTypesOrder(a->members[i], b->members[i])) return true;
		if(compiledTypesOrder(b->members[i], a->members[i])) return false;
	}
	return a->name < b->name;
};
constexpr bool functionTypeCompare(const Function& a, const Function& b) {
	if(compiledTypesOrder(a->returnType, b->returnType)) return true;
	if(compiledTypesOrder(b->returnType, a->returnType)) return false;
	if(a->argTypes.size() != b->argTypes.size()) return a->argTypes.size() < b->argTypes.size();
	for(std::size_t i = 0; i < a->argTypes.size(); i++) {
		if(compiledTypesOrder(a->argTypes[i], b->argTypes[i])) return true;
		if(compiledTypesOrder(b->argTypes[i], a->argTypes[i])) return false;
	}
	return false;
};
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

// it's basically a < b
bool compiledTypesOrder(const CompiledType& a, const CompiledType& b) {
	if(typeOrderHelp(a) != typeOrderHelp(b)) {
		return typeOrderHelp(a) < typeOrderHelp(b); 
	}
	return std::visit(visitor{
		[&](const Integer& a, const Integer& b) {
			return integerTypeCompare(a, b);
		},
		[&](const Pointer& a, const Pointer& b) {
			return pointerTypeCompare(a, b);
		},
		[&](const Struct& a, const Struct& b) {
			return structTypeCompare(a, b);
		},
		[&](const Array& a, const Array& b) {
			return arrayTypeCompare(a, b);
		},
		[&](const Union& a, const Union& b) {
			return unionTypeCompare(a, b);
		},
		[&](const Function& a, const Function& b) {
			return functionTypeCompare(a, b);
		},
		[&](const auto&, const auto&) {
			throw std::logic_error("You were too silly, machine");
			return true;
		}
	}, a, b);
}

// a == b
bool compiledTypesCompare(const CompiledType& a, const CompiledType& b) {
	return !(compiledTypesCompare(a, b) && compiledTypesCompare(b, a));
}

}
