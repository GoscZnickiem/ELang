#ifndef _ELC_DATA_OPERATORS_
#define _ELC_DATA_OPERATORS_

#include "typoid.hpp"

#include <string>
#include <vector>


namespace elc::ast::op {

enum class Relation {
	LESS, GREATER, EQUAL, UNDEFINED
};

enum class Associativity {
	LEFT, RIGHT
};

class PrecedenceLevel {
	std::string name;
	Associativity associativity;
	std::vector<PrecedenceLevel*> lessThan; // list of higher precedences

	[[nodiscard]] bool reachable(const PrecedenceLevel& other) const;

public:
	[[nodiscard]] const std::string& getName() const;
	[[nodiscard]] Associativity getAssociativity() const noexcept;
	[[nodiscard]] Relation compare(const PrecedenceLevel& other) const;
	[[nodiscard]] static Relation compare(const PrecedenceLevel& a, const PrecedenceLevel& b);
	void addRelation(Relation r, PrecedenceLevel& other);
	static void addRelation(PrecedenceLevel& a, Relation r, PrecedenceLevel& b);

	PrecedenceLevel(std::string n, Associativity a);
};

struct OperatorParsingData {
	PrecedenceLevel precedence;
	std::vector<std::string> symbols;
	std::vector<build::Typoid> arguments;
	bool isFirstArgBeforeSymbol;
};

}

#endif // !_ELC_DATA_OPERATORS_
