#include "ast.hpp"
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
		std::cout << "File " << arg << ":\n";
		std::ifstream file(arg);
		if(!file.is_open()) {
			std::cerr << "unable to open file " << arg << "\n";
			continue;
		}

		std::vector<elc::Token> tokens;
		try {
			tokens = elc::tokenize(file);
		} catch (std::runtime_error& e) {
			for(auto& token : tokens) {
				std::cout << token << "\n";
			}
			std::cerr << "\033[1;31m================\n";
			std::cout << "Lexing error:\033[0m\n";
			std::cerr << e.what() << "\n";
		}
		file.close();

		elc::ast::Unit prog;
		try {
			prog = elc::parse(tokens);
		} catch (std::runtime_error& e) {
			std::cerr << "\033[1;31m================\n";
			std::cout << "Parsing error:\033[0m\n";
			std::cerr << e.what() << "\n";
			continue;
		}

		for(auto& e : prog.globals) {
			std::cout << e->toString() << "\n";
		}
	}

}
