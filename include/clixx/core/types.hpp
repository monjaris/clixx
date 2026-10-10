#pragma once

namespace types
{

// only function in the header. casts a value(should be an lvalue) to T
template<class TargetT>
constexpr TargetT cast(const auto& value) {
    return static_cast<TargetT>(value);
}


using usize = __SIZE_TYPE__;
using isize = __PTRDIFF_TYPE__;

using char8 = char;
using uchar8 = unsigned char;
using char16 = char16_t;
using char32 = char32_t;

using int8 = signed char;
using int16 = __INT16_TYPE__;
using int32 = __INT32_TYPE__;
using int64 = __INT64_TYPE__;

using uint8 = unsigned char;
using uint16 = __UINT16_TYPE__;
using uint32 = __UINT32_TYPE__;
using uint64 = __UINT64_TYPE__;

using real32 = float;
using real64 = double;

using nullptrT = decltype(nullptr);


static_assert(sizeof(char8) == 1, "char8 must be 1 byte");
static_assert(sizeof(int8) == 1, "int8 must be 1 byte");
static_assert(sizeof(uint8) == 1, "uint8 must be 1 byte");

static_assert(sizeof(char16) == 2, "char16 must be 2 bytes");
static_assert(sizeof(int16) == 2, "int16 must be 2 bytes");
static_assert(sizeof(uint16) == 2, "uint16 must be 2 bytes");

static_assert(sizeof(char32) == 4, "char32 must be 4 bytes");
static_assert(sizeof(int32) == 4, "int32 must be 4 bytes");
static_assert(sizeof(uint32) == 4, "uint32 must be 4 bytes");
static_assert(sizeof(real32) == 4, "fp32 must be 4 bytes");

static_assert(sizeof(int64) == 8, "int64 must be 8 bytes");
static_assert(sizeof(uint64) == 8, "uint64 must be 8 bytes");
static_assert(sizeof(real64) == 8, "fp64 must be 8 bytes");



// check if T and U are same type
template<class T, class U> inline constexpr bool is_same = false;
template<class T> inline constexpr bool is_same<T, T> = true;
template<class T, class U> concept IdConcept = is_same<T, U>;


// check if T and U are same type (variadic)
template<class T, class... U> inline constexpr bool is_any_same = (is_same<T, U> || ...);
template<class T, class... U>
concept IdsConcept = is_any_same<T, U...>;


// get underlying type via compiler instrinct
template<class T> using EnumValueT = __underlying_type(T);
// get integral value of an enum class member
template<class EnumT, EnumValueT<EnumT> x>
EnumT enum_v = cast<EnumT>(x);


template<class T> struct IsRefImpl { constexpr static bool value = false; };
template<class T> struct IsRefImpl<T&> { constexpr static bool value = true; };
template<class T> struct IsRefImpl<T&&> { constexpr static bool value = false; };
//
template<class T> struct IsRvRefImpl { constexpr static bool value = false; };
template<class T> struct IsRvRefImpl<T&> { constexpr static bool value = false; };
template<class T> struct IsRvRefImpl<T&&> { constexpr static bool value = true; };
//
template<class T> struct IsConstImpl { constexpr static bool value = false; };
template<class T> struct IsConstImpl<T&> { constexpr static bool value = false; };
template<class T> struct IsConstImpl<T&&> { constexpr static bool value = true; };

// Is reference
template<class T>
bool is_ref = IsRefImpl<T>::value;
// Is rvalue reference
template<class T>
bool is_rvref = IsRvRefImpl<T>::value;
// Is const
template<class T>
bool is_const = IsConstImpl<T>::value;


template<class T> struct RemoveReferenceImpl { using Type = T; };
template<class T> struct RemoveReferenceImpl<T&> { using Type = T; };
template<class T> struct RemoveReferenceImpl<T&&> { using Type = T; };
//
template<class T> struct RemoveRvalueReferenceImpl { using Type = T; };
template<class T> struct RemoveRvalueReferenceImpl<T&&> { using Type = T; };
//
template<class T> struct RemoveConstImpl { using Type = T; };
template<class T> struct RemoveConstImpl<const T> { using Type = T; };


// Remove reference from T
template<class T> using UnRef = RemoveReferenceImpl<T>::Type;
// Remove rvalue reference from T
template<class T> using UnRvRef = RemoveRvalueReferenceImpl<T>::Type;
// Remove const from T
template<class T> using UnConst = RemoveConstImpl<T>::Type;
// Remove const and reference from T
template<class T> using UnRefConst = RemoveConstImpl<RemoveReferenceImpl<T>>::Type;


template<class T> constexpr bool is_triv = __is_trivial(T);
template<class T> constexpr bool is_triv_copyable = __is_trivially_copyable(T);
template<class T> constexpr bool is_triv_destructible = __is_trivially_destructible(T);


template<class T> struct UnDeduceImpl { using Type = T; };
template<class T> using UnDeduce = UnDeduceImpl<T>::Type;


template<class CharT>
concept CharConcept = (
    is_any_same<CharT,
        char, types::char8, types::uchar8, types::char16, types::char32
    >
);

template<class IntT>
concept IntConcept = (
    is_any_same<IntT,
        types::int8, types::int16, types::int32, types::int64
    >
);

template<class UintT>
concept UintConcept = (
    is_any_same<UintT, types::uint8, types::uint16, types::uint32, types::uint64>
);

template<class FloatT>
concept FloatConcept = (
    is_any_same<FloatT, types::real32, types::real64>
);


}

