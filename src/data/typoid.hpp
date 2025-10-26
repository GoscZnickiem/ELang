#ifndef _ELC_DATA_TYPOID_
#define _ELC_DATA_TYPOID_

#include "types.hpp"

#include <optional>

namespace elc::ast::build {

enum class Kind {
	VALUE, TYPE, FUNCTOR, PRECEDENCE
};

struct Typoid {
	Kind kind;
	std::optional<elc::type::CompiledType> type;
};

}


#endif // !_ELC_DATA_TYPOID_
