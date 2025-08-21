#ifndef _ELC_DATA_ASTBUILDING_
#define _ELC_DATA_ASTBUILDING_

#include "data/tokens.hpp"
#include "data/types.hpp"
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <vector>

namespace elc::ast::build {

struct Node {
	std::vector<Node*> dependencies;
	std::vector<Node*> dependenceIn;
};

using NodePtr = std::unique_ptr<Node>;

struct Namespace;

struct Priority {
	bool operator()(const NodePtr& a, const NodePtr& b) const;
};

struct Context {
	std::priority_queue<NodePtr, std::vector<NodePtr>, Priority> unresolvedStubs;
	std::map<std::string, NodePtr> symbols;
	Namespace* currentNamespace;
};

struct Stub : public Node {
	std::list<Token> tokens;
	Namespace* parent;
};

struct Namespace : public Stub {
	std::string name;
};

struct Function : public Stub {
	std::string name;
	type::Function type;
};

struct VarDecl : public Stub {
	std::string name;
};

}

#endif // !_ELC_DATA_ASTBUILDING_
