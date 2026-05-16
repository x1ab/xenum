/* v0.2
	#include "xenum.h"

	#define MyEnum(X)	\
		X(First)	\
		X(Second)	\
		X(WithVal, 50)	\

	XENUM(MyEnum);
*/

#ifndef ENUD47GYN5WYWT78C468UY0348UY899YC0R70MH87GN7GB5D43A
#define ENUD47GYN5WYWT78C468UY0348UY899YC0R70MH87GN7GB5D43A

#include <string.h>

//----------------------------------------------------------------------------
// API...

#define XENUM(EnumTypeName) \
	_xenum_DEFINE_THE_TYPE(enum, EnumTypeName) \
	_xenum_DEFINE_ACCESSORS(EnumTypeName) \

#define XENUM_CLASS(EnumTypeName) \
	_xenum_DEFINE_THE_TYPE(enum class, EnumTypeName) \
	_xenum_DEFINE_ACCESSORS(EnumTypeName) \

//----------------------------------------------------------------------------
// Impl...

#define _xenum_DEFINE_THE_TYPE(EnumKeyword, EnumTypeName) \
	EnumKeyword EnumTypeName { \
		EnumTypeName(_xenum_ENUM_VALUE) \
	}; \

#define _xenum_DEFINE_ACCESSORS(EnumTypeName) \
	inline const char* EnumTypeName##_cstr(auto value) { \
		using enum EnumTypeName; \
		switch(value) { \
			EnumTypeName(_xenum_ENUM_CASE) \
			default: return ""; /*!! Do some actual error handling here!... !!*/ \
		} \
	} \
	inline auto EnumTypeName##_v(const char *str) { \
		using enum EnumTypeName; \
		EnumTypeName(_xenum_ENUM_STRCMP) \
		return (EnumTypeName)0; /*!! Do some actual error handling here!... !!*/ \
	} \

// Enum element def.:
#define _xenum_ENUM_VALUE(name,...) name __VA_OPT__(= __VA_ARGS__),

// Looped piece of the symbol-to-string converter:
#define _xenum_ENUM_CASE(name,...) case name: return #name;

// Looped piece of the string-to-symbol converter:
#define _xenum_ENUM_STRCMP(name,...) if (!strcmp(str,#name)) {return name;}


#endif // ENUD47GYN5WYWT78C468UY0348UY899YC0R70MH87GN7GB5D43A
