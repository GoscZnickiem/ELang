#include "compiler.hpp"
#include "ast.hpp"
#include "help/visitor.hpp"

#include <iostream>
#include <llvm/Analysis/CGSCCPassManager.h>
#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/MC/TargetRegistry.h>
#include <cstdint>
#include <memory>
#include <string>
#include <system_error>
#include <variant>

namespace elc {

Compiler::Compiler() {
	llvm::InitializeNativeTarget();
	llvm::InitializeNativeTargetAsmPrinter();
	llvm::InitializeNativeTargetAsmParser();

	llvmContext = std::make_unique<llvm::LLVMContext>();
	module = std::make_unique<llvm::Module>("HelloModule", *llvmContext);
	builder = std::make_unique<llvm::IRBuilder<>>(*llvmContext);
}

void Compiler::compileUnit([[maybe_unused]] const ast::Unit& unit) {

	for(const auto& e : unit.globals) {
		std::visit(visitor{
			[&](const ast::FunDecl& a) {
				std::cout << "a function declaration! " << a->name->toString() << "\n";
			},
			[&](auto& arg) {
				std::cout << "Something: " << arg->toString() << "\n";
			}
		}, e);
	}

	constexpr uint32_t val = 42;
	const std::string mainFunName = "main";

	llvm::FunctionType* mainFunType = llvm::FunctionType::get(builder->getInt32Ty(), false);
	llvm::Function* mainFun = llvm::Function::Create(mainFunType, llvm::Function::ExternalLinkage, mainFunName, *module);
	verifyFunction(*mainFun);
	llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(*llvmContext, "entry", mainFun);
	builder->SetInsertPoint(entryBlock);

	llvm::Value* resInt = builder->getInt32(val);
	llvm::Value* resI32 = builder->CreateIntCast(resInt, builder->getInt32Ty(), true);
	builder->CreateRet(resI32);

	auto targetTriple = llvm::sys::getDefaultTargetTriple();
	module->setTargetTriple(targetTriple);


	std::string error;
    const llvm::Target* target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
    if (target == nullptr) {
        llvm::errs() << error;
        return;
    }

    const llvm::TargetOptions opt;
    auto* targetMachine = target->createTargetMachine(targetTriple, "generic", "", opt, llvm::Reloc::PIC_);
    module->setDataLayout(targetMachine->createDataLayout());

	std::error_code errCode;
	llvm::raw_fd_ostream outStream("./hello", errCode);
	module->print(outStream, nullptr);
	module->print(llvm::outs(), nullptr);

	llvm::raw_fd_ostream dest("hello.o", errCode, llvm::sys::fs::OpenFlags::OF_None);
    if (errCode) {
        llvm::errs() << "Could not open file: " << errCode.message();
        return;
    }

    llvm::legacy::PassManager pass;
    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, llvm::CodeGenFileType::ObjectFile)) {
        llvm::errs() << "TargetMachine can't emit a file of this type";
        return;
    }

    pass.run(*module);
    dest.flush();
}



void compile(const ast::Unit& unit) {
	static Compiler compiler;
	compiler.compileUnit(unit);
}

}
