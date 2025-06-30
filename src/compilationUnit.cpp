#include "compilationUnit.hpp"
#include "lexer/lexer.hpp"
#include "parser/indexer.hpp"
#include "data/types.hpp"

#include <fstream>
#include <iostream>
#include <list>
#include <map>

namespace elc {

namespace {

void processFile(std::ifstream& sourceFile) {
	std::list<Token> tokens;
	try{
		tokens = lex(sourceFile);
	} catch (std::runtime_error& e) {
		for(auto& token : tokens) {
			std::cout << token << "\n";
		}
		std::cerr << "\033[1;31m================\n";
		std::cerr << "Lexing error:\033[0m\n";
		std::cerr << e.what() << "\n";
		return;
	}

	auto stubs = index(tokens);
}

}

void compileFile(const char* fileName) {
	std::cout << "File " << fileName << ":\n";

	std::ifstream sourceFile{fileName};
	if(!sourceFile.is_open()) {
		std::cerr << "Error: Unable to open file " << fileName << "\n";
		return;
	}

	processFile(sourceFile);

	sourceFile.close();
}

}
