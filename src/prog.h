#ifndef PROG
#define PROG

struct Statement {

};

struct VarDecl {

};

struct VarAssgn {

};

struct FunDecl {

};

struct FunAssgn {

};

struct Prog {
	struct VarDecl* vardecls;
	struct VarDecl* fundecls;
	struct VarAssgn* varassgns;
	struct VarAssgn* funassgns;
};


#endif // !PROG
