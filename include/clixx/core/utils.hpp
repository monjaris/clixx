#pragma once
#include "defines.hpp"
CLIXX_NAMESPACE_BEGIN()
namespace util {


template<class T>
struct TypeIdentity { using Type = T; };


template<class T>
class Ref
{
    T* m_ptr = nullptr;

public:
    explicit Ref(const T& ref)
        : m_ptr(ADDRESS_OF(ref))
    {}

    T& operator-> () {
        return *m_ptr;
    }
};


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
constexpr bool not_at(ValueTypeOf<ContainerT> value, ContainerT cont, usize idx) {
    if (idx > cont.size()) return false;
    return cont[idx] != value;
}


[[nodiscard]] constexpr usize cstrLen(const char* cstr) {
    usize len = 0;
    while (*cstr++) ++len;
    return len;
}


[[nodiscard]] constexpr usize max(usize x, usize y) {
    return (x > y)? (x):(y);
}


CLIXX_NAMESPACE_END()
CLIXX_NAMESPACE_END()
