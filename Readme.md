Overview
--------
This is an implementation to support C++ user-defined fixed width integer literal 
suffixes.

The standard integer literal suffixes like `u`, `ul`, and `ull` suffer from
portability issues similar to C++ native types like `int`, `long`, and `long long`.
While `<cstdint>` provides helpful macros like `UINT64_C(<int-literal>)`, no one
wants to use macros if they don't have to, and they just aren't elegant.

Implemented suffixes include `u8`, `u16`, `u32`, `u64`, `z`, `i8`, `i16`, `i32`, and `i64`.
The `z` type is `size_t` per the `printf` conversion char for size_t as described in
https://en.cppreference.com/w/cpp/io/c/fprintf

The implementation has full support for binary, octal, decimal, and hexadecimal.
Invalid literal values for their type will generate a compiler error via `static_assert()`
and do not require use of the `constexpr` keyword to enforce that. Per the standard,
all suffixes begin with an underscore.

Examples
--------
```cpp
#include "FixedWidthIntLiterals.h"
using namespace scw::intliterals
auto ux = 100_u32;  // ux will be uint32_t
auto uy = 100_u64;  // uy will be uint64_t
auto uz = 0xFF_u8;  // uz will be uint8_t value of 255
auto ix = 100_i32;  // ix is typed as int32_t
auto iy = 100_i64;  // iy is typed as int64_t
auto iz = -50_i8;   // iz is typed as int8_t
auto sz = 100_z;    // sz is typed as size_t
```

Limitations
-----------
A significant problem is using these values with signed integers. Consider an `int8_t`, which has
values from -128 to 127. In terms of the value we receive in our user-defined literal
function, it's just the number with no negative sign. Thus for signed integers we can't
determine when we see '128' if it's a valid negative integer, or an invalid positive
integer. This is just a fundamental limitation of the current user-defined literal 
specification.

The choice was made in this implementation to live with this deficiency and just not allow 
the case of the largest negative integer. Therefore, the following will lead to a compile 
error:
```cpp
   constexpr auto x = -128_i8; // Compile error even though it's a valid literal
```

For this case, the user can always just use the more verbose `numeric_limits` minimum value: 
```cpp
   constexpr auto x = std::numeric_limits<i8>::min();
```

Code Design Notes
-----------------
You can also implement user-defined integer literals that require no parsing with the much 
more simplistic operator form: 
```cpp
    constexpr uint32_t operator"" _u32(ulonglong v)
```
But if you use that form you can't trigger compile time `static_assert` failures without 
explicit use of the `constexpr` keyword in the value declarations. 

Parsing happens in opposite order in terms of how the functions are ordered within
the file. Thus, examine the code by starting at the bottom and working up the file.

