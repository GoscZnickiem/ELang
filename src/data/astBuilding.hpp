#ifndef _ELC_DATA_ASTBUILDING_
#define _ELC_DATA_ASTBUILDING_

#include <vector>
namespace elc::ast::build {

struct Node {
	std::vector<Node*> dependencies;
	std::vector<Node*> dependenceIn;
};

struct CompilationUnit : public Node {
	
};



}

#endif // !_ELC_DATA_ASTBUILDING_
