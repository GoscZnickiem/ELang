#include "compiler.hpp"
#include "ast.hpp"
#include "help/visitor.hpp"

// #include <cstddef>
#include <memory>
#include <string>
#include <system_error>
#include <variant>
#include <map>
// #include <vector>
#include <iostream>
#include <llvm/Analysis/CGSCCPassManager.h>
#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/DerivedTypes.h>
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

namespace elc {

const std::map<std::string, unsigned> builtinIntTypes {
	{"i32", 32}
};

class Compiler {
public:
	Compiler() {
		llvm::InitializeNativeTarget();
		llvm::InitializeNativeTargetAsmPrinter();
		llvm::InitializeNativeTargetAsmParser();

		llvmContext = std::make_unique<llvm::LLVMContext>();
		builder = std::make_unique<llvm::IRBuilder<>>(*llvmContext);
	}

	std::unique_ptr<llvm::LLVMContext> llvmContext;
	std::unique_ptr<llvm::IRBuilder<>> builder;
	std::unique_ptr<llvm::Module> module;

	[[nodiscard]] llvm::Type* readType(const ast::Type& type) const {
		llvm::Type* result = nullptr;
		std::visit(visitor{
			[&](const ast::Identifier& ident) {
				const std::string& name = ident->name;
				if(builtinIntTypes.contains(name)) {
					result = llvm::IntegerType::get(*llvmContext, builtinIntTypes.at(name));
				} else {
					throw std::runtime_error("Error: undefined type name " + name);
				}
			},
			[&](const auto& arg) {
				throw std::runtime_error("Error: unknown type " + arg->toString() + " (how tf did this even happen)");
			}
		}, type);

		if(result == nullptr) {
			throw std::runtime_error("Error: failed to read type " + astToString(type));
		}
		return result;
	}

	[[nodiscard]] llvm::Value* compileExpression(const ast::Expression& expr) const {
		llvm::Value* result = nullptr;
		std::visit(visitor{
			[&](const ast::Numeral& num) {
				// TODO: type?
				result = builder->getInt32(num->getI32());
			},
			[&](const auto& arg) {
				throw std::runtime_error("Error: No idea how to compile " + arg->toString());
			}
		}, expr);

		return result;
	}

	void compileInstruction(const ast::Instruction& instr) const {
		std::visit(visitor{
			[&](const ast::Return& ret) {
				llvm::Value* expr = compileExpression(ret->expr);
				// TODO: check return type
				llvm::Value* resI32 = builder->CreateIntCast(expr, builder->getInt32Ty(), true);
				builder->CreateRet(resI32);
			},
			[&](const auto& arg) {
				throw std::runtime_error("Error: No idea how to compile " + arg->toString());
			}
		}, instr);
	}

	void compileBlock(const ast::Block& block) const {
		for(const auto& instr : block->instructions) {
			compileInstruction(instr);
		}
	}

	[[nodiscard]] llvm::Function* createFunction(const ast::FunDecl& funDecl) const {
		llvm::Type* returnType = readType(funDecl->returnType);
		std::vector<llvm::Type*> parameterTypes;
		for(const auto& arg : funDecl->arguments) {
			parameterTypes.push_back(readType(arg.first));
		}
		llvm::FunctionType* funType = llvm::FunctionType::get(returnType, parameterTypes, false);
		llvm::Function* fun = llvm::Function::Create(funType, llvm::Function::ExternalLinkage, funDecl->name->name, *module);

		std::size_t i = 0;
		for(auto& arg : fun->args()) {
			arg.setName(funDecl->arguments[i++].second->name);
		}

		llvm::verifyFunction(*fun, &llvm::errs());

		llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(*llvmContext, "entry", fun);
		builder->SetInsertPoint(entryBlock);

		compileBlock(funDecl->body);

		return fun;
	}

	void compileUnit([[maybe_unused]] const ast::Unit& unit) {
		module = std::make_unique<llvm::Module>(unit.name, *llvmContext);

		for(const auto& e : unit.globals) {
			std::visit(visitor{
				[&](const ast::FunDecl& a) {
					auto* _ = createFunction(a);
				},
				[&](auto& arg) {
					std::cerr << "Well I cannot read this: " << arg->toString() << "\n";
				}
			}, e);
		}

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
};


void compile(const ast::Unit& unit) {
	static Compiler compiler;
	compiler.compileUnit(unit);
}

}
