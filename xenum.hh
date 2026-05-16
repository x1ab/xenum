/* Minimalist stringified C++ enums (v0.6, https://github.com/x1ab/xenum)

	#include "xenum.hh"

	#define MyEnum(X)	\
		X(First)	\
		X(Second)	\
		X(WithVal, 50)	\
		X(NoGood, -1)	\

	XENUM(MyEnum);
	// Or: XENUM(MyEnum, {.null = NoGood}); // Optionally set what MyEnum_v("garbage") should return. Default: MyEnum{} (i.e. 0).
*/

#ifndef ENUD47GYN5WYWT78C468UY0348UY899YC0R70MH87GN7GB5D43A
#define ENUD47GYN5WYWT78C468UY0348UY899YC0R70MH87GN7GB5D43A

#include <string.h>

//--- API --------------------------------------------------------------------
#define XENUM(EnumTypeName, ...) \
	_xenum_DEFINE_THE_TYPE(enum, EnumTypeName) \
	inline constexpr struct { EnumTypeName null = EnumTypeName{}; } _xenum_##EnumTypeName##_cfg __VA_ARGS__; \
	_xenum_DEFINE_ACCESSORS(EnumTypeName, _xenum_##EnumTypeName##_cfg) \

#define XENUM_CLASS(EnumTypeName, ...) \
	_xenum_DEFINE_THE_TYPE(enum class, EnumTypeName) \
	inline constexpr struct { EnumTypeName null = EnumTypeName{}; } _xenum_##EnumTypeName##_cfg __VA_ARGS__; \
	_xenum_DEFINE_ACCESSORS(EnumTypeName, _xenum_##EnumTypeName##_cfg) \

//--- Impl. ------------------------------------------------------------------
#define _xenum_DEFINE_THE_TYPE(EnumKeyword, EnumTypeName) \
	EnumKeyword EnumTypeName { \
		EnumTypeName(_xenum_ENUM_VALUE) \
	}; \

#define _xenum_DEFINE_ACCESSORS(EnumTypeName, Cfg) \
	inline const char* EnumTypeName##_cstr(auto value) { \
		using enum EnumTypeName; \
		switch (static_cast<EnumTypeName>(value)) { \
			EnumTypeName(_xenum_ENUM_CASE) \
			default: return ""; \
		} \
	} \
	inline auto EnumTypeName##_v(const char *str) { \
		using enum EnumTypeName; \
		EnumTypeName(_xenum_ENUM_STRCMP) \
		return Cfg.null; \
	} \

// Enum element def.:
#define _xenum_ENUM_VALUE(name,...) name __VA_OPT__(= __VA_ARGS__),
// Looped piece of the symbol-to-string converter:
#define _xenum_ENUM_CASE(name,...) case name: return #name;
// Looped piece of the string-to-symbol converter:
#define _xenum_ENUM_STRCMP(name,...) if (!strcmp(str,#name)) {return name;}

#endif // ENUD47GYN5WYWT78C468UY0348UY899YC0R70MH87GN7GB5D43A
