#include "xenum.hh"

#define Enum2(X) X(one) X(two) X(Unknown, -1)

XENUM_CLASS(Enum2, {.null = Enum2::Unknown}); // Change the default fallback from 0...
	//! NOTE: Enum2 is scoped, so qualifying `Unknown` is required by C++! :-(
