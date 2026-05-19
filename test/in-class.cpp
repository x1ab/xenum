#include "xenum.hh"

#include <iostream>

#define MemberEnum(def) \
	def(A) def(B) def(C, 50) def(null, -1)

struct S {
	XENUM(MemberEnum);
};
	XENUM(MemberEnum, .null = null); // Change it to differ from the in-class one, and then test it in main!

int main(int argc, char** argv)
{
	using namespace std;

	cout << S::MemberEnum::C << "\n";
	cout <<    MemberEnum::C << "\n";

	// Invalid:
	cout << "S::"<< "invalid" <<" = "<< S::MemberEnum_v("invalid") << "\n";
	cout <<         "invalid" <<" = "<<    MemberEnum_v("invalid") << "\n";

	// Input:
	if (argc > 1) {
		cout << "S::"<<argv[1] <<" = "<< S::MemberEnum_v(argv[1]) << "\n";
		cout <<        argv[1] <<" = "<<    MemberEnum_v(argv[1]) << "\n";
	}
}
