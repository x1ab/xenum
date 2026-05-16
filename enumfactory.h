/*
	#include "enumfactory.h"

	#define MyEnum(X)	\
		X(First)	\
		X(Second)	\
		X(WithValue, 50)	\
		X(OneMore,   100)	\

	DEFINE_ENUM(MyEnum);
*/

#ifndef ENUD47GYN5WYWT78C468UY0348UY899YC0R70MH87GN7GB5D43A
#define ENUD47GYN5WYWT78C468UY0348UY899YC0R70MH87GN7GB5D43A


#include <string.h>

//
// API...
//

#define DEFINE_ENUM(EnumTypeName) \
 \
	_enumf_DEFINE_THE_ENUM_TYPE(EnumTypeName) \
	_enumf_DEFINE_ACCESSORS(EnumTypeName) \

//
// Impl...
//

#define _enumf_DEFINE_THE_ENUM_TYPE(EnumTypeName) \
	enum EnumTypeName { \
		EnumTypeName(_enumf_ENUM_VALUE) \
	}; \

#define _enumf_DEFINE_ACCESSORS(EnumTypeName) \
	inline const char* EnumTypeName##_cstr(EnumTypeName value) \
	{ \
		switch(value) \
		{ \
			EnumTypeName(_enumf_ENUM_CASE) \
			default: return ""; /* handle input error */ \
		} \
	} \
	inline EnumTypeName EnumTypeName##_v(const char *str) \
	{ \
		EnumTypeName(_enumf_ENUM_STRCMP) \
		return (EnumTypeName)0; /* handle input error */ \
	} \


//
// Impl. helpers...
//

// Enum value definition
#define _enumf_ENUM_VALUE(name,...) name __VA_OPT__(= __VA_ARGS__),

// Symbol to string
#define _enumf_ENUM_CASE(name,...) case name: return #name;

// String to symbol
#define _enumf_ENUM_STRCMP(name,...) if (!strcmp(str,#name)) {return name;}


#endif // ENUD47GYN5WYWT78C468UY0348UY899YC0R70MH87GN7GB5D43A
