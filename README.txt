Based on: https://stackoverflow.com/a/202511/1479945 (by @Suma)

This one compiles on a piece of wood (unlike e.g. the finicky magic_enum.hpp);
it's basically still just plain C, requiring only string.h (enlike the heavy-duty
ad-hoc C++ parser cited e.g. in Debdatta Basu's example (https://stackoverflow.com/a/23404302/1479945), 
which doesn't even achieve that much more than the simple C version in the end).

The smoke-test examples compile just fine with C++11 with Clang/GCC, and C++17
with MSVC (it doesn't recognize -std:c++11), BUT: `__VA_OPT__` in the macros
actually requires C++20 or C23 (and even `-Zc:preprocessor` for MSVC)!


Changes to the original:

- Added the missing string.h.
- Renamed getname to be consistent with its pair.
- Eliminated the use of DEFINE_ENUM in the cpps.
- Eliminated DECLARE_ENUM altogether.
- Reversed the arg order of DEFINE_ENUM to be more natural
  when calling the X macro sg. like <my enumtype>_DEF.
  - But I don't like it better this way after all. :)
- Added include guards.
- Also applied pmttavara's great suggestion: https://stackoverflow.com/questions/147267/easy-way-to-use-variables-of-enum-types-as-string-in-c/202511#comment83003525_202511
  - So, no more "which arg order" dilemma! :)
- Refactored to be more self-describing.

TODO:

- Test/add enum class support
- Test/add enum-in-a-class support
  -> Would obviously require shifting to "real" C++.

- Allow duplicate values! The current switch-based approach can't do it:

	#define MyEnum(X) \
		X(First)  \
		X(Second) \
		X(Other, Second) // This would err out with duplicate `case` value! :-/

	DEFINE_ENUM(MyEnum)
