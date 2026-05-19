# Minimalist stringified C++ enums in ~60 lines

_(-> https://github.com/x1ab/xenum)_


There are infinitely many implementations of enum stringification (because the built-in C++
enums are horrible); this one is optimized for ridiculously frugal implementation, with
minimal compilation complexity (i.e. extremely fast & almost dependency-free build), at
the expense of somewhat clunky (but still manageable) syntax.

Supports bidirectional conv.: both enum-to-string (`const char*`) and string-to-enum.
(Not from `std::string`, because `<string_view>` is an obscenely heavy header nowadays;
not adding it by default just for some convenience.)

The fallback value to return for invalid names can be customized. (The default is 0.)


## Usage

```
	#include "xenum.hh"

	// Definition:

	#define MyEnum(X) X(First) X(Second) X(Fifty, 50) X(NoGood, -1) // You can replace X with anything if you prefer.
	XENUM(MyEnum);
	// Or XENUM_CLASS(MyEnum)
	// Or XENUM(      MyEnum, .null = NoGood); // Optionally set what MyEnum_v("junk") should return. Default: 0.
	// Or XENUM_CLASS(MyEnum, .null = MyEnum::NoGood); // Lame extra qualif. required by C++ for `enum class`.


	// Access:

	cout << MyEnum::First << "\n";
	cout << MyEnum_cstr(Fifty) << "\n"; // Or MyEnum_cstr(MyEnum::Fifty) with XENUM_CLASS.
	cout << MyEnum_v("Second") << "\n";
```

C++20 is required for `__VA_OPT__`, `using enum`, and designated init (for optional
customizations). (And `-Zc:preprocessor` for MSVC.)

------------------------------------------------------------------------------

## Impl.

The generated code would be sg. like this:

```
	enum MyEnum { First, Second, Fifty = 50, };

	struct _xenum_MyEnum_Cfg { MyEnum null; }; // Alas, C++ won't let you init a var of it right away, so:
	inline static constexpr _xenum_MyEnum_Cfg _xenum_MyEnum_cfg { }; // Or ...{ .null = ... };

	inline const char* MyEnum_cstr(auto value) {
		using enum MyEnum;
		switch (static_cast<EnumTypeName>(value)) {
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
		return _xenum_MyEnum_cfg.null;
	}
```

#### Note:

- I've used `auto` instead of the fixed enum type for the enum param in `<MyEnum>_cstr`
  (but also for the retval of `<MyEnum>_v`, for symmetry) plus an explicit `static_cast`,
  to allow supplying computed values (like `Flag1 | Flag2`) without the tedious casting
  C++ would otherwise require in that context.


------------------------------------------------------------------------------

#### Compared to others:

Unlike e.g. the commonly used [magic_enum.hpp](https://github.com/Neargye/magic_enum/blob/master/include/magic_enum/magic_enum.hpp),
which is 1500 lines(!), this one is <60 (as of this writing), and trivially compiles
instantly on a piece of wood. (But obviously has way less features in exchange.)

Also unlike e.g. the interesting ad-hoc C++ parser solution seen e.g. in https://stackoverflow.com/a/23404302/1479945
with the opposite priorities: very ergonomic usage syntax, but pretty decadent,
build- and runtime costs, this one uses little C++: no templates, no std::, just
`<string.h>`.


**Based on:** https://stackoverflow.com/a/202511/1479945

Changes to the original example:

- Added the missing `<string.h>` & include guards.
- Renamed the macros & functions.
- Eliminated the use of `DEFINE_ENUM` in the cpps.
- Eliminated `DECLARE_ENUM` altogether.
- Reversed the arg order of `DEFINE_ENUM` to be more natural
  when naming the X-macro sg. like `<my enumtype>_DEF`.
- Also applied pmttavara's great suggestion: https://stackoverflow.com/questions/147267/easy-way-to-use-variables-of-enum-types-as-string-in-c/202511#comment83003525_202511
  - So, no more "which arg order" dilemma! :)
- Refactored to be more self-describing. (Plus comments, README.)
- `__VA_OPT__` trick to simplify assigning values
- Scoped enum (`enum class`) support
- Customizable default/invalid/null value (for converting from bad names)
- Works both in global (namespace) scope and inside classes.


------------------------------------------------------------------------------

## TODO

- Allow duplicate values! The current switch-based approach can't do it:
  ```
	#define MyEnum(X) \
		X(First)  \
		X(Second) \
		X(Other, Second) // Would err out with duplicate `case` value! :-/

	XENUM(MyEnum)
  ```
  Simply turning it also to an if-chain (as in `<MyEnum>_v()`) would solve it,
  but compilers may be more likely to do lookup-table optim. for `switch`.
  Needs a dreaded config macro, probably.

- Maybe there could be a way to support it also in local (block) scope.
