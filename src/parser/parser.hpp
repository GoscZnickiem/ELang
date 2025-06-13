#ifndef _ELC_PARSER_PARSER_
#define _ELC_PARSER_PARSER_

#include "ast.hpp"
#include "tokens.hpp"

#include <optional>
#include <vector>

namespace elc {

class CompilationUnit;

class Parser {
public:
	explicit Parser(CompilationUnit* root);

	std::optional<ast::Instruction> getInstruction();

private:
	std::vector<Token>* tokenBuffer;
};

}

#endif // !_ELC_PARSER_PARSER_
