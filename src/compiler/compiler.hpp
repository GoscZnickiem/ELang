#ifndef _ELC_COMPILER_COMPILER_
#define _ELC_COMPILER_COMPILER_

#include "data/ast.hpp"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <memory>

namespace elc {

void compile(const ast::Unit& unit);

}

#endif // !_ELC_COMPILER_COMPILER_
