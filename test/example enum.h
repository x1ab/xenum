#include "xenum.hh"

#define MyEnum(X)        \
	X(First)         \
	X(Second)        \
	X(Fifty, 50)     \
	X(OneMore, 100)
	//!! Alas, this would err out for "duplicate `case` value":
	//!!	X(OneMore, Second)

XENUM(MyEnum)

// Or, customized (with non-0 default/fallback value):
//XENUM(MyEnum, .null = -1) // Won't compile! (Kinda fair, but MyEnum{} would, even if no item is 0...)
//XENUM(MyEnum, .null = 50) // STILL won't compile! :-/ (Annoying AF.)
//XENUM(MyEnum, .null = (MyEnum)50)   // OK. (But still annoying AF.)
//XENUM(MyEnum, .null = (MyEnum)-666) // Hmm: MSVC & GCC eat this (not even warn!), Clang errs out.
