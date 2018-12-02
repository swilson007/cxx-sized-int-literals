////////////////////////////////////////////////////////////////////////////////
/// Copyright 2018 Steven C. Wilson
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
/// documentation files (the "Software"), to deal in the Software without restriction, including without
/// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
/// the Software, and to permit persons to whom the Software is furnished to do so, subject to the following
/// conditions:
///
/// The above copyright notice and this permission notice shall be included in all copies or substantial
/// portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
/// LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
/// NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
/// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
/// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// This file contains an implementation to support fixed-size integer literal suffixes.
///
/// Implemented suffixes include u8, u16, u32, u64, z, i8, i16, i32, i64.
/// The 'z' type is size_t per the C++11 printf convention described in
/// https://en.cppreference.com/w/cpp/io/c/fprintf
///
/// The implementation has full support for binary, octal, decimal, and hexadecimal.
/// Invalid literal values for their type will generate a compiler error via static_assert()
/// and do not require use of the 'constexpr' keyword to enforce that.  Per the standard,
/// all suffixes begin with an underscore.
///
/// Examples
/// --------
///  #include "SizedIntLiterals.h"
///  using namespace scw::intliterals;
///  auto ux = 100_u32; // ux will be uint32_t
///  auto uy = 100_u64; // uy will be uint64_t
///  auto uz = 0xFF_u8; // uz will be uint8_t value of 255
///  auto ix = 100_i32; // ix is typed as int32_t
///  auto iy = 100_i64; // iy is typed as int64_t
///  auto iz = -50_i8; // iz is typed as int8_t
///  auto sz = 100_z; // sz is typed as size_t
///
/// Code Design Notes
/// -----------------
/// Parsing happens in opposite order in terms of how the functions are ordered within
/// the file. Thus, examine the code by starting at the bottom and working up in the file.
///
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>

#ifndef SCW_SIZED_INT_LITERALS_NAMESPACE
#define SCW_SIZED_INT_LITERALS_NAMESPACE scw
#endif

namespace SCW_SIZED_INT_LITERALS_NAMESPACE {
namespace intliterals {
namespace detail {

using u64 = std::uint64_t;

////////////////////////////////////////////////////////////////////////////////
/// Calculates value*value
constexpr u64 sqrOf(u64 value) {
  return value * value;
}

////////////////////////////////////////////////////////////////////////////////
/// Calculates: value^n
constexpr u64 powerOf(u64 value, u64 n) {
  return n == 0 ? u64{1} : sqrOf(powerOf(value, n / 2u)) * u64{n % 2 == 0 ? 1 : value};
}

////////////////////////////////////////////////////////////////////////////////
/// Convert the incoming char digit value to the proper numerical value. This function
/// handles 0-9,a-f,A-F digits... so everything.
constexpr u64 digitToValue(char kDigit) {
  return static_cast<u64>(kDigit >= '0' && kDigit <= '9' ?
      kDigit - '0' : ((kDigit >= 'a' && kDigit <= 'f') ? kDigit - 'a' + 10 : kDigit - 'A' + 10));
}

////////////////////////////////////////////////////////////////////////////////
/// Integer parser that knows the correct base of the incoming digits, and
/// kDigits will contain only the final digits to parse. We have two specializations
/// of this to facilitate recursion.
template <u64 kRadix, char... kDigits>
struct ParseIntegerValue;

////////////////////////////////////////////////////////////////////////////////
/// Specialization for only one digit - the recursion base case.
template <u64 kRadix, char kDigit>
struct ParseIntegerValue<kRadix, kDigit> {
  static constexpr u64 parse() { return digitToValue(kDigit); }
};

////////////////////////////////////////////////////////////////////////////////
/// Specialization for multiple digits that recurses.
template <u64 kRadix, char kDigit, char... kDigits>
struct ParseIntegerValue<kRadix, kDigit, kDigits...> {
  static constexpr u64 parse() {
    return (digitToValue(kDigit) * powerOf(kRadix, u64{sizeof...(kDigits)})) +
           ParseIntegerValue<kRadix, kDigits...>::parse();
  }
};

////////////////////////////////////////////////////////////////////////////////
/// This is the primary template for 2 or more digits. The non-specialized template
/// is used for base 10 values. Specializations for other bases follow.
template <char... kDigits>
struct ParseBaseUnknown : public ParseIntegerValue<10, kDigits...> {};
template <char... kDigits>
struct ParseBaseUnknown<'0', kDigits...> : public ParseIntegerValue<8, kDigits...> {};
template <char... kDigits>
struct ParseBaseUnknown<'0', 'b', kDigits...> : public ParseIntegerValue<2, kDigits...> {};
template <char... kDigits>
struct ParseBaseUnknown<'0', 'B', kDigits...> : public ParseIntegerValue<2, kDigits...> {};
template <char... kDigits>
struct ParseBaseUnknown<'0', 'x', kDigits...> : public ParseIntegerValue<16, kDigits...> {};
template <char... kDigits>
struct ParseBaseUnknown<'0', 'X', kDigits...> : public ParseIntegerValue<16, kDigits...> {};

////////////////////////////////////////////////////////////////////////////////
/// The base function to kick off parsing of the digits. This handles the case of just a single
/// digit vs more than one digit. For a single digit, we can just hand it to the base-10
/// parser. Otherwise we let the appropriate parse specialization handle it.
template <char... kDigits>
constexpr u64 createValue() {
  return std::conditional<(sizeof...(kDigits) > 1), ParseBaseUnknown<kDigits...>,
                            ParseIntegerValue<10, kDigits...>>::type::parse();
}

////////////////////////////////////////////////////////////////////////////////
/// Sigh the macro... Can't get a string into a static_assert with a template param,
/// it must be literal, and I don't want to copy/paste the function.
#define SCW_DEFINE_CHECK_VALID_FUNC(typename_, errorMessage_)                      \
  template <u64 kValue>                                                            \
  constexpr typename_ checkValid_##typename_() {                                   \
    static_assert(kValue <= std::numeric_limits<typename_>::max(), errorMessage_); \
    return static_cast<typename_>(kValue);                                         \
  }

// clang-format off
SCW_DEFINE_CHECK_VALID_FUNC(uint8_t, "uint8 literal out of range.");
SCW_DEFINE_CHECK_VALID_FUNC(uint16_t, "uint16 literal out of range.");
SCW_DEFINE_CHECK_VALID_FUNC(uint32_t, "uint32 literal out of range.");
SCW_DEFINE_CHECK_VALID_FUNC(uint64_t, "uint64 literal can't be out of range!");
SCW_DEFINE_CHECK_VALID_FUNC(int8_t, "int8 literal out of range. Note that the value of numeric_limits<int8>::min() is not supported");
SCW_DEFINE_CHECK_VALID_FUNC(int16_t, "int16 literal out of range. Note that the value of numeric_limits<int16>::min() is not supported");
SCW_DEFINE_CHECK_VALID_FUNC(int32_t, "int8 literal out of range. Note that the value of numeric_limits<int32>::min() is not supported");
SCW_DEFINE_CHECK_VALID_FUNC(int64_t, "int64 literal out of range. Note that the value of numeric_limits<int64>::min() is not supported");
SCW_DEFINE_CHECK_VALID_FUNC(size_t, "size_t literal out of range.");
// clang-format on

}  // namespace detail

////////////////////////////////////////////////////////////////////////////////
/// Finally the user-defined literal operators. Again with the macros?
#define SCW_DEFINE_INTEGER_OPERATOR(typesuffix_, typename_)                \
  template <char... digits>                                                \
  constexpr typename_ operator"" typesuffix_() {                        \
    return detail::checkValid_##typename_<detail::createValue<digits...>()>(); \
  }

// clang-format off
SCW_DEFINE_INTEGER_OPERATOR(_u8, uint8_t);
SCW_DEFINE_INTEGER_OPERATOR(_u16, uint16_t);
SCW_DEFINE_INTEGER_OPERATOR(_u32, uint32_t);
SCW_DEFINE_INTEGER_OPERATOR(_u64, uint64_t);
SCW_DEFINE_INTEGER_OPERATOR(_i8, int8_t);
SCW_DEFINE_INTEGER_OPERATOR(_i16, int16_t);
SCW_DEFINE_INTEGER_OPERATOR(_i32, int32_t);
SCW_DEFINE_INTEGER_OPERATOR(_i64, int64_t);
SCW_DEFINE_INTEGER_OPERATOR(_z, size_t);
// clang-format on

}  // namespace intliterals
}  // namespace SCW_SIZED_INT_LITERALS_NAMESPACE

