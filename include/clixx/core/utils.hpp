#pragma once
#include "defines.hpp"
NAMESPACE_BEGIN(clixx)
inline NAMESPACE_BEGIN(util)

template<class T>
struct TypeIdentity { using Type = T; };


template<class T>
constexpr auto getIdentity() {
    if constexpr (requires { typename T::value_type; }) {
        return TypeIdentity<typename T::value_type>();
    }
    else if constexpr (requires { typename T::ValueT; }) {
        return TypeIdentity<typename T::ValueT>();
    }
    else {
        static_assert(sizeof(T) == 0, "Could not resolve value type");
    }
}


template<class T>
using ValueTypeOf = decltype(getIdentity<T>())::Type;


// @returns true if `value` is not equal to `cont[idx]`
// @safety performs a bound check
template<class ContainerT> [[nodiscard]]
bool not_at(ValueTypeOf<ContainerT> value, ContainerT cont, usize idx) {
    if (idx > cont.size()) return false;
    return cont[idx] != value;
}


NAMESPACE_END()
NAMESPACE_END()
