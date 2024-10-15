#ifndef _ELC_LEXER_STATES_
#define _ELC_LEXER_STATES_

#include <string>
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

class Init : public State { 
public: 
	void process(Tokenizer& t, int c) final; 
};

class String : public State { 
public: 
	void process(Tokenizer& t, int c) final; 
private:
	bool slash = false;
};

class Numeral : public State { 
public: 
	explicit Numeral(bool d = false);
	void process(Tokenizer& t, int c) final; 
private:
	bool dot = false;
};

class Ident : public State { 
public: 
	void process(Tokenizer& t, int c) final; 
};

class Symbol : public State { 
public: 
	void process(Tokenizer& t, int c) final; 
private:
	std::string buffor;
};

class Dot : public State { 
public: 
	void process(Tokenizer& t, int c) final; 
};

}

}

#endif // !_ELC_LEXER_STATES_
