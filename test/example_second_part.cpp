#include "example enum.h"
#include "example enum class.h"

#include <iostream>
	using namespace std;

void f() {
	cout << MyEnum_v("Fifty") << endl;
	cout << MyEnum_cstr(/*MyEnum::*/OneMore) << endl; // MyEnum_cstr is not scoped.

	cout << (int)Enum2_v("one") << endl; // C++ bullshit: Scoped enums don't << by default. :-/
	cout << Enum2_cstr(Enum2::two) << endl; // Enum2 is scoped (enum class), qualif. required!

	cout << "- invalid str. to val.: " << (int)Enum2_v("Invalid!") << endl; // Should return 0 (... which is debatable, of course!)
	cout << "- invalid val. to str.: " << Enum2_cstr(-999) << endl; // Should compile, and return ""
}
