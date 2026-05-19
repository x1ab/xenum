# Minimalist (two-way-)stringified C++ enums in <70 lines

_(-> https://github.com/x1ab/xenum)_


There are infinitely many implementations of enum stringification (because the built-in C++
enums are horrible); this one (based on X-Macros) is optimized for a ridiculously frugal
implementation, with minimal compilation complexity (i.e. fast & almost dependency-free
build), at the expense of somewhat clunky (but still tolerable) syntax.

Supports bidirectional conv.: both enum-to-string (`const char*`) and string-to-enum.
(Not from `std::string`, because `<string_view>` is an obscenely heavy header nowadays;
not including it unconditionally, just for the extra convenience.)

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

	struct _xenum_MyEnum_Pair { MyEnum v; const char* s; };

	inline const char* MyEnum_cstr(auto value) {
		using enum MyEnum;
		static constexpr _xenum_MyEnum_Pair pairs[] = {
			{ First, "First" },
			{ Second: return "Second" },
			{ Fifty: return "Fifty" },
		};
		for (const auto& p : pairs) { if (p.v == static_cast<EnumTypeName>(value)) return p.s; }
		return "";
	}

	auto MyEnum_v(const char *str) {
		using enum MyEnum;
		static constexpr _xenum_MyEnum_Pair pairs[] = { ...same... };
		for (const auto& p : pairs) { if (!strcmp(p.s, str)) return p.v; }
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
which is 1500 lines(!), this one is <70 (as of this writing), and trivially compiles
instantly on a piece of wood. (But obviously has way less features in exchange.)

Also unlike e.g. the interesting ad-hoc C++ parser solution seen e.g. in https://stackoverflow.com/a/23404302/1479945
with the opposite priorities: very ergonomic usage syntax, but pretty decadent,
build- and runtime costs, this one uses very little C++, no std, only string.h.


**Based on:** https://stackoverflow.com/a/202511/1479945

Changes to the original example (and to earlier versions):

- Added the missing `<string.h>` & include guards.
- Renamed the macros & functions.
- Eliminated the use of `DEFINE_ENUM` in the cpps.
- Eliminated `DECLARE_ENUM` altogether.
- Reversed the arg order of `DEFINE_ENUM` to be more natural
  when naming the X-macro sg. like `<my enumtype>_DEF`.
- Also applied pmttavara's great suggestion: https://stackoverflow.com/questions/147267/easy-way-to-use-variables-of-enum-types-as-string-in-c/202511#comment83003525_202511
  - So, no more "which arg order" dilemma! :)
- Refactored to be more self-describing. (Plus comments, README.)
- `__VA_OPT__` trick to simplify assigning values (C++20!)
- Scoped enum (`enum class`) support (C++20!)
- Customizable default/invalid/null value (for converting from bad names) (C++20!)
- Works both in global (namespace) scope and inside classes.
- Duplicate values work fine (unlike the elegant, but fragile switch-based approach).
- Items are separated with commas, not spaces.

------------------------------------------------------------------------------

## TODO

- Config macro for optional std::string/_view support (in case the client TU
  has to include those headers anyway)
  - DO NOT try to detect if the headers have already been included, and enable 
    automatically: inclusion contexts (i.e. order) may differ, leading to
    inconsistent behavior!

- Maybe there could be a way to support it also in local (block) scope?
  (Not really worth the hassle though, of course.)
