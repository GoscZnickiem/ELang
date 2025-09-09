#include "astBuilding.hpp"
#include <stdexcept>

namespace elc::ast::build {

void Context::setSymbolClass(std::string& s, SymbolClass c) {
	auto [it, success] = symbolClass.try_emplace(s, c);
	if(success) return;
	if(it->second == SymbolClass::NAMESPACE) {
		if(c != SymbolClass::NAMESPACE) {
			throw std::runtime_error("The symbol '" + s + "' is a namespace.");
		}
	} else if(it->second != c && it->second != SymbolClass::ANY) {
		throw std::runtime_error("The symbol '" + s + "' is already declared as a symbol of a different symbol class.");
	}
	it->second = c;
}

bool NodeCompare::operator()(const Node& a, const Node& b) const {
	if(a.dependencies.size() != b.dependencies.size()) {
		return a.dependencies.size() > b.dependencies.size();
	}
	return a.priority > b.priority;
}

}
