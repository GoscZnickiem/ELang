#include "lexer/lexer.hpp"
#include <fstream>
#include <iostream>
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

		auto tokens = elc::tokenize(file);
		file.close();

		for(auto& token : tokens) {
			std::cout << token << "\n";
		}

	}

}
