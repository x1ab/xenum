#include "example enum.h"

#include <iostream>
	using namespace std;

int main(int, char**) {

	cout << "Smoke-test:\n";

	cout <<"\t"<< MyEnum::First << endl;
	cout <<"\t"<< MyEnum_cstr(/*MyEnum::*/First) << endl; // MyEnum_cstr is not scoped.

	void f();
	f();
}
