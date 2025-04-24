#ifndef _ELC_COMPILER_TYPECHECKER_
#define _ELC_COMPILER_TYPECHECKER_

#include "ast.hpp"
#include "types.hpp"
#include <set>

namespace elc::type {

std::set<CompiledType> getAmbiguousType(const ast::Expression& expr);

}

#endif // !_ELC_COMPILER_TYPECHECKER_
