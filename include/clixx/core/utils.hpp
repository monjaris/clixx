#pragma once
#include "defines.hpp"
#include <type_traits>
NAMESPACE_BEGIN(clixx)
inline NAMESPACE_BEGIN(util)

template<class T>
constexpr auto getValue() {
    if constexpr (requires { typename T::value_type; }) {
        return std::type_identity<typename T::value_type>();
    }
    if constexpr (requires { typename T::value_type; }) {
        return std::type_identity<typename T::value_type>();
    }
    else {
        static_assert(sizeof(T) == 0, "Could not resolve value type");
    }
}


template<class T>
using FetchValueT = decltype(getValue<T>())::type;


template<class ContainerT> [[nodiscard]]
bool not_at(FetchValueT<ContainerT> value, ContainerT cont, usize idx) {
    if (idx > cont.size()) return false;

    return cont[idx] != value;
}


NAMESPACE_END()
NAMESPACE_END()
