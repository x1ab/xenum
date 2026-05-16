#include "example enum.h"

#include <iostream>
	using namespace std;

int main(int, char**) {
	cout << MyEnum::First << endl;
	cout << MyEnum_cstr(MyEnum::First) << endl;

	void f();
	f();
}
