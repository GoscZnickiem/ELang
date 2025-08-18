#ifndef _ELC_HELP_VARIANT_
#define _ELC_HELP_VARIANT_

#include <type_traits>
#include <variant>
#include <optional>

template<typename T>
concept stdvariant = requires(T a) {
	typename std::variant_size<T>::type;
};

template<typename T, stdvariant Var, bool = false>
struct addToVariantImpl;
template<typename T, stdvariant Var>
struct addToVariantImpl<T, Var, true> {
	using type = Var;
};
template<typename T, typename...U>
struct addToVariantImpl<T, std::variant<U...>, false> {
	using type = std::variant<T, U...>;
};

template<typename T, stdvariant Var>
struct addToVariant;
template<typename T, typename...U>
struct addToVariant<T, std::variant<U...>> {
	using type = typename addToVariantImpl<T, std::variant<U...>, std::disjunction_v<std::is_same<T, U>...>>::type;
};

template<typename...Vars>
struct variantImpl;
template<typename...T, typename...U>
struct variantImpl<std::variant<T...>, U...>{
	using type = typename variantImpl<T..., U...>::type;
};
template<typename T, typename...U>
struct variantImpl<T, U...>{
	using type = typename addToVariant<T, typename variantImpl<U...>::type>::type;
};
template<stdvariant V>
struct variantImpl<V>{
	using type = V;
};
template<typename T>
struct variantImpl<T>{
	using type = std::variant<T>;
};

template<typename...T>
using variant = variantImpl<T...>::type;

template <typename T, typename Variant>
struct matchesVariantT;

template <typename T, typename... Types>
struct matchesVariantT<T, std::variant<Types...>> 
	: std::disjunction<std::is_same<T, Types>...> {};

template <typename VariantSubset, typename T>
std::optional<VariantSubset> matchesVariant(const T& value) {
	if constexpr (matchesVariantT<T, VariantSubset>::value) {
		return VariantSubset{value};
	} else {
		return std::nullopt;
	}
}

#endif // !_ELC_HELP_VARIANT_
