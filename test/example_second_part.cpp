#include "example enum.h"
#include "example enum class.h"

#include <iostream>
	using namespace std;

void f() {

	cout << "Plain enum:\n";

	cout <<"\t"<< MyEnum_v("Fifty") << endl;
	cout <<"\t"<< MyEnum_cstr(/*MyEnum::*/OneMore) << endl; // MyEnum_cstr is not scoped.
	cout <<"\t"<< "- invalid str. to val. (should be 0): " << MyEnum_v("Invalid!") << endl;

	cout << "Enum class:\n";

	cout <<"\t"<< (int)Enum2_v("one") << endl; // C++ bullshit: Scoped enums don't << by default. :-/
	cout <<"\t"<< Enum2_cstr(Enum2::two) << endl; // Enum2 is scoped (enum class), qualif. required!
	cout <<"\t"<< "- invalid str. to val. (should be -1): " << (int)Enum2_v("Invalid!") << endl;
	cout <<"\t"<< "- invalid val. to str. (should be empty): " << Enum2_cstr(-999) << endl; // Should compile, and return ""
}
