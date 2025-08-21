#ifndef _ELC_DATA_ASTBUILDING_
#define _ELC_DATA_ASTBUILDING_

#include "data/tokens.hpp"
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace elc::ast::build {

struct Node {
	std::vector<Node*> dependencies;
	std::vector<Node*> dependenceIn;
};

struct Namespace;

struct Context {
	std::list<std::unique_ptr<ast::build::Node>> unnamedStubs;
	std::map<std::string, std::unique_ptr<ast::build::Node>> namedStubs;
	Namespace* currentNamespace;
};

struct Namespace : public Node {
	std::string name;
	Namespace* parent;
	std::list<Token> tokens;
};

struct Stub : public Node {
	std::list<Token> tokens;
	Namespace* parent;
};

}

#endif // !_ELC_DATA_ASTBUILDING_
