#include "symbols.hpp"
#include "help/visitor.hpp"
#include <stdexcept>
#include <variant>

namespace elc::ast::build {

void Symbol::addOverload(TokenParsingData&& data) {
	switch (symbolClass) {
		case SymbolClass::ANY: {
			switch (data.typoid.kind) {
				case Kind::VALUE:
					std::visit(visitor{
						[this](const type::Function&) {
							symbolClass = SymbolClass::FUNCTION;
						},
						[this](const auto&) {
							symbolClass = SymbolClass::VALUE;
						}
					}, *data.typoid.type);
					break;
				case Kind::TYPE:
					symbolClass = SymbolClass::TYPE;
					break;
				case Kind::PRECEDENCE:
					symbolClass = SymbolClass::PRECEDENCE;
					break;
				case Kind::FUNCTOR:
					break;
			}
			break; }
		case SymbolClass::FUNCTION:
			if(data.typoid.kind == Kind::FUNCTOR) break;
			if(data.typoid.kind == Kind::VALUE) {
				std::visit(visitor{
					[this](const type::Function&) {},
					[this](const auto&) {
						throw std::runtime_error("this should be a function, idiot");
					}
				}, *data.typoid.type);
				break;
			}
			throw std::runtime_error("this should be a function, idiot");
			break;
		case SymbolClass::VALUE:
		case SymbolClass::TYPE:
		case SymbolClass::PRECEDENCE:
		case SymbolClass::NAMESPACE:
			throw std::runtime_error("this value is already defined");
			break;
	}

	overloads.push_back(std::move(data));
}

void Symbol::setAsNamespace() {
	if(!overloads.empty()) {
		throw std::runtime_error("Namespaces cannot be named the same as other values.");
	}
	symbolClass = SymbolClass::NAMESPACE;
}

}
