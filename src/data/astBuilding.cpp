#include "astBuilding.hpp"

namespace elc::ast::build {

bool Priority::operator()(const NodePtr& a, const NodePtr& b) const {
	return a->dependencies.size() > b->dependencies.size();
}

}
