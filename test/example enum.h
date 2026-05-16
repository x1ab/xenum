#include "enumfactory.h"

#define MyEnum(X)	\
	X(First)	\
	X(Second)	\
	X(SomeOther, 50)	\
	X(OneMore,   100)	\

DEFINE_ENUM(MyEnum)
