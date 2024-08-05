#include <fstream>
#include <iostream>
#include <string>
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

		std::string line;
		while(std::getline(file, line)) {
			std::cout << line << "\n";
		}

		file.close();
	}

}
