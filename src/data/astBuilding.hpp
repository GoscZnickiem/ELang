#ifndef _ELC_DATA_ASTBUILDING_
#define _ELC_DATA_ASTBUILDING_

#include "data/tokens.hpp"
#include "help/graph.hpp"
#include "help/variant.hpp"
#include <cstddef>
#include <list>
#include <map>
#include <string>
#include <vector>

namespace elc::ast::build {

struct Node;

struct NodeCompare {
	bool operator()(const Node& a, const Node& b) const;
};

struct Node {
	std::vector<Node*> dependencies;
	std::vector<Node*> dependenceIn;
	Graph<Node*, NodeCompare>* graphNode;
	std::size_t priority;
};

struct Namespace;

struct CodeNode : public Node {
	std::list<Token> tokens;
	Namespace* parent;
};

struct Namespace : public CodeNode {
	std::string name;
};



struct Function : public CodeNode {
	std::string name;
};

struct VarDecl : public CodeNode {
	std::string name;
};

using Stub = variant<
	Namespace, Function, VarDecl
>;

struct Priority {
	bool operator()(const Stub* a, const Stub* b) const;
};

enum class SymbolClass {
	ANY, VALUE, FUNCTION, TYPE, PRECEDENCE,
	NAMESPACE
};

struct PrecedenceLevel {

};

struct OperatorParsingData {

};

struct SymbolParsingData {
	std::string name;
	ast::build::SymbolClass symbolClass;

};

struct Context {
	std::map<std::string, Stub> symbols;
	std::map<std::string, SymbolClass> symbolClass;
	Graph<Node*, NodeCompare> nodes;
	Namespace* currentNamespace;

	void setSymbolClass(std::string& s, SymbolClass c);
};

}

#endif // !_ELC_DATA_ASTBUILDING_
