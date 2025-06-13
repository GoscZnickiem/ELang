#include "compilationUnit.hpp"
#include "ast.hpp"

#include <iostream>
#include <optional>
#include <variant>

namespace elc {

CompilationUnit::CompilationUnit(const char* fileName)
: sourceFileName(fileName),
	sourceFile(fileName),
	env(),
	tokenizer(&sourceFile),
	parser(this) { }

void CompilationUnit::process() {
	std::cout << "File " << sourceFileName << ":\n";
	if(!sourceFile.is_open()) {
		std::cerr << "Error: Unable to open file " << sourceFileName << "\n";
		return;
	}

	for(auto inst = parser.getInstruction(); inst.has_value(); inst = parser.getInstruction()) {
		std::visit([](auto&& val) {
			using T = std::decay_t<decltype(val)>;

			if (auto i = ast::matchesVariant<ast::Statement>(val)) {
				
			} else if (auto i = ast::matchesVariant<ast::Declaration>(val)) {

			} else {
				throw std::runtime_error("YOU MORON");
			}
		}, *inst);
	}
}

CompilationUnit::~CompilationUnit() {
	sourceFile.close();
}

}
