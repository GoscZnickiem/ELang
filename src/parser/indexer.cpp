#include "indexer.hpp"
#include "data/tokens.hpp"
#include <cstddef>
#include <list>
#include <map>
#include <stdexcept>
#include <string>

namespace elc {

namespace {

struct Namespace;

struct UnidentifiedStub {
	std::list<Token> tokens;
	Namespace* space{};
};

struct Stub {
	std::string name;
	std::list<Token> tokens;
	Namespace* space{};
};

std::list<Token>::iterator findEndOfStatement(std::list<Token>& tokens) {
	std::size_t braceBalance = 0;
	auto it = tokens.begin();
	while(true) {
		if(it->type == TokenType::BRACE_L) {
			braceBalance++;
		} else if(it->type == TokenType::BRACE_R) {
			braceBalance--;
			if(braceBalance < 0) {
				throw std::runtime_error("Error: unexpected token '" +
							 tokens.begin()->data +
							 "' encountered. Line: " +
							 std::to_string(tokens.begin()->metadata.lineEnd) + 
							 ", Column: " + 
							 std::to_string(tokens.begin()->metadata.columnEnd) +
							 ".");
			}
		} else if(braceBalance == 0 && it->type == TokenType::SEMICOLON) {
			return ++it;
		}
		it++;
	}
	return it;
}

UnidentifiedStub splitStub(std::list<Token>& tokens) {
	UnidentifiedStub stub;
	stub.tokens.splice(stub.tokens.end(), tokens, tokens.begin(), findEndOfStatement(tokens));
	return stub;
}

struct Namespace {
	std::string name;
	std::list<UnidentifiedStub> unidentifiedStubs;
	std::map<std::string, Stub> stubs;
	std::map<std::string, Namespace> children;
	Namespace* parent;

	explicit Namespace(std::string n, Namespace* p = nullptr) : name(std::move(n)), parent(p) {}

	void listContents(std::list<Token>& tokens) {
		for(auto it = tokens.begin(); it != tokens.end(); it = tokens.begin()) {
			if(it->type == TokenType::SYMBOL && it->data == "namespace") {
				processNamespace(tokens);
			} else if(it->type == TokenType::END || it->type == TokenType::BRACE_R) {
				return;
			} else {
				unidentifiedStubs.push_back(std::move(splitStub(tokens)));
			}
		}

		throw std::runtime_error("No end token???");
	}

	void processNamespace(std::list<Token>& tokens) {
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

		auto [child, _] = children.emplace(spaceName, this);
		child->second.listContents(tokens);

		it = tokens.begin();
		if(it->type != TokenType::BRACE_R) {
			throw std::runtime_error("Error: expected '}'. Line: " +
							std::to_string(it->metadata.lineEnd) + 
							", Column: " + 
							std::to_string(it->metadata.columnEnd) +
							".");
		}
		tokens.erase(it);
	}

	void identifyTypes() {
		for(auto it = unidentifiedStubs.begin(); it != unidentifiedStubs.end();) {
			auto current = it++;
			auto i = current->tokens.begin();
			if(i->type == TokenType::SEMICOLON) {
				unidentifiedStubs.erase(current);
			} else if(i->type == TokenType::SYMBOL && i->data == "type") {
				i++;
				if(i->type != TokenType::SYMBOL) {
					throw std::runtime_error("Error: expected typename identifier. Line: " +
							  std::to_string(i->metadata.lineEnd) + 
							  ", Column: " + 
							  std::to_string(i->metadata.columnEnd) +
							  ".");
				}
				// TODO: check validity of the type name (maybe)
				auto [_, success] = stubs.emplace(i->data, std::move(current->tokens), this);
				if(!success) {
					throw std::runtime_error("Error: Redefinition of type symbol'" + 
							  i->data +
							  "'. Line: " +
							  std::to_string(i->metadata.lineEnd) + 
							  ", Column: " + 
							  std::to_string(i->metadata.columnEnd) +
							  ".");
				}
				unidentifiedStubs.erase(current);
			}

		}

		for(auto& child : children) {
			child.second.identifyTypes();
		}
	}

	void identifyFunctors() {
		for(auto it = unidentifiedStubs.begin(); it != unidentifiedStubs.end();) {
			auto current = it++;
			auto i = current->tokens.begin();
			if(i->type == TokenType::SEMICOLON) {
				unidentifiedStubs.erase(current);
			} else if(i->type == TokenType::SYMBOL && i->data == "type") {
				i++;
				if(i->type != TokenType::SYMBOL) {
					throw std::runtime_error("Error: expected typename identifier. Line: " +
							  std::to_string(i->metadata.lineEnd) + 
							  ", Column: " + 
							  std::to_string(i->metadata.columnEnd) +
							  ".");
				}
				// TODO: check validity of the type name (maybe)
				auto [_, success] = stubs.emplace(i->data, std::move(current->tokens), this);
				if(!success) {
					throw std::runtime_error("Error: Redefinition of type symbol'" + 
							  i->data +
							  "'. Line: " +
							  std::to_string(i->metadata.lineEnd) + 
							  ", Column: " + 
							  std::to_string(i->metadata.columnEnd) +
							  ".");
				}
				unidentifiedStubs.erase(current);
			}

		}

		for(auto& child : children) {
			child.second.identifyFunctors();
		}
	}
};

}

std::vector<Stub> index(std::list<Token>& tokens) {
	Namespace global{""};
	global.listContents(tokens);
	if(tokens.begin()->type != TokenType::END) { 
		throw std::runtime_error("Error: unexpected token '" +
						   tokens.begin()->data +
						   "' encountered. Line: " +
						   std::to_string(tokens.begin()->metadata.lineEnd) + 
						   ", Column: " + 
						   std::to_string(tokens.begin()->metadata.columnEnd) +
						   ".");
	}
}

}
