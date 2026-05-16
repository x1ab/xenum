#include "xenum.h"

#define MyEnum(X)	\
	X(First)	\
	X(Second)	\
	X(SomeOther, 50)	\
	X(OneMore,   100)	\

//!! Alas, this would err out for duplicate `case` value though:
	X(OneMore,   Second)

DEFINE_ENUM(MyEnum)
