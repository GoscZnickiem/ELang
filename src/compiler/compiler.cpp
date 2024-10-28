#include "compiler.hpp"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>
#include <system_error>

namespace elc {

Compiler::Compiler() {
	llvmContext = std::make_unique<llvm::LLVMContext>();
	module = std::make_unique<llvm::Module>("HelloModule", *llvmContext);
	builder = std::make_unique<llvm::IRBuilder<>>(*llvmContext);
}

void Compiler::compileUnit() {
	module->print(llvm::outs(), nullptr);

	std::error_code errCode;
	llvm::raw_fd_ostream outStream("./hello", errCode);
	module->print(outStream, nullptr);
}



void compile() {
	Compiler compiler;
	compiler.compileUnit();
}

}
