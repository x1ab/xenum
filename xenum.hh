/* Minimalist stringified C++ enums (v0.12, https://github.com/x1ab/xenum)

	#include "xenum.hh"

	#define MyEnum(X)       \
		X(First),       \
		X(Second),      \
		X(WithVal, 50), \
		X(NoGood, -1)     // A trailing , is also fine.

	XENUM(MyEnum);
	// Or: XENUM(MyEnum, .null = NoGood); // Optionally set what MyEnum_v("garbage") should return. Default: 0.
*/

#ifndef ENUD47GYN5WYWT78C468UY0348UY899YC0R70MH87GN7GB5D43A
#define ENUD47GYN5WYWT78C468UY0348UY899YC0R70MH87GN7GB5D43A

#include <string.h>

//--- API --------------------------------------------------------------------
// Note: The cfg. struct can't be anon., C++ gets anal with that. (MSVC (and pro'ly GCC-ext.) flies through it just fine.)
#define XENUM(EnumTypeName, ...) \
	_xenum_DEFINE_THE_TYPE(enum, EnumTypeName) \
	struct _xenum_CPP_bullshit_##EnumTypeName##_Cfg { EnumTypeName null; }; inline static constexpr _xenum_CPP_bullshit_##EnumTypeName##_Cfg _xenum_##EnumTypeName##_cfg { __VA_ARGS__ }; \
	_xenum_DEFINE_ACCESSORS(EnumTypeName, _xenum_##EnumTypeName##_cfg) \

#define XENUM_CLASS(EnumTypeName, ...) \
	_xenum_DEFINE_THE_TYPE(enum class, EnumTypeName) \
	struct _xenum_CPP_bullshit_##EnumTypeName##_Cfg { EnumTypeName null; }; inline static constexpr _xenum_CPP_bullshit_##EnumTypeName##_Cfg _xenum_##EnumTypeName##_cfg { __VA_ARGS__ }; \
	_xenum_DEFINE_ACCESSORS(EnumTypeName, _xenum_##EnumTypeName##_cfg) \

//--- Impl. ------------------------------------------------------------------
#define _xenum_DEFINE_THE_TYPE(EnumKeyword, EnumTypeName) \
	EnumKeyword EnumTypeName { EnumTypeName(_xenum_ENUM_VALUE) };

	//! All this (+ using Pad later) just to suppress the "x bytes padding added" MSVC warning C4820 (with /Wall)...
	#if _MSC_VER >= 1929 // https://devblogs.microsoft.com/cppblog/msvc-cpp20-and-the-std-cpp20-switch/#c++20-[[no_unique_address]]
	# define _XENUM_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
	#else
	# define _XENUM_NO_UNIQUE_ADDRESS [[no_unique_address]]
	#endif
	namespace _xenum_ { template <int N> struct Pad { char _[N]; };
	                    template <>      struct Pad<0> {}; }

#define _xenum_DEFINE_ACCESSORS(EnumTypeName, Cfg) \
	struct _xenum_##EnumTypeName##_Pair { const char* s; EnumTypeName v; /* In this order to manage padding, just to pacify MSVC /Wall... */ \
		 _XENUM_NO_UNIQUE_ADDRESS _xenum_::Pad<(sizeof(const char*) > sizeof(EnumTypeName)) ? sizeof(const char*) - sizeof(EnumTypeName) : 0> _pad{}; \
	}; \
	[[maybe_unused]] inline static const char* EnumTypeName##_cstr(auto value) { \
		using enum EnumTypeName; \
		static constexpr _xenum_##EnumTypeName##_Pair pairs[] = { EnumTypeName(_xenum_ENUM_PAIR) }; \
		for (const auto& p : pairs) { if (p.v == static_cast<EnumTypeName>(value)) return p.s; } \
		return ""; \
	} \
	[[maybe_unused]] inline static auto EnumTypeName##_v(const char *str) { \
		using enum EnumTypeName; \
		static constexpr _xenum_##EnumTypeName##_Pair pairs[] = { EnumTypeName(_xenum_ENUM_PAIR) }; \
		for (const auto& p : pairs) { if (!strcmp(p.s, str)) return p.v; } \
		return Cfg.null; \
	} \

// Enum element def.:
#define _xenum_ENUM_VALUE(name,...) name __VA_OPT__(= __VA_ARGS__)
// Looped piece for the array (order matters: the larger ptr first, enum + padding(!) last):
#define _xenum_ENUM_PAIR(name,...) { #name, name }

#endif // ENUD47GYN5WYWT78C468UY0348UY899YC0R70MH87GN7GB5D43A
