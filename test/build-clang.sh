#!/bin/sh

clang++ -Wall -Wextra -pedantic -std=c++20 -I.. -o  example-clang.test example_first_part.cpp example_second_part.cpp $*
clang++ -Wall -Wextra -pedantic -std=c++20 -I.. -o in-class-clang.test in-class.cpp                                   $*
