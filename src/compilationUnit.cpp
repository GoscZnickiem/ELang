#include "compilationUnit.hpp"
#include "data/ast.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "environment.hpp"

#include <fstream>
#include <iostream>
#include <list>

namespace elc {

namespace {

void processFile(std::ifstream& sourceFile) {
	std::list<Token> tokens;
	try{
		tokens = lex(sourceFile);
	} catch (std::runtime_error& e) {
		std::cerr << "\033[1;31m================\n";
		std::cerr << "Lexing error:\033[0m\n";
		std::cerr << e.what() << "\n";
		return;
	}

	std::vector<ast::Declaration> declarations;
	try {
		declarations = parse(tokens);
	} catch (std::runtime_error& e) {
		std::cerr << "\033[1;31m================\n";
		std::cerr << "Parsing error:\033[0m\n";
		std::cerr << e.what() << "\n";
		return;
	}


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
