#include "types.hpp"
#include "help/visitor.hpp"
#include <variant>

namespace elc::type {

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
namespace { 
int intTypeOrder(IntegerType t) {
	switch (t) {
		case IntegerType::Int32: return 0;
		case IntegerType::Uint32: return 1;
		case IntegerType::Int64: return 2;
		case IntegerType::Uint64: return 3;
		case IntegerType::Int16: return 4;
		case IntegerType::Uint16: return 5;
		case IntegerType::Int8: return 6;
		case IntegerType::Uint8: return 7;
		case IntegerType::Float32: return 8;
		case IntegerType::Float64: return 9;
	}
}
}
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

bool compiledTypesOrder(const CompiledType& a, const CompiledType& b) {
	return std::visit(visitor{
		[&](const Integer& a, const Integer& b) {
			return intTypeOrder(a->type) < intTypeOrder(b->type);
		},
		[&](const Pointer& a, const Pointer& b) {
			return compiledTypesOrder(a->pointed, b->pointed);
		},
		[&](const Struct& a, const Struct& b) {
			if(a->members.size() == b->members.size()) {
				for(std::size_t i = 0; i < a->members.size(); i++) {
					if(compiledTypesOrder(a->members[i], b->members[i])) return true;
					if(compiledTypesOrder(b->members[i], a->members[i])) return false;
				}
				return a->name < b->name;
			}  
			return a->members.size() < b->members.size();
		},
		[&](const Array& a, const Array& b) {
			if(compiledTypesOrder(a->element, b->element)) return true;
			if(compiledTypesOrder(b->element, a->element)) return false;
			return a->size < b->size;
		},
		[&](const Union& a, const Union& b) {
			if(a->members.size() != b->members.size()) return a->members.size() < b->members.size();
			for(std::size_t i = 0; i < a->members.size(); i++) {
				if(compiledTypesOrder(a->members[i], b->members[i])) return true;
				if(compiledTypesOrder(b->members[i], a->members[i])) return false;
			}
			return a->name < b->name;
		},
		[&](const Function& a, const Function& b) {
			if(compiledTypesOrder(a->returnType, b->returnType)) return true;
			if(compiledTypesOrder(b->returnType, a->returnType)) return false;
			if(a->argTypes.size() != b->argTypes.size()) return a->argTypes.size() < b->argTypes.size();
			for(std::size_t i = 0; i < a->argTypes.size(); i++) {
				if(compiledTypesOrder(a->argTypes[i], b->argTypes[i])) return true;
				if(compiledTypesOrder(b->argTypes[i], a->argTypes[i])) return false;
			}
			return false;
		},
		[&](const Integer&, const Bool&) {
			return true;
		},
		[&](const Integer&, const Pointer&) {
			return true;
		},
		[&](const Integer&, const Struct&) {
			return true;
		},
		[&](const Integer&, const Array&) {
			return true;
		},
		[&](const Integer&, const Union&) {
			return true;
		},
		[&](const Integer&, const Function&) {
			return true;
		},
		[&](const Bool&, const Pointer&) {
			return true;
		},
		[&](const Bool&, const Struct&) {
			return true;
		},
		[&](const Bool&, const Array&) {
			return true;
		},
		[&](const Bool&, const Union&) {
			return true;
		},
		[&](const Bool&, const Function&) {
			return true;
		},
		[&](const Pointer&, const Struct&) {
			return true;
		},
		[&](const Pointer&, const Array&) {
			return true;
		},
		[&](const Pointer&, const Union&) {
			return true;
		},
		[&](const Pointer&, const Function&) {
			return true;
		},
		[&](const Struct&, const Array&) {
			return true;
		},
		[&](const Struct&, const Union&) {
			return true;
		},
		[&](const Struct&, const Function&) {
			return true;
		},
		[&](const Array&, const Union&) {
			return true;
		},
		[&](const Array&, const Function&) {
			return true;
		},
		[&](const Union&, const Function&) {
			return true;
		},
		[&](const auto&, const auto&) {
			return false;
		}
	}, a, b);
}

}
