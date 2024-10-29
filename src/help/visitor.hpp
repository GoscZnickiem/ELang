#ifndef _ELC_HELP_VISITOR_
#define _ELC_HELP_VISITOR_

namespace elc {

template<typename ... Callable>
struct visitor : Callable... {
	using Callable::operator()...;
};

}

#endif // !_ELC_HELP_VISITOR_
