#include "indexer.hpp"
#include "data/tokens.hpp"
#include <list>
#include <stdexcept>
#include <string>
#include <utility>

namespace elc {

namespace {

std::pair<Stub, std::list<Token>::iterator> identifyStub(std::list<Token>::iterator it) {

}

std::pair<std::list<Stub>, std::list<Token>::iterator> listContents(std::list<Token>& tokens, std::string space);

std::pair<std::list<Stub>, std::list<Token>::iterator> processNamespace(std::list<Token>& tokens, std::string space) {
	std::list<Stub> stubs;
	auto it = tokens.begin();

	it++;
	if(it->type != TokenType::SYMBOL) {
		throw std::runtime_error("Error: expected namespace name but '" +
						   it->data +
						   "' encountered. Line: " +
						   std::to_string(it->metadata.lineEnd) + 
						   ", Column: " + 
						   std::to_string(it->metadata.columnEnd) +
						   ".");
	}
	std::string spaceName = it->data;
	// TODO: check if namespace name is valid
	// TODO: nested namespaces
	it++;
	if(it->type != TokenType::BRACE_L) {
		throw std::runtime_error("Error: expected '{' but '" +
						   it->data +
						   "' encountered. Line: " +
						   std::to_string(it->metadata.lineEnd) + 
						   ", Column: " + 
						   std::to_string(it->metadata.columnEnd) +
						   ".");
	}
	it++;

	tokens.erase(tokens.begin(), it);

	auto [s, i] = listContents(tokens, space + "::" + spaceName);
	stubs.splice(stubs.end(), s);
	it = i;

	if(it->type != TokenType::BRACE_R) {
		throw std::runtime_error("Error: expected '}'. Line: " +
						   std::to_string(it->metadata.lineEnd) + 
						   ", Column: " + 
						   std::to_string(it->metadata.columnEnd) +
						   ".");
	}

	tokens.erase(it);

	return {stubs, it};
}

// Something somth = expr;
// =>
// def somth of Something;
// somth = expr;
//
// fun f(Arg1 a1, Arg2 a2) -> RetType { function body };
// =>
// def f of fun (Arg1, Arg2) -> RetType;
// f = (Arg1 a1, Arg2 a2){ function body };
//
// class A : B { class body };
// =>
// def A of type;
// A = class { class body } : B;

std::pair<std::list<Stub>, std::list<Token>::iterator> listContents(std::list<Token>& tokens, std::string space) {
	std::list<Stub> stubs;

	for(auto it = tokens.begin(); it != tokens.end(); it++) {
		if(it->type == TokenType::SYMBOL && it->data == "namespace") {
			auto [s, i] = processNamespace(tokens, space);
			stubs.splice(stubs.end(), s);
			it = i;
		// } else if() { TODO: other stuff
		} else if(it->type == TokenType::SYMBOL && it->data == "def") {

		}
	}
}

}

std::vector<Stub> index(std::list<Token>& tokens) {
	
}

}
