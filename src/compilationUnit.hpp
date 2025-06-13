#ifndef _ELC_COMPILATIONUNIT_
#define _ELC_COMPILATIONUNIT_

#include <fstream>
#include "environment.hpp"
#include "lexer/tokenizer.hpp"
#include "parser/parser.hpp"

namespace elc {

struct CompilationUnit {
	const char* sourceFileName;
	std::ifstream sourceFile;

	Environment env;

	Tokenizer tokenizer;
	Parser parser;

	explicit CompilationUnit(const char *fileName);

	void process();

	CompilationUnit(const CompilationUnit &) = delete;
	CompilationUnit(CompilationUnit &&) = delete;
	CompilationUnit &operator=(const CompilationUnit &) = delete;
	CompilationUnit &operator=(CompilationUnit &&) = delete;
	~CompilationUnit();
};

}

#endif // !_ELC_COMPILATIONUNIT_
