#ifndef _ELC_DATA_SYMBOLS_
#define _ELC_DATA_SYMBOLS_

#include "operators.hpp"
#include "typoid.hpp"

#include <optional>
#include <vector>

namespace elc::ast::build {

enum class SymbolClass {
	ANY, VALUE, FUNCTION, TYPE, PRECEDENCE,
	NAMESPACE
};

struct TokenParsingData {
	Typoid typoid;
	std::optional<op::OperatorParsingData> opData;
};

class Symbol {
	SymbolClass symbolClass;
	std::vector<TokenParsingData> overloads;

public:
	void addOverload(TokenParsingData&& data);
	void setAsNamespace();
};

}


#endif // !_ELC_DATA_SYMBOLS_
