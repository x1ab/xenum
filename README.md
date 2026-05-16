# Minimal-code-cost stringified C/C++ enums


There are infinitely many implementations of this (because the built-in C/C++ enums
are horrible); this one is optimized for minimal complexity (i.e. very fast & almost
dependency-free build), at the expense of somewhat clunky (but still manageable) syntax.


## Usage

```
	#include "xenum.h"

	// Definition:

	#define MyEnum(X) X(First) X(Second) X(Fifty, 50) // You can replace X with anything you prefer.
	XENUM(MyEnum); // Or XENUM_CLASS(MyEnum)

	// Access:

	cout << MyEnum::First << "\n";
	cout << MyEnum_v("Second") << "\n";
	cout << MyEnum_cstr(Fifty) << "\n"; // Or MyEnum_cstr(MyEnum::Fifty) with XENUM_CLASS.
```

------------------------------------------------------------------------------

## Impl.

The generated code would be sg. like this:

```
	enum MyEnum { First, Second, Fifty = 50, };

	inline const char* MyEnum_cstr(auto value) {
		using enum MyEnum;
		switch(value) {
			case First: return "First";
			case Second: return "Second";
			case Fifty: return "Fifty";
			default: return "";
		}
	}

	auto MyEnum_v(const char *str) {
		using enum MyEnum;
		if (!strcmp(str,"First")) return First;
		if (!strcmp(str,"Second")) return Second;
		if (!strcmp(str,"Fifty")) return Fifty;
		return (SomeEnum)0;
	}
```

------------------------------------------------------------------------------

Based on: https://stackoverflow.com/a/202511/1479945 (by @Suma)

This one compiles on a piece of wood (unlike e.g. the finicky magic_enum.hpp);
it's still almost plain C, requiring only string.h (unlike the heavy ad-hoc
C++ parser cited e.g. in Debdatta Basu's example (https://stackoverflow.com/a/23404302/1479945), 
which doesn't even achieve all that much more in the end).

The smoke-test examples compile just fine with C++11 with Clang/GCC, and C++17
with MSVC (it doesn't recognize `-std:c++11`), BUT: `__VA_OPT__` in the macros
actually requires C++20 (and even `-Zc:preprocessor` for MSVC)!


Changes to the original:

- Added the missing string.h.
- Renamed getname to be consistent with its pair.
- Eliminated the use of `DEFINE_ENUM` in the cpps.
- Eliminated `DECLARE_ENUM` altogether.
- Reversed the arg order of `DEFINE_ENUM` to be more natural
  when naming the X-macro sg. like `<my enumtype>_DEF`.
  - But I don't like it better this way, after all. :)
- Added include guards.
- Also applied pmttavara's great suggestion: https://stackoverflow.com/questions/147267/easy-way-to-use-variables-of-enum-types-as-string-in-c/202511#comment83003525_202511
  - So, no more "which arg order" dilemma! :)
- Refactored to be more self-describing.
- `__VA_OPT__` trick to simplify assigning values
- Scoped enum (`enum class`) support.


------------------------------------------------------------------------------

## TODO

- Add enum-in-a-class support (-> How to (re)scope the conv. functions?)

- Allow duplicate values! The current switch-based approach can't do it:

	#define MyEnum(X) \
		X(First)  \
		X(Second) \
		X(Other, Second) // This would err out with duplicate `case` value! :-/

	XENUM(MyEnum)
