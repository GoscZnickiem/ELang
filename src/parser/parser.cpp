#include "parser.hpp"

#include "tokens.hpp"

#include <cstddef>
#include <map>
#include <utility>
#include <stack>
#include <stdexcept>
#include <iostream>
#include <vector>

namespace elc {

enum Action { SHIFT, REDUCE, ACCEPT, ERROR };

struct ParseTableEntry {
    Action action;
    int rule;
};

const std::map<std::pair<int, TokenType>, ParseTableEntry> parseTable = {
    {{0, TokenType::NUMERAL},	{SHIFT, 5}}
};

// Goto table for non-terminals
const std::map<std::pair<int, char>, int> gotoTable = {
    // Goto after reducing
    {{0, 'S'}, 1},
    {{0, 'T'}, 2},
    {{0, 'F'}, 3},
    // More entries...
};

// Grammar rules (numbered)
struct Rule {
    char lhs; // Left-hand side (non-terminal)
    int rhs_len; // Number of symbols on right-hand side
};

const std::vector<Rule> grammarRules = {
    {'S', 2}, // S -> E ;
    {'E', 1}, // E -> num
    // {'E', 1}, // E -> T
    // {'T', 3}, // T -> T * F
    // {'T', 1}, // T -> F
    // {'F', 3}, // F -> ( E )
    // {'F', 1}, // F -> num
};

struct StackItem {
    int state;
    char symbol;
};

void parse(std::vector<Token> &tokens) {
	std::stack<StackItem> parseStack;
	parseStack.push({0, 'S'});

	std::size_t pos = 0;
    while (true) {
        const int state = parseStack.top().state;
        const Token lookahead = tokens[pos];

        auto action = parseTable.find({state, lookahead.type});
        if (action == parseTable.end() || action->second.action == ERROR) {
			throw std::runtime_error("Syntax error");
        }

        switch (action->second.action) {
            case SHIFT: {
                // Shift: push the new state and the terminal symbol onto the stack
                parseStack.push({action->second.rule, 't'}); // 't' = terminal (for simplicity)
                ++pos; // Consume the token
                break;
            }
            case REDUCE: {
                // Reduce: pop symbols from the stack and replace them with the LHS non-terminal
                const Rule rule = grammarRules[action->second.rule];
                for (int i = 0; i < rule.rhs_len; ++i) {
                    parseStack.pop(); // Pop the RHS symbols
                }

                // Find the next state from the goto table
                const int topState = parseStack.top().state;
                parseStack.push({gotoTable.at({topState, rule.lhs}), rule.lhs});
                break;
            }
            case ACCEPT: {
                std::cout << "Parsing successful!" << std::endl;
                return;
            }
            default:
                std::cerr << "Unknown action!" << std::endl;
                return;
        }
    }
}

}
