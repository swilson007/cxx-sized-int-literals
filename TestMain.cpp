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

#ifndef SCW_FIXEDWIDTH_INT_LITERALS_NAMESPACE
#define SCW_FIXEDWIDTH_INT_LITERALS_NAMESPACE scw
#endif

#include "FixedWidthIntLiterals.h"

using namespace SCW_FIXEDWIDTH_INT_LITERALS_NAMESPACE::intliterals;

int main(int argc, char* argv[]) {

  // These should cause a compile error if you uncomment them
  {
//    constexpr auto bin1 = 0b111111111_u8;
//    constexpr auto oct1 = 111111_u8;
//    constexpr auto dec1 = 257_u8;
//    constexpr auto hex1 = 0xff01_u8;
//    constexpr auto hex1 = 0xffff12345_u32;
  }

  {
    constexpr auto bin1 = 0b101_u64;
    constexpr auto oct1 = 0101_u64;
    constexpr auto dec1 = 101_u64;
    constexpr auto hex1 = 0xfeF1_u64;

    static_assert(bin1 == 0b101u, "xxx");
    static_assert(oct1 == 0101u, "xxx");
    static_assert(dec1 == 101u, "xxx");
    static_assert(hex1 == 0xfeF1u, "xxx");
    static_assert(std::is_same<const uint64_t, decltype(bin1)>::value, "Broken");
    static_assert(std::is_same<const uint64_t, decltype(oct1)>::value, "Broken");
    static_assert(std::is_same<const uint64_t, decltype(dec1)>::value, "Broken");
    static_assert(std::is_same<const uint64_t, decltype(hex1)>::value, "Broken");
  }

  {
    constexpr auto bin1 = 0b101_u32;
    constexpr auto oct1 = 0101_u32;
    constexpr auto dec1 = 101_u32;
    constexpr auto hex1 = 0xfeF1_u32;

    static_assert(bin1 == 0b101u, "xxx");
    static_assert(oct1 == 0101u, "xxx");
    static_assert(dec1 == 101u, "xxx");
    static_assert(hex1 == 0xfeF1u, "xxx");
    static_assert(std::is_same<const uint32_t, decltype(bin1)>::value, "Broken");
    static_assert(std::is_same<const uint32_t, decltype(oct1)>::value, "Broken");
    static_assert(std::is_same<const uint32_t, decltype(dec1)>::value, "Broken");
    static_assert(std::is_same<const uint32_t, decltype(hex1)>::value, "Broken");
  }

  {
    constexpr auto bin1 = 0b101_i64;
    constexpr auto oct1 = 0101_i64;
    constexpr auto dec1 = 101_i64;
    constexpr auto hex1 = 0xfeF1_i64;

    static_assert(bin1 == 0b101, "xxx");
    static_assert(oct1 == 0101, "xxx");
    static_assert(dec1 == 101, "xxx");
    static_assert(hex1 == 0xfeF1, "xxx");
    static_assert(std::is_same<const int64_t, decltype(bin1)>::value, "Broken");
    static_assert(std::is_same<const int64_t, decltype(oct1)>::value, "Broken");
    static_assert(std::is_same<const int64_t, decltype(dec1)>::value, "Broken");
    static_assert(std::is_same<const int64_t, decltype(hex1)>::value, "Broken");
  }

  {
    constexpr auto bin1 = 0b101_i32;
    constexpr auto oct1 = 0101_i32;
    constexpr auto dec1 = 101_i32;
    constexpr auto hex1 = 0xfeF1_i32;

    static_assert(bin1 == 0b101, "xxx");
    static_assert(oct1 == 0101, "xxx");
    static_assert(dec1 == 101, "xxx");
    static_assert(hex1 == 0xfeF1, "xxx");
    static_assert(std::is_same<const int32_t, decltype(bin1)>::value, "Broken");
    static_assert(std::is_same<const int32_t, decltype(oct1)>::value, "Broken");
    static_assert(std::is_same<const int32_t, decltype(dec1)>::value, "Broken");
    static_assert(std::is_same<const int32_t, decltype(hex1)>::value, "Broken");
  }

  {
    constexpr auto bin1 = 0b101_z;
    constexpr auto oct1 = 0101_z;
    constexpr auto dec1 = 101_z;
    constexpr auto hex1 = 0xfeF1_z;

    static_assert(bin1 == 0b101, "xxx");
    static_assert(oct1 == 0101, "xxx");
    static_assert(dec1 == 101, "xxx");
    static_assert(hex1 == 0xfeF1, "xxx");
    static_assert(std::is_same<const size_t, decltype(bin1)>::value, "Broken");
    static_assert(std::is_same<const size_t, decltype(oct1)>::value, "Broken");
    static_assert(std::is_same<const size_t, decltype(dec1)>::value, "Broken");
    static_assert(std::is_same<const size_t, decltype(hex1)>::value, "Broken");
  }

  {
    constexpr auto x8 = 100_u8;
    constexpr auto x16 = 100_u16;
    static_assert(std::is_same<const uint8_t, decltype(x8)>::value, "Broken");
    static_assert(std::is_same<const uint16_t, decltype(x16)>::value, "Broken");
  }

  {
    static_assert(0b1110101_i32 == INT32_C(0b1110101), "xxx");
    static_assert(0xffeeffdd0012345_u64 == UINT64_C(0xffeeffdd0012345), "xxx");
    static_assert(07766554433_u32 == UINT32_C(07766554433), "xxx");
  }

  return 0;
}
