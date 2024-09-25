#ifndef _ELC_LEXER_STATES_
#define _ELC_LEXER_STATES_

#define DECLSTATE(s) class s : public State { public: void process(Tokenizer& t, int c) final; private:

namespace elc {

class Tokenizer;

class State {
public:
	State() = default;
	virtual ~State() = default;
	State(const State &) = default;
	State(State &&) = default;
	State &operator=(const State &) = default;
	State &operator=(State &&) = default;

	virtual void process(Tokenizer& t, int c) = 0;
};

namespace state {

DECLSTATE(Init) };
DECLSTATE(String) 
	bool slash = false;
};
DECLSTATE(Numeral) 
	bool dot = false;
public:
	explicit Numeral(bool d = false);
};
DECLSTATE(Ident) };
DECLSTATE(Symbol)
	bool empty = true;
public:
	explicit Symbol(bool e = true);
};
DECLSTATE(Minus) };
DECLSTATE(Dot) };

}

}

#endif // !_ELC_LEXER_STATES_
