#include "ast.hpp"
#include "compiler/compiler.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "tokens.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

int main(int argc, char *argv[]) {

	if(argc < 2) {
		std::cerr << "no arguments\n";
	}

	// Parse arguments:
	
	std::vector<const char*> arguments;
	
	for(int i = 1; i < argc; i++) {
		char* arg = argv[i];
		if(arg[0] == '-') {
			if(arg[1] == 'h') {
				std::cout << "help\n";
			} else {
				std::cout << "option " << arg << "\n";
			}
		} else {
			std::cout << "argument " << arg << "\n";
			arguments.push_back(arg);
		}
	}

	for(auto& arg : arguments) {

		std::vector<elc::Token> tokens;
		try {
			tokens = elc::tokenize(file);
		} catch (std::runtime_error& e) {
			for(auto& token : tokens) {
				std::cout << token << "\n";
			}
			std::cerr << "\033[1;31m================\n";
			std::cerr << "Lexing error:\033[0m\n";
			std::cerr << e.what() << "\n";
		}

		elc::ast::Unit unit;
		try {
			unit = elc::parse(tokens);
			unit.name = arg;
		} catch (std::runtime_error& e) {
			std::cerr << "\033[1;31m================\n";
			std::cerr << "Parsing error:\033[0m\n";
			std::cerr << e.what() << "\n";
			continue;
		}

		for(auto& e : unit.globals) {
			std::cout << elc::ast::astToString(e) << "\n";
		}

		std::cout << "\n";
		elc::compile(unit);
	}

}
