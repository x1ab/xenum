# Minimalist (two-way-)stringified C++ enums in <70 lines

_(-> https://github.com/x1ab/xenum)_

There are infinitely many implementations of enum stringification (because the built-in C++
enums suck); this annoying-yet-fascinating little piece of C++ bullshit, based on (Andrei
Alexandrescu's flavor of) the venerable X-macro technique from the 60s (so even before C!):

- is optimized for a ridiculously frugal implementation, with minimal compilation complexity
  (i.e. fast & almost dependency-free build) and size,

- doesn't have the severe limitations of the typical heavy metaprog. approaches (like a
  crippled range, no duplicate values, narrow definition context, etc.),

- supports bidirectional conv.: both enum-to-string (`const char*`) and string-to-enum.
  (Not from `std::string`, because `<string_view>` is an obscenely heavy header nowadays;
  not including it unconditionally, just for the extra convenience; see the TODOs though.)

- The fallback value to use for invalid names can be customized. (The default is 0.)

- Compiles cleanly even with MSVC's notoriously noisy /Wall

- ...even inside a class.

The cost of all that is a somewhat clunky (but still tolerable) syntax.

------------------------------------------------------------------------------

## Usage

```
	#include "xenum.hh"

	// Definition:

	#define MyEnum(X) X(First), X(Second), X(Fifty, 50), X(NoGood, -1) // You can replace X with whatever you prefer.
	XENUM(MyEnum);
	// Or XENUM_CLASS(MyEnum)
	// Or XENUM(      MyEnum, .null = NoGood); // Optionally set what MyEnum_v("junk") should return. Default: 0.
	// Or XENUM_CLASS(MyEnum, .null = MyEnum::NoGood); // Lame extra qualif. required by C++ for `enum class`.


	// Access:

	cout << MyEnum::First << "\n";      // As usual.
	cout << MyEnum_cstr(Fifty) << "\n"; // Or MyEnum_cstr(MyEnum::Fifty) with XENUM_CLASS.
	cout << MyEnum_v("Second") << "\n"; // If not found: a configured value (or 0).
```

- C++20 is required for `__VA_OPT__`, `using enum`, designated init (for optional
  customizations), and probably a few other things. And `-Zc:preprocessor` for MSVC
  (*sigh*, they can't fix that without breaking their ecosystem).

- Since the enum type is also defined as a macro function (`#define MyEnum(...)`),
  function-style casts like `MyEnum(5)` in client code are syntactically ambiguous,
  and will not work... unless you just write `MyEnum (5)` instead, and swagger on...

------------------------------------------------------------------------------

## Impl.

The generated code would be sg. like this:

```
	enum MyEnum { First, Second, Fifty = 50, };

	struct _xenum_MyEnum_Cfg { MyEnum null; }; // C++ won't let you anon. it and set a var right away, so:
	inline static constexpr _xenum_MyEnum_Cfg _xenum_MyEnum_cfg { }; // Or ...{ .null = ... };

	struct _xenum_MyEnum_Pair { MyEnum v; const char* s; };

	inline const char* MyEnum_cstr(auto value) {
		using enum MyEnum;
		static constexpr _xenum_MyEnum_Pair pairs[] = {
			{ First, "First" },
			{ Second, "Second" },
			{ Fifty, "Fifty" },
		};
		for (const auto& p : pairs) { if (p.v == static_cast<MyEnum>(value)) return p.s; }
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

- I use `auto` instead of the fixed enum type for the enum param in `<MyEnum>_cstr`
  (and also for the retval of `<MyEnum>_v`, for symmetry) plus an explicit `static_cast`,
  to allow supplying computed values (like `Flag1 | Flag2`) without the tedious casting
  C++ would otherwise require in that context.


------------------------------------------------------------------------------

#### Compared to others:

Unlike e.g. the commonly used [magic_enum.hpp](https://github.com/Neargye/magic_enum/blob/master/include/magic_enum/magic_enum.hpp),
which is ~1500 lines(!) of very heavy comp-time payload (including #including
even `<functional>`), this one is ~4% of that, and compiles instantly on a
piece of wood. Also, it doesn't have the embarrassing range limitation (among
others) of magic_enum.

[better_enums](https://github.com/aantron/better-enums) is in the same 1500-line
ballpark, but much lighter than its magic colleague. Still includes more than
just `<string.h>` (namely: `<iosfwd>`, `<stdexcept>`), and does some heavy
comp-time "calculus".
Oh, and it can't be used inside a class!... Umm, and "has a soft limit of 64
declared constants. You can extend it by following ... instructions." And it
also seems to be based on `switch`, which may imply the additional headache
of duplicate values possibly causing a compilation error (didn't verify, but
XENUM used to be implemented that way, with that same inherent limitation).

I've also seen an interestingly brief ad-hoc C++ parser solution mentioned in
https://stackoverflow.com/a/23404302/1479945, but didn't really check, as it
has had the opposite priorities: very ergonomic syntax, but kinda decadent
comp-/runtime costs (compared to XENUM).


**Based on:** https://stackoverflow.com/a/202511/1479945

Improvements to the original example (and some other related ones):

- Renamed macros & functions.
- Eliminated `DEFINE_ENUM` in the cpps.
- Eliminated `DECLARE_ENUM` altogether.
- Reversed the arg order of `DEFINE_ENUM` to be more natural
  when naming the X-macro sg. like `<my enumtype>_DEF`.
- Also applied pmttavara's great suggestion: https://stackoverflow.com/questions/147267/easy-way-to-use-variables-of-enum-types-as-string-in-c/202511#comment83003525_202511
  - So, no more "which arg order" dilemma! :)
- Refactored to be self-containing & self-describing. (Plus README.)
- `__VA_OPT__` trick to simplify assigning values (C++20)
- Scoped enum (`enum class`) support (C++20)
- Customizable default/invalid/null value (for converting from bad names) (C++20)
- Works both in global (namespace) scope and inside classes.
- Duplicate values work fine (unlike the elegant, but fragile switch-based approach).
- Items are separated with commas, not spaces.
- No warnings with the Big Three (at max levels).

------------------------------------------------------------------------------

## TODO

- Config macro for optional std::string/_view support (in case the client TU
  has to include those headers anyway)
  - DO NOT try to detect if the headers have already been included, and enable 
    automatically: inclusion contexts (i.e. order) may differ, leading to
    inconsistent behavior!

- Maybe there could be a way to support it also in local (block) scope?
  (Not really worth the hassle though, of course.)
