#include "example enum.h"
#include "example enum 2.h"

#include <iostream>
	using namespace std;

void f() {
	cout << MyEnum_v("SomeOther") << endl;
	cout << MyEnum_cstr(MyEnum::OneMore) << endl;

	cout << Enum2_v("one") << endl;
	cout << Enum2_cstr(Enum2::two) << endl;
}
