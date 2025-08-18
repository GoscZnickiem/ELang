#ifndef _ELC_HELP_VISITOR_
#define _ELC_HELP_VISITOR_

//////////////////////////////////////////////////////////////////////////////
///
/// A visitor?
/// Hmm... Indeed, I have coded long enough.
/// The University of Wroclaw has long since forgotten my name,
/// And I am EAGER to make them remember.
/// However,
/// The blood of variants stains your hands, and I must admit...
/// I'm curious about your skills, Compiler.
/// And so, before I tear down the templates and CRUSH the functions of std...
/// You shall do as an appetizer.
/// Come forth, Child of Man...
/// And DIE.
///
//////////////////////////////////////////////////////////////////////////////
template<typename ... Callable>
struct visitor : Callable... {
	using Callable::operator()...;
};

#endif // !_ELC_HELP_VISITOR_
