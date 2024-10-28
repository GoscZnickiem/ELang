#ifndef _ELC_COMPILER_COMPILER_
#define _ELC_COMPILER_COMPILER_

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <memory>
namespace elc {

class Compiler {
public:
	Compiler();

	void compileUnit();

private:
	std::unique_ptr<llvm::LLVMContext> llvmContext;
	std::unique_ptr<llvm::Module> module;
	std::unique_ptr<llvm::IRBuilder<>> builder;

};

void compile();

}

#endif // !_ELC_COMPILER_COMPILER_
